#!/bin/bash

FIFO="/tmp/usb_pipe"
MOUNT_POINT="/mnt/usb"

if [ ! -d "$MOUNT_POINT" ]; then
    mkdir -p "$MOUNT_POINT"
    chmod 777 "$MOUNT_POINT"
fi

if [ ! -p "$FIFO" ]; then
    mkfifo "$FIFO"
    chmod 666 "$FIFO"
fi

# 记录日志
echo "$(date): $1 $2" > /tmp/usb


case "$1" in
    "add")
        DEVICE="/dev/$2"

        echo "$(date): Attempting to mount $DEVICE" >> /tmp/usb

        systemctl stop mnt-usb.mount 2>/dev/null
        systemctl reset-failed mnt-usb.mount 2>/dev/null

        # 使用 systemd-mount
        /usr/bin/systemd-mount --no-block --collect "$DEVICE" "$MOUNT_POINT" >> /tmp/usb 2>&1

        # 等待并检查挂载结果
        if [ $? -eq 0 ];then
            echo "mount successful" >> /tmp/usb
            echo "USB_MOUNTED:$DEVICE" > "$FIFO"
        else
            echo "USB_MOUNT_FAILED:$DEVICE" > "$FIFO"
        fi
        ;;
    "remove")
        DEVICE="/dev/$2"
        echo "$(date): Removing device $DEVICE" >> /tmp/usb

        /usr/bin/systemd-mount --umount "$MOUNT_POINT" >> /tmp/usb 2>&1
        echo "USB_REMOVED:$DEVICE" > "$FIFO"
        ;;
esac

