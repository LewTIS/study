#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024
#define LOG_FILE "/tmp/cbs_network.log"
#define ERROR_LOG_FILE "/tmp/cbs_network_error.log"
#define CHECK_INTERVAL 60
#define PING_TIMEOUT 2
#define PING_COUNT 1

// 网络类型
typedef enum {
    NETWORK_WIFI,
    NETWORK_ETHERNET,
    NETWORK_UNKNOWN
} NetworkType;

// 错误类型
typedef enum {
    ERROR_NONE,
    ERROR_INTERNET_LOST,      // 互联网连接丢失
    ERROR_LOCAL_NETWORK_LOST, // 本地网络连接丢失
    ERROR_UNKNOWN
} NetworkError;

// 网络状态
typedef struct {
    NetworkType type;         // 当前网络类型
    char interface[32];       // 网络接口名称
    char ip_address[16];      // IP地址
    char gateway[16];         // 网关地址
    char dns_servers[256];    // DNS服务器
    char ssid[64];           // WiFi SSID（如果是WiFi）
    int is_connected;        // 是否连接
    NetworkError last_error; // 最后一次错误
    time_t last_error_time; // 最后一次错误时间
} NetworkStatus;

// 函数声明
void initNetworkMonitor(void);
void cleanupNetworkMonitor(void);
NetworkType getCurrentNetworkType(void);
int checkLocalNetwork(NetworkStatus* status);
int checkInternetConnection(NetworkStatus* status);
void logNetworkError(NetworkStatus* status, NetworkError error);
void monitorNetwork(void);
const char* getNetworkTypeString(NetworkType type);
const char* getErrorTypeString(NetworkError error);

#endif 