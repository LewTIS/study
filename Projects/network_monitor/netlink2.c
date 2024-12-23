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
#include <linux/if.h>
#include <arpa/inet.h>

#define INET_ADDRSTRLEN 16

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
void send_notification(const char *ifname, const char *status, const char *ip) {
    char message[256];
    snprintf(message, sizeof(message), "Interface: %s, Status: %s, IP: %s", ifname, status, ip);
    write_log(message);
}
void send_notification2(const char *ifname, const char *status){
    char message[256];
    snprintf(message, sizeof(message), "Interface: %s, Status: %s", ifname, status);
    write_log(message);

}
// 获取接口名称
void get_interface_name(int ifindex, char *ifname) {
    if_indextoname(ifindex, ifname);
}

// 处理 RTM_NEWADDR 消息
void handle_newaddr(struct nlmsghdr *nlh) {
    struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
    struct rtattr *rta = IFA_RTA(ifa);
    int rta_len = IFA_PAYLOAD(nlh);
    char ip[INET_ADDRSTRLEN] = "";
    char ifname[IF_NAMESIZE] = "";

    get_interface_name(ifa->ifa_index, ifname);

    for (; RTA_OK(rta, rta_len); rta = RTA_NEXT(rta, rta_len)) {
        if (rta->rta_type == IFA_ADDRESS) {
            inet_ntop(AF_INET, RTA_DATA(rta), ip, sizeof(ip));
        }
    }

    if (strlen(ip) > 0) {
        send_notification(ifname, "IP assigned", ip);
    } else {
        send_notification(ifname, "IP assignment failed", "Not assigned");
    }
}

// 处理 RTM_DELADDR 消息
void handle_deladdr(struct nlmsghdr *nlh) {
    struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
    char ifname[IF_NAMESIZE] = "";

    get_interface_name(ifa->ifa_index, ifname);

    send_notification(ifname, "IP unassigned", "Not assigned");
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
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR; // 监听接口状态和 IPv4 地址变化
    
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
            switch (nlh->nlmsg_type) {
                case RTM_NEWLINK:
                {
                    struct ifinfomsg *ifi = NLMSG_DATA(nlh);
                    char ifname[IF_NAMESIZE];
                    if_indextoname(ifi->ifi_index, ifname);

                    if (ifi->ifi_flags & IFF_LOWER_UP) {
                        send_notification2(ifname, "connected");
                    } else {
                        send_notification2(ifname, "disconnected");
                    }
                }
                    break;
                case RTM_NEWADDR:
                    handle_newaddr(nlh);
                    break;
                case RTM_DELADDR:
                    handle_deladdr(nlh);
                    break;
            }
        }
    }
    close(sock);
    return 0;
}