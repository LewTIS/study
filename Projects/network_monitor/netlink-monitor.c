#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <time.h>
#include <linux/if.h>

// 日志文件路径
#define LOG_FILE "/tmp/network-status.log"

// 记录日志
void write_log(const char *message) {
    time_t now;
    struct tm *timeinfo;
    char timestamp[64];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s - %s\n", timestamp, message);
        fclose(log_file);
    }
}

// 发送通知
void send_notification(const char *interface, const char *status) {
    char message[256];
    snprintf(message, sizeof(message), "Interface: %s Status: %s", interface, status);
    
    write_log(message);
}

// 获取接口初始状态
int get_interface_state(const char *ifname) {
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return 0;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
    
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        close(sockfd);
        return 0;
    }
    
    close(sockfd);
    return (ifr.ifr_flags & IFF_LOWER_UP) ? 1 : 0;
}

int main() {
    // 初始化状态
    int last_wlan_state = get_interface_state("wlan0");
    int last_eth_state = get_interface_state("eth0");

    // 创建 Netlink socket
    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK;

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

                // 判断接口类型
                if (strcmp(ifname, "wlan0") == 0) {
                    if ((ifi->ifi_flags & IFF_LOWER_UP) && !last_wlan_state) {
                        printf("%s is connected\n", ifname);
                        send_notification(ifname, "connected");
                        last_wlan_state = 1;
                    } else if (!(ifi->ifi_flags & IFF_LOWER_UP) && last_wlan_state) {
                        printf("%s is disconnected\n", ifname);
                        send_notification(ifname, "disconnected");
                        last_wlan_state = 0;
                    }
                } else if (strcmp(ifname, "eth0") == 0) {
                    if ((ifi->ifi_flags & IFF_LOWER_UP) && !last_eth_state) {
                        printf("%s is connected\n", ifname);
                        send_notification(ifname, "connected"); 
                        last_eth_state = 1;
                    } else if (!(ifi->ifi_flags & IFF_LOWER_UP) && last_eth_state) {
                        printf("%s is disconnected\n", ifname);
                        send_notification(ifname, "disconnected");
                        last_eth_state = 0;
                    }
                }
            }
        }
    }

    close(sock);
    return 0;
}