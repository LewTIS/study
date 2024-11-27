#!/bin/bash


# 检查是否为root用户
if [ "$(id -u)" != "0" ]; then
    echo "Error: This script must be run as root" >&2
    exit 1
fi

MOUNT_POINT="/mnt/usb"
LOG_FILE="/tmp/usb_monitor.log"
FIFO_FILE="/tmp/usb_fifo"

# 创建并设置日志文件权限
touch "$LOG_FILE"
chmod 666 "$LOG_FILE"

#创建命名管道
if [ ! -p "$FIFO_FILE" ]; then
    mkfifo "$FIFO_FILE"
    chmod 666 "$FIFO_FILE"
fi

# 确保挂载点存在
if [ ! -d "$MOUNT_POINT" ]; then
    mkdir -p "$MOUNT_POINT"
    sudo chmod 777 "$MOUNT_POINT"
fi

# 日志函数
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S'): $1" >> "$LOG_FILE"
}

log "USB监控服务启动"

# 监控 /dev 目录的变化
inotifywait -m -e create,delete /dev |
while read path action file; do
    # 检查是否是USB存储设备
 if  echo "$file" | grep -q '^sd[a-z][0-9]$'; then
        device="/dev/$file"
        log "检测到设备变化: $action $device"

        if [ "$action" = "CREATE" ]; then
            # 等待设备就绪
            sleep 1
            # 尝试挂载设备
            if mountpoint -q "$MOUNT_POINT"; then
                 continue
            fi
            mount "$device" "$MOUNT_POINT" 2>> "$LOG_FILE"

            if [ $? -eq 0 ]; then
                log "成功挂载 $device 到 $MOUNT_POINT"
                # 通知middleware
                echo "USB_INSERTED:$device" > "$FIFO_FILE"
            else
                log "挂载 $device 失败"
            fi
        elif [ "$action" = "DELETE" ]; then
            # 处理设备移除
            if mountpoint -q "$MOUNT_POINT"; then
                umount "$MOUNT_POINT" 2>> "$LOG_FILE"
                echo "USB_REMOVED:$device" > "$FIFO_FILE"
                log "设备已移除并卸载"
            fi
        fi
    fi
done

