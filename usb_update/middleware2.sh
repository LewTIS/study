#!/bin/bash

# 检查是否为root用户
if [ "$(id -u)" != "0" ]; then
    echo "Error: This script must be run as root" >&2
    exit 1
fi

FIFO_FILE="/tmp/usb_fifo"

#创建命名管道
if [ ! -p "$FIFO_FILE" ]; then
    mkfifo "$FIFO_FILE"
    chmod 666 "$FIFO_FILE"
fi

#消息格式： USB_INSERTED:/dev/sdc
while true;do
    if read message < "$FIFO_FILE"; then
    
        status="${message%%:*}"
	    device="${message#*:}"

	case "$status" in
	    "USB_INSERTED")
            echo "USB设备 $device 已挂载"
		;;
	    "USB_REMOVED")
			echo "USB设备 $device 已移除"
		;;
	esac
    fi
done
