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
#define PING_TIMEOUT 2
#define PING_COUNT 1
#define CHECK_INTERVAL 60
#define LOG_FILE "/tmp/cbs_network.log"

// NetworkManager状态枚举
typedef enum {
    NM_STATE_UNKNOWN = 0,
    NM_STATE_ASLEEP = 10,
    NM_STATE_DISCONNECTED = 20,
    NM_STATE_DISCONNECTING = 30,
    NM_STATE_CONNECTING = 40,
    NM_STATE_CONNECTED_LOCAL = 50,
    NM_STATE_CONNECTED_SITE = 60,
    NM_STATE_CONNECTED_GLOBAL = 70
} NMState;

// 网络类型枚举
typedef enum {
    NETWORK_WIFI,
    NETWORK_ETHERNET,
    NETWORK_UNKNOWN
} NetworkType;

// 网络状态结构体
typedef struct {
    char interface_name[32];
    char ip_address[16];
    char gateway[16];
    char connection_name[64];
    char ssid[64];
    char signal_strength[8];
    char dns_servers[256];
    int is_active;
    int has_ip;
    int has_gateway;
    int dns_working;
    NMState nm_state;
} NetworkStatus;

// 函数声明
void initializeMonitor(void);
void cleanupMonitor(void);
int executeCommand(const char* command, char* output, size_t outputSize);
void logError(const char *message, NetworkType networkType);
NMState getNMState(void);
void getWiFiDetails(NetworkStatus* status);
void getConnectionDetails(NetworkStatus* status);
void getNetworkStatus(NetworkStatus* status, const char* interface);
NetworkType getCurrentNetworkType(void);
int checkInternetConnection(void);
void monitorNetwork(void);

#endif // NETWORK_MONITOR_H