#!/bin/sh
ETH="$1"
STATE="$2"
MAX_TRIES=3
SLEEP_TIME=3

LOG_PATH="/var/log/ifplugd.log"

start_polling_script() {
    echo "Started gateway ping script for interface $ETH" >> $LOG_PATH
    sudo systemctl start gateway_ping.service
}

stop_polling_script() {
    sudo systemctl stop gateway_ping.service
    echo "Stopped gateway ping script for interface $ETH" >> $LOG_PATH
}

check_network() {
    # 检查IP
    IP=$(ip -4 addr show $ETH | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    if [ -z "$IP" ]; then
        echo "interface $ETH get IP address failed" >>$LOG_PATH
        return 1
    fi
    echo "interface $ETH got IP address $IP" >>$LOG_PATH

    # 检查网关
    GATEWAY=$(ip route | grep default | awk '{print $3}')
    if [ -z "$GATEWAY" ]; then
        echo "interface $ETH get gateway failed" >>$LOG_PATH
        return 1
    fi
    echo "interface $ETH got gateway $GATEWAY" >>$LOG_PATH
    
    return 0
}

if [ "$STATE" = "up" ]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)" | sudo tee -a $LOG_PATH
    
    # 尝试3次检查网络配置
    for i in $(seq 1 $MAX_TRIES); do
        echo "Attempt $i of $MAX_TRIES checking network configuration..." >>$LOG_PATH
        
        if check_network; then
            start_polling_script
            exit 0
        fi
        
        # 如果不是最后一次尝试，则等待后继续
        if [ $i -lt $MAX_TRIES ]; then
            sleep $SLEEP_TIME
        fi
    done
    
    # 如果3次都失败，记录最终失败日志
    echo "Failed to get proper network configuration after $MAX_TRIES attempts" >>$LOG_PATH
    
elif [ "$STATE" = "down" ]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)" | sudo tee -a $LOG_PATH
    stop_polling_script
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Unknown state: $STATE" | sudo tee -a $LOG_PATH
fi