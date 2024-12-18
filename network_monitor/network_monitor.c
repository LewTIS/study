#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <linux/if.h>

void send_notification(const char *interface, const char *message) {
    char command[256];
    snprintf(command, sizeof(command), "notify-send 'Network Status' '%s: %s'", interface, message);
    system(command);
    printf("interface: %s, message: %s\n");
}

int main() {
    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR; // 监听链路和IPv4地址变化

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1; 
    }

    char buffer[4096];
    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            continue;
        }

        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
        for (; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len)) {
            if (nlh->nlmsg_type == RTM_NEWLINK) {
                struct ifinfomsg *ifi = NLMSG_DATA(nlh);
                char ifname[IF_NAMESIZE];
                if_indextoname(ifi->ifi_index, ifname);

                if (strcmp(ifname, "wlan0") == 0 || strcmp(ifname, "wlp2s0") == 0) {
                    if (ifi->ifi_flags & IFF_LOWER_UP) {
                        send_notification(ifname, "Wi-Fi connected");
                    } else {
                        send_notification(ifname, "Wi-Fi disconnected");
                    }
                } else if (strcmp(ifname, "eth0") == 0 || strcmp(ifname, "enp0s3") == 0) {
                    if (ifi->ifi_flags & IFF_LOWER_UP) {
                        send_notification(ifname, "Ethernet cable connected");
                    } else {
                        send_notification(ifname, "Ethernet cable disconnected");
                    }
                }
            } else if (nlh->nlmsg_type == RTM_NEWADDR) {
                struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
                char ifname[IF_NAMESIZE];
                if_indextoname(ifa->ifa_index, ifname);

                if (strcmp(ifname, "wlan0") == 0 || strcmp(ifname, "wlp2s0") == 0 || strcmp(ifname, "eth0") == 0 || strcmp(ifname, "enp0s3") == 0) {
                    send_notification(ifname, "IP address assigned");
                }
            } else if (nlh->nlmsg_type == RTM_DELADDR) {
                struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
                char ifname[IF_NAMESIZE];
                if_indextoname(ifa->ifa_index, ifname);

                if (strcmp(ifname, "wlan0") == 0 || strcmp(ifname, "wlp2s0") == 0 || strcmp(ifname, "eth0") == 0 || strcmp(ifname, "enp0s3") == 0) {
                    send_notification(ifname, "IP address removed");
                }
            }
        }
    }

    close(sock);
    return 0;
}