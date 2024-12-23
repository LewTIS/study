## 1.wifi监控脚本：
```c
#!/bin/bash
# /usr/local/bin/wifi_monitor.sh

# 确保日志目录存在
mkdir -p /var/log/wifi-monitor

log_event() {
    echo "$(date '+%Y-%m-%d %H:%M:%S'): $1" >> /var/log/wifi-monitor/wifi.log
}

case "$2" in
    "CTRL-EVENT-CONNECTED")
        log_event "WiFi connected - Interface: $1"
        # 启动IP检查脚本，传入接口名称
        /usr/local/bin/check_ip.sh "$1" &
        ;;
    "CTRL-EVENT-DISCONNECTED")
        log_event "WiFi disconnected - Interface: $1"
        ;;
esac
```

## 2. ip、gateway以及gateway连通性检查脚本：
```c
#!/bin/bash
# /usr/local/bin/check_ip.sh

INTERFACE=$1
MAX_TRIES=5
SLEEP_TIME=2

log_event() {
    echo "$(date '+%Y-%m-%d %H:%M:%S'): $1" >> /var/log/wifi-monitor/wifi.log
}

check_network() {
    # 检查IP地址
    IP=$(ip addr show dev $INTERFACE | grep "inet " | awk '{print $2}' | cut -d/ -f1)
    if [ -z "$IP" ]; then
        log_event "No IP address assigned to $INTERFACE"
        return 1
    fi
    log_event "IP address assigned: $IP"

    # 检查默认网关
    GATEWAY=$(ip route | grep default | grep $INTERFACE | awk '{print $3}')
    if [ -z "$GATEWAY" ]; then
        log_event "No gateway assigned for $INTERFACE"
        return 1
    fi
    log_event "Gateway assigned: $GATEWAY"

    # 测试网关连通性
    if ! ping -c 1 -W 1 $GATEWAY >/dev/null 2>&1; then
        log_event "Gateway $GATEWAY is not reachable"
        return 1
    fi
    log_event "Gateway $GATEWAY is reachable"

    return 0
}

# 主循环
for i in $(seq 1 $MAX_TRIES); do
    log_event "Attempt $i of $MAX_TRIES checking network configuration..."
    
    if check_network; then
        log_event "Network configuration successful on $INTERFACE"
        exit 0
    fi
    
    # 如果不是最后一次尝试，则等待后继续
    if [ $i -lt $MAX_TRIES ]; then
        sleep $SLEEP_TIME
    fi
done

log_event "Failed to get proper network configuration after $MAX_TRIES attempts"
exit 1
```
## 3.集成系统服务systemd
```c
# /etc/systemd/system/wifi-monitor.service

[Unit]
Description=WiFi Connection Monitor
After=network.target wpa_supplicant.service

[Service]
ExecStart=/usr/sbin/wpa_cli -i wlan0 -a /usr/local/bin/wifi_monitor.sh
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

## 4.部署
### 设置执行权限
- chmod +x /usr/local/bin/wifi_monitor.sh
- chmod +x /usr/local/bin/check_ip.sh

### 创建日志目录
- mkdir -p /var/log/wifi-monitor
- chmod 755 /var/log/wifi-monitor

### 启用并启动服务
- systemctl daemon-reload
- systemctl enable wifi-monitor
- systemctl start wifi-monitor