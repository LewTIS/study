#include "network.h"

static volatile int running = 1;
static char last_error[MAX_BUFFER_SIZE] = {0};

// 信号处理函数
static void signal_handler(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        running = 0;
    }
}

// 初始化监控器
void initializeMonitor(void) {
    // 设置信号处理
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

    // 记录启动信息
    logError("Network monitoring service started", NETWORK_UNKNOWN);
}

// 清理函数
void cleanupMonitor(void) {
    logError("Network monitoring service stopped", NETWORK_UNKNOWN);
}

// 执行系统命令
int executeCommand(const char* command, char* output, size_t outputSize) {
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

    int status = pclose(pipe);
    if (status == -1) {
        snprintf(last_error, sizeof(last_error), 
                "Command close error: %s (errno: %d)", 
                command, errno);
        return -1;
    }

    return WEXITSTATUS(status);
}

// 记录错误信息
void logError(const char *message, NetworkType networkType) {
    time_t now;
    time(&now);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    const char* networkTypeStr;
    switch (networkType) {
        case NETWORK_WIFI:
            networkTypeStr = "WiFi";
            break;
        case NETWORK_ETHERNET:
            networkTypeStr = "Ethernet";
            break;
        default:
            networkTypeStr = "Unknown";
    }

    FILE *fp = fopen(LOG_FILE, "a");
    if (fp != NULL) {
        fprintf(fp, "[%s] Network Type: %s - %s\n", 
                timeStr, networkTypeStr, message);
        fclose(fp);
        fprintf(stderr, "[%s] Network Type: %s - %s\n", 
                timeStr, networkTypeStr, message);
    }
}

// 获取NetworkManager状态
NMState getNMState(void) {
    char command[] = "nmcli -t -f STATE g";
    char output[MAX_BUFFER_SIZE];
    
    if (executeCommand(command, output, sizeof(output)) == 0) {
        output[strcspn(output, "\n")] = 0;
        
        if (strcmp(output, "connected") == 0) return NM_STATE_CONNECTED_GLOBAL;
        if (strcmp(output, "asleep") == 0) return NM_STATE_ASLEEP;
        if (strcmp(output, "disconnected") == 0) return NM_STATE_DISCONNECTED;
        if (strcmp(output, "connecting") == 0) return NM_STATE_CONNECTING;
    }
    
    return NM_STATE_UNKNOWN;
}

// 获取WiFi详细信息
void getWiFiDetails(NetworkStatus* status) {
    char command[MAX_BUFFER_SIZE];
    char output[MAX_BUFFER_SIZE];

    snprintf(command, sizeof(command), 
             "nmcli -t -f active,ssid,signal dev wifi | grep '^yes'");
    
    if (executeCommand(command, output, sizeof(output)) == 0) {
        char* token = strtok(output, ":");
        if (token) token = strtok(NULL, ":"); // skip 'yes'
        if (token) {
            strncpy(status->ssid, token, sizeof(status->ssid) - 1);
            token = strtok(NULL, ":");
            if (token) {
                strncpy(status->signal_strength, token, sizeof(status->signal_strength) - 1);
            }
        }
    }
}

// 获取连接详细信息
void getConnectionDetails(NetworkStatus* status) {
    char command[MAX_BUFFER_SIZE];
    char output[MAX_BUFFER_SIZE];

    // 获取活动连接名称
    snprintf(command, sizeof(command), "nmcli -t -f NAME c show --active");
    if (executeCommand(command, output, sizeof(output)) == 0) {
        output[strcspn(output, "\n")] = 0;
        strncpy(status->connection_name, output, sizeof(status->connection_name) - 1);
    }

    // 获取IP信息
    if (strlen(status->connection_name) > 0) {
        snprintf(command, sizeof(command), 
                "nmcli -t -f IP4.ADDRESS c show '%s'", status->connection_name);
        if (executeCommand(command, output, sizeof(output)) == 0) {
            char* token = strtok(output, ":");
            if (token) token = strtok(NULL, ":");
            if (token) {
                strncpy(status->ip_address, token, sizeof(status->ip_address) - 1);
            }
        }

        // 获取网关信息
        snprintf(command, sizeof(command), 
                "nmcli -t -f IP4.GATEWAY c show '%s'", status->connection_name);
        if (executeCommand(command, output, sizeof(output)) == 0) {
            char* token = strtok(output, ":");
            if (token) token = strtok(NULL, ":");
            if (token) {
                strncpy(status->gateway, token, sizeof(status->gateway) - 1);
            }
        }

        // 获取DNS服务器信息
        snprintf(command, sizeof(command), 
                "nmcli -t -f IP4.DNS c show '%s'", status->connection_name);
        if (executeCommand(command, output, sizeof(output)) == 0) {
            char* token = strtok(output, ":");
            if (token) 
                token = strtok(NULL, ":");
            if (token) {
                strncpy(status->dns_servers, token, sizeof(status->dns_servers) - 1);
            }
        }
    }
}

// 获取网络状态
void getNetworkStatus(NetworkStatus* status, const char* interface) {
    memset(status, 0, sizeof(NetworkStatus));
    strncpy(status->interface_name, interface, sizeof(status->interface_name) - 1);

    status->nm_state = getNMState();

    if (status->nm_state == NM_STATE_CONNECTED_GLOBAL) {
        getConnectionDetails(status);
        
        if (strncmp(interface, "wlan", 4) == 0) {
            getWiFiDetails(status);
        }

        status->is_active = 1;
        status->has_ip = (strlen(status->ip_address) > 0);
        status->has_gateway = (strlen(status->gateway) > 0);
        status->dns_working = (system("nslookup localhost > /dev/null 2>&1") == 0);
    }
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

// 检查互联网连接
int checkInternetConnection(void) {
    const char* test_hosts[] = {"8.8.8.8", "1.1.1.1", "208.67.222.222"};
    const int num_hosts = sizeof(test_hosts) / sizeof(test_hosts[0]);
    char command[MAX_BUFFER_SIZE];

    for (int i = 0; i < num_hosts; i++) {
        snprintf(command, sizeof(command), 
                "ping -c %d -W %d %s > /dev/null 2>&1", 
                PING_COUNT, PING_TIMEOUT, test_hosts[i]);
        if (system(command) == 0) {
            return 1;
        }
    }
    return 0;
}

// 监控网络状态
void monitorNetwork(void) {
    static NetworkType lastNetwork = NETWORK_UNKNOWN;
    static NMState lastNMState = NM_STATE_UNKNOWN;
    NetworkStatus status;
    char message[MAX_BUFFER_SIZE];
    
    NetworkType currentNetwork = getCurrentNetworkType();
    
    // 获取当前网络状态
    getNetworkStatus(&status, 
                     currentNetwork == NETWORK_WIFI ? "wlan0" : "eth0");

    // 检测网络类型变化
    if (currentNetwork != lastNetwork) {
        snprintf(message, sizeof(message), 
                "Network type changed from %s to %s", 
                lastNetwork == NETWORK_WIFI ? "WiFi" : 
                (lastNetwork == NETWORK_ETHERNET ? "Ethernet" : "Unknown"),
                currentNetwork == NETWORK_WIFI ? "WiFi" : 
                (currentNetwork == NETWORK_ETHERNET ? "Ethernet" : "Unknown"));
        logError(message, currentNetwork);
        lastNetwork = currentNetwork;
    }

    // 检测NetworkManager状态变化
    if (status.nm_state != lastNMState) {
        snprintf(message, sizeof(message), 
                "NetworkManager state changed: %d -> %d", 
                lastNMState, status.nm_state);
        logError(message, currentNetwork);
        lastNMState = status.nm_state;
    }

    // 记录详细的连接信息
    if (status.nm_state == NM_STATE_CONNECTED_GLOBAL) {
        snprintf(message, sizeof(message), 
                "Connection: %s, IP: %s, Gateway: %s, DNS: %s%s%s", 
                status.connection_name, 
                status.ip_address, 
                status.gateway,
                status.dns_servers,
                strlen(status.ssid) > 0 ? ", SSID: " : "",
                strlen(status.ssid) > 0 ? status.ssid : "");
        logError(message, currentNetwork);

        // 检查互联网连接
        if (!checkInternetConnection()) {
            logError("CBS lost its connection to the internet", currentNetwork);
        }
    } else if (status.nm_state == NM_STATE_DISCONNECTED) {
        logError("CBS unable to connect to the local network", currentNetwork);
    }
}

int main(void) {
    initializeMonitor();

    printf("Network monitoring started...\n");

    while (running) {
        monitorNetwork();
        sleep(CHECK_INTERVAL);
    }

    cleanupMonitor();
    return 0;
}