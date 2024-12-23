#!/bin/bash

# 检查是否为root用户
if [ "$(id -u)" != "0" ]; then
    echo "Error: This script must be run as root" >&2
    exit 1
fi

FIFO="/tmp/usb_pipe"

if [ ! -p "$FIFO" ]; then
    mkfifo "$FIFO"
    chmod 666 "$FIFO"
fi

#INSERTED：表示USB drive插入并挂载
#REMOVED：表示USB drive被拔出
while true; do
    if read message < "$FIFO"; then
        case "$message" in
            "INSERTED")
                echo "USB设备已挂载到 /mnt/usb"
               
                ;;
            "REMOVED")
                echo "USB设备已移除"
               
                ;;
            *)
                echo "未知消息: $message"
                ;;
        esac
    fi
done

