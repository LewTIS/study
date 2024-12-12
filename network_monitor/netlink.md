### 1.Netlink简介
#### 什么是Netlink
 - 内核与用户空间通信的机制
 - 基于套接字的接口
 -  Netlink 的历史背景和发展过程
#### Netlink的特点
 - 双向通信
 - 异步通信
 - 多播支持
#### Netlink的协议簇
 - NETLINK_ROUTE:路由协议，用于管理路由表，路由表由路由协议维护，路由协议通过netlink协议栈与用户态进行通信。
- 其他协议类  型
#### Netlink 的工作原理
 - 如何在内核和用户空间之间传递消息。
### 2.系统架构
 - 硬件层 -> 内核 -> Netlink -> 用户程序
 - 使用图表展示从硬件事件到用户空间程序的完整数据流。
 - 对每个层次的详细解释，如内核如何生成和处理 Netlink 消息。
### 3.核心数据结构
 - Netlink 消息头
 ```c
 struct nlmsghdr {
    __u32 nlmsg_len;    // 消息长度
    __u16 nlmsg_type;   // 消息类型
    __u16 nlmsg_flags;  // 消息标志
    __u32 nlmsg_seq;    // 序列号
    __u32 nlmsg_pid;    // 发送进程ID
};
 ``` 
 - 消息体部分：网络接口信息结构
 ```c
 struct ifinfomsg {
	unsigned char	ifi_family;
	unsigned char	__ifi_pad;
	unsigned short	ifi_type;		/* ARPHRD_* */
	int		ifi_index;		/* Link index	*/
	unsigned	ifi_flags;		/* IFF_* flags	*/
	unsigned	ifi_change;		/* IFF_* change mask */
};
```
### 4.实现步骤
1.创建Netlink套接字
2.配置和绑定地址
3.接收消息
4.消息解析
5.网络接口状态判断和处理
每个步骤的详细代码示例和解释
### 关键技术
### 5.代码实现
详细讲解
### 6.测试和验证
#### 测试环境
#### 测试用例
- 网线插入
- 网线拔出
- 异常情况
#### 测试结果
### 演示重点:
- 工作原理
- 实现方法
- 实际效果
- 应用场景
### 图表
流程图
架构图
代码高亮
演示截图
