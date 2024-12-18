#!/bin/bash

# 定义日志文件路径
LOG_FILE="/tmp/network-status.log"

# 获取当前时间戳
timestamp=$(date +"%Y-%m-%d %H:%M:%S")

# 获取 interface 和触发的 action
INTERFACE=$1
ACTION=$2

# 定义日志函数
log_message() {
    local message="$1"
    echo "[$timestamp] $message" >> "$LOG_FILE"
    echo "[$timestamp] $message"
}
#echo $INTERFACE $ACTION
# 检查 action
case "$ACTION" in
    carrier)
        # 网线插入或 Wifi 连接
        CARRIER_STATUS=$(cat /sys/class/net/$INTERFACE/carrier 2>/dev/null)

        if [ "$CARRIER_STATUS" -eq 1 ]; then
            log_message "interface $INTERFACE is connected"
        else
            log_message "interface $INTERFACE is disconnected"
        fi
        ;;
    dhcp4-change)
        # dhcp4 ip地址变化
        IP=$(ip -4 addr show $INTERFACE | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
        if [ -z "$IP" ]; then
            log_message "interface $INTERFACE get IP address failed"
        else
            log_message "interface $INTERFACE got IP address $IP"
        fi
        ;;
    *)
        exit 0
        ;;
esac

# 获取当前默认网关
GATEWAY=$(ip route | grep default | awk '{print $3}')

# 检查是否获得到网关
if [ -z "$GATEWAY" ]; then
    log_message "interface $INTERFACE get gateway failed"
else
    log_message "interface $INTERFACE got gateway $GATEWAY"
fi