#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <time.h>
#include <linux/if.h>
#include <stdbool.h>

// 日志文件路径
#define LOG_FILE "/tmp/network-status.log"

// 记录日志
void write_log(const char *message) {
    time_t now;
    struct tm *timeinfo;
    char timestamp[64];
    
    // 获取当前时间
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // 日志
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s - %s\n", timestamp, message);
        fclose(log_file);
    }
}

// 发送通知（仅记录日志）
void send_notification(const char *ifname, const char *status, const char *ip, const char *gateway) {
    char message[256];
    snprintf(message, sizeof(message), "Interface: %s, Status: %s, IP: %s, Gateway: %s", ifname, status, ip, gateway);
    write_log(message);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 获取 IP 和网关信息
bool get_ip_and_gateway(const char *ifname, char *ip, char *gateway) {
    char command[256];
    char line[256];
    FILE *fp;

    // 初始化 IP 和网关为 "Not assigned"
    strcpy(ip, "Not assigned");
    strcpy(gateway, "Not assigned");

    // 构建命令以获取 IP 地址
    snprintf(command, sizeof(command), "ip addr show %s | grep 'inet ' | awk '{print $2}' | cut -d'/' -f1", ifname);

    // 执行命令并读取输出
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return false;
    }

    // 读取 IP 地址
    if (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // 去掉换行符
        strncpy(ip, line, 15);
        ip[15] = '\0';
    }

    pclose(fp);

    // 构建命令以获取网关地址
    snprintf(command, sizeof(command), "ip route show default dev %s | awk '{print $3}'", ifname);

    // 执行命令并读取输出
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return false;
    }

    // 读取网关地址
    if (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // 去掉换行符
        strncpy(gateway, line, 15);
        gateway[15] = '\0';
    }

    pclose(fp);

    return true;
}
int main() {
    // 创建 Netlink socket, 用于与内核通信，接收网络接口状态变化的消息
    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK; // 监听接口状态

    // 将套接字绑定到指定的地址上
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1; 
    }

    // 接收消息
    char buffer[4096];
    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            continue;
        }

        // 解析消息
        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
        for (; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len)) {
            if (nlh->nlmsg_type == RTM_NEWLINK) {
                struct ifinfomsg *ifi = NLMSG_DATA(nlh);
                char ifname[IF_NAMESIZE];
                if_indextoname(ifi->ifi_index, ifname);

                char ip[16] = "";
                char gateway[16] = "";
                if (strcmp(ifname, "eth0") == 0 || strcmp(ifname, "enp0s3") == 0) { // ethernet接口名称

                    if (ifi->ifi_flags & IFF_LOWER_UP) {
                        sleep(2);
                        if (get_ip_and_gateway(ifname, ip, gateway)) {
                            send_notification(ifname, "connected", ip, gateway); 
                        } else {
                            send_notification(ifname, "connected", "Failed to get IP", "Failed to get gateway");
                        }
                        
                    } else {
                        sleep(2);
                        if (get_ip_and_gateway(ifname, ip, gateway)) {
                            send_notification(ifname, "disconnected", ip, gateway);
                        } else {
                            send_notification(ifname, "disconnected", ip, gateway);
                        }    
                    }    
                }else if (strcmp(ifname, "wlan0") == 0 || strcmp(ifname, "wlp2s0") == 0){
                    if (ifi->ifi_flags & IFF_LOWER_UP) {
                        sleep(2);
                        if (get_ip_and_gateway(ifname, ip, gateway)) {
                            send_notification(ifname, "connected", ip, gateway); 
                        } else {
                            send_notification(ifname, "connected", "Failed to get IP", "Failed to get gateway");
                        }
                        
                    } else {
                        sleep(2);
                        if (get_ip_and_gateway(ifname, ip, gateway)) {
                            send_notification(ifname, "disconnected", ip, gateway);
                        } else {
                            send_notification(ifname, "disconnected", ip, gateway);
                        }    
                    }    
                }
            }
        }
    }
    close(sock);
    return 0;
}