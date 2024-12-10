#!/bin/bash

# 配置
INTERNET_TARGETS=(
    "8.8.8.8"        # Google DNS
    "1.1.1.1"        # Cloudflare DNS
)

# 动态获取本地网关
get_gateway() {
    ip route | grep default | awk '{print $3}'
}

# 检查网络连接
check_connectivity() {
    local targets=("$@")
    for target in "${targets[@]}"; do
        if ! ping -c 1 -W 2 "$target" >/dev/null 2>&1; then
            return 1
        fi
    done
    return 0
}

# 记录日志并发送通知
notify_issue() {
    local message=$1
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $message" >> "/tmp/internet-connectivity.log"
    logger -t internet-monitor "$message"
    if [ -n "$DISPLAY" ]; then
        notify-send "Internet Status" "$message" -u normal -i network-wired
    fi
}

# 主检查逻辑
main() {
    while true; do
        local gateway=$(get_gateway)
        if [ -z "$gateway" ] || ! check_connectivity "$gateway"; then
            notify_issue "Local network issue detected"
        elif ! check_connectivity "${INTERNET_TARGETS[@]}"; then
            notify_issue "Internet connectivity issue detected"
        fi
        sleep 60  # 每分钟检查一次
    done
}

# 启动检查
main