#include "network_monitor.h"

static volatile int running = 1;
static char last_error[MAX_BUFFER_SIZE] = {0};

// 信号处理
static void signal_handler(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        running = 0;
    }
}

// 执行命令并获取输出
static int executeCommand(const char* command, char* output, size_t outputSize) {
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        snprintf(last_error, sizeof(last_error), 
                "Failed to execute command: %s (errno: %d)", 
                command, errno);
        return -1;
    }

    memset(output, 0, outputSize);
    if (fgets(output, outputSize, pipe) == NULL) {
        pclose(pipe);
        snprintf(last_error, sizeof(last_error), 
                "No output from command: %s", command);
        return -1;
    }

    return pclose(pipe);
}

// 获取当前网络类型
NetworkType getCurrentNetworkType(void) {
    char command[] = "nmcli -t -f TYPE c show --active";
    char output[MAX_BUFFER_SIZE];
    
    if (executeCommand(command, output, sizeof(output)) == 0) {
        output[strcspn(output, "\n")] = 0;
        if (strstr(output, "802-11-wireless") != NULL) {
            return NETWORK_WIFI;
        } else if (strstr(output, "802-3-ethernet") != NULL) {
            return NETWORK_ETHERNET;
        }
    }
    
    return NETWORK_UNKNOWN;
}

// 检查本地网络连接
int checkLocalNetwork(NetworkStatus* status) {
    char command[MAX_BUFFER_SIZE];
    char output[MAX_BUFFER_SIZE];
    int local_connection = 1;

    // 1. 检查接口状态
    printf("Debug: Checking interface %s\n", status->interface);
    snprintf(command, sizeof(command), 
             "ip link show %s | grep 'state UP' > /dev/null 2>&1", 
             status->interface);
    if (system(command) != 0) {
        
        local_connection = 0;
        goto check_end;
    }

    // 2. 检查IP地址
    snprintf(command, sizeof(command), 
             "ip addr show %s | grep 'inet ' | awk '{print $2}' | cut -d/ -f1", 
             status->interface);
    if (executeCommand(command, output, sizeof(output)) == 0) {
        // 移除输出中的换行符
        output[strcspn(output, "\n")] = 0;
        
        // 检查是否获取到IP地址
        if (strlen(output) == 0) {
            printf("Debug: No IP address found for interface %s\n", 
                   status->interface);
            local_connection = 0;
            goto check_end;
        }

        // 直接复制IP地址
        strncpy(status->ip_address, output, sizeof(status->ip_address) - 1);
        status->ip_address[sizeof(status->ip_address) - 1] = '\0';  // 确保字符串结束
        
        printf("Debug: IP address obtained: %s\n", status->ip_address);
    }

    // 3. 检查网关
    snprintf(command, sizeof(command), 
             "ip route | grep default | grep %s | awk '{print $3}'", 
             status->interface);
    if (executeCommand(command, output, sizeof(output)) == 0) {
        // 移除输出中的换行符
        output[strcspn(output, "\n")] = 0;
        if (strlen(output) > 0) {
            strncpy(status->gateway, output, sizeof(status->gateway) - 1);
            // 测试网关可达性
            snprintf(command, sizeof(command), 
                    "ping -c 1 -W 1 %s > /dev/null 2>&1", 
                    status->gateway);
            if (system(command) != 0) {
                local_connection = 0;
                goto check_end;
            }
        }
        printf("Debug: Gateway obtained: %s\n", status->gateway);
    }

check_end:
    if (!local_connection && status->last_error != ERROR_LOCAL_NETWORK_LOST) {
        logNetworkError(status, ERROR_LOCAL_NETWORK_LOST);
    }
    return local_connection;
}

// 检查互联网连接
int checkInternetConnection(NetworkStatus* status) {
   
    char command[MAX_BUFFER_SIZE];
    int internet_connection = 0;
    const char* test_hosts[] = {"8.8.8.8", "1.1.1.1", "208.67.222.222"};
    
    // 检查DNS解析
    if (system("nslookup www.google.com > /dev/null 2>&1") == 0 ||
        system("nslookup www.baidu.com > /dev/null 2>&1") == 0) {
        
        // 检查公网连接
        for (size_t i = 0; i < sizeof(test_hosts)/sizeof(test_hosts[0]); i++) {
            snprintf(command, sizeof(command), 
                    "ping -c %d -W %d %s > /dev/null 2>&1", 
                    PING_COUNT, PING_TIMEOUT, test_hosts[i]);
            if (system(command) == 0) {
                internet_connection = 1;
                break;
            }
        }
    }

    if (!internet_connection && status->last_error != ERROR_INTERNET_LOST) {
        logNetworkError(status, ERROR_INTERNET_LOST);
    }

    return internet_connection;
}

// 记录网络错误
void logNetworkError(NetworkStatus* status, NetworkError error) {
    time_t now;
    time(&now);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    const char* errorMsg = (error == ERROR_INTERNET_LOST) ?
                          "CBS lost its connection to the internet" :
                          "CBS unable to connect to the local network";

    // 更新状态
    status->last_error = error;
    status->last_error_time = now;

    // 写入错误日志
    FILE *fp = fopen(ERROR_LOG_FILE, "a");
    if (fp != NULL) {
        fprintf(fp, "[%s] Network Type: %s - %s\n", 
                timeStr, 
                getNetworkTypeString(status->type), 
                errorMsg);
        fprintf(fp, "Interface: %s, IP: %s, Gateway: %s%s%s\n",
                status->interface,
                status->ip_address,
                status->gateway,
                strlen(status->ssid) > 0 ? ", SSID: " : "",
                strlen(status->ssid) > 0 ? status->ssid : "");
        fclose(fp);
    }

    // 同时输出到标准错误
    fprintf(stderr, "[%s] %s - %s\n", timeStr, 
            getNetworkTypeString(status->type), errorMsg);
}

// 监控网络状态
void monitorNetwork(void) {
    NetworkStatus status = {0};
    static NetworkType lastType = NETWORK_UNKNOWN;
    
    // 获取当前网络类型
    status.type = getCurrentNetworkType();
    strncpy(status.interface, 
            status.type == NETWORK_WIFI ? "wlan0" : "enp0s3", 
            sizeof(status.interface) - 1);

    // 检测网络类型变化
    if (status.type != lastType) {
        char message[MAX_BUFFER_SIZE];
        snprintf(message, sizeof(message), 
                "Network type changed from %s to %s",
                getNetworkTypeString(lastType),
                getNetworkTypeString(status.type));
        
        FILE *fp = fopen(LOG_FILE, "a");
        if (fp != NULL) {
            fprintf(fp, "%s\n", message);
            fclose(fp);
        }
        lastType = status.type;
    }

    // 检查网络连接
    if (!checkLocalNetwork(&status)) {
        return;
    }
    if (!checkInternetConnection(&status)) {
       return;
    }
 
}

// 初始化
void initNetworkMonitor(void) {
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // 确保日志文件可写
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open log file %s (errno: %d)\n", 
                LOG_FILE, errno);
        exit(1);
    }
    fclose(fp);

    fp = fopen(ERROR_LOG_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open error log file %s (errno: %d)\n", 
                ERROR_LOG_FILE, errno);
        exit(1);
    }
    fclose(fp);
}

// 清理
void cleanupNetworkMonitor(void) {
    // 记录服务停止信息
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp != NULL) {
        fprintf(fp, "Network monitoring service stopped\n");
        fclose(fp);
    }
}

// 工具函数
const char* getNetworkTypeString(NetworkType type) {
    switch (type) {
        case NETWORK_WIFI: return "WiFi";
        case NETWORK_ETHERNET: return "Ethernet";
        default: return "Unknown";
    }
}

const char* getErrorTypeString(NetworkError error) {
    switch (error) {
        case ERROR_INTERNET_LOST: return "Internet Connection Lost";
        case ERROR_LOCAL_NETWORK_LOST: return "Local Network Lost";
        case ERROR_NONE: return "No Error";
        default: return "Unknown Error";
    }
}

// 主函数
int main(void) {
    initNetworkMonitor();
    printf("Network monitoring started...\n");

    while (running) {
        monitorNetwork();
        sleep(CHECK_INTERVAL);
    }

    cleanupNetworkMonitor();
    return 0;
}