#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

//发送桌面通知
void send_notification(const char *message) {
    char command[256];
    snprintf(command, sizeof(command), "notify-send 'Network Status' '%s' -u normal -i network-wired", message);
    system(command);
}

int main() {
    //创建Netlink socket,用于与内核通信，接收网络接口状态变化的消息
    //AF_NETLINK:使用Netlink协议
    //SOCK_RAW:使用原始套接字
    //NETLINK_ROUTE:用于路由信息的Netlink消息类型
    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl addr;//这是 Netlink 地址结构体,指定 Netlink 套接字的地址信息
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK; //地址族字段，用于指定套接字地址族,表示 Netlink 地址族
    addr.nl_groups = RTMGRP_LINK; //指定进程想要订阅的多播组,这是一个多播组掩码，表示接收与网络接口相关的事件
    
    //将套接字绑定到指定的地址上
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1; 
    }

    //接收消息
    char buffer[4096];
    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            continue;
        }
    //解析消息
        // nlmsghdr:Netlink 消息头结构体
        /*
        struct nlmsghdr {
            __u32 nlmsg_len;    消息长度，包括消息头 
            __u16 nlmsg_type;   消息类型 
            __u16 nlmsg_flags;  消息标志 
            __u32 nlmsg_seq;    序列号 
            __u32 nlmsg_pid;    发送者 PID 
        };
        */
        //将buffer中的数据转换为Netlink消息头结构体
        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;
    //遍历消息
        //NLMSG_OK(nlh, len):用于检查 Netlink 消息是否合法，即消息是否完整
        //NLMSG_NEXT(nlh, len):用于获取下一个消息
        for (; NLMSG_OK(nlh, len); nlh = NLMSG_NEXT(nlh, len)) {
            //RTM_NEWLINK:常量，表示有一个新的网络接口事件
            if (nlh->nlmsg_type == RTM_NEWLINK) {
                //NLMSG_DATA(nlh)：用于获取 Netlink 消息数据部分的指针。它会跳过消息头，指向实际的数据部分。
                //当 Netlink 消息的类型是 RTM_NEWLINK 时，消息数据部分包含的是 struct ifinfomsg 结构体
                struct ifinfomsg *ifi = NLMSG_DATA(nlh);
            //判断网络接口是否处于活动状态
                //ifi->ifi_flags:表示网络接口的标志，其中 IFF_RUNNING(0x40) 表示网络接口的物理层是否已经建立连接并处于运行状态
                if (ifi->ifi_flags & IFF_RUNNING) {
                    printf("Interface is UP\n");
                    send_notification("Ethernet cable connected");
                } else {
                    printf("Interface is DOWN\n");
                    send_notification("Ethernet cable disconnected");
                }
            }
        }
    }

    close(sock);
    return 0;
}