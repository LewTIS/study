#!/bin/bash
FIFO="/tmp/usb_pipe"

# 检查是否为root用户
if [ "$(id -u)" != "0" ]; then
    echo "Error: This script must be run as root" >&2
    exit 1
fi


# 确保FIFO存在
if [ ! -p "$FIFO" ]; then
    mkfifo "$FIFO"
    chmod 666 "$FIFO"
fi

#USB_INSERTED：表示USB drive插入并挂载
#USB_REMOVED：表示USB drive被拔出
# 根据参数决定发送的消息
case "$1" in
    add)
        message="USB_INSERTED"
        ;;
    remove)
        message="USB_REMOVED"
        ;;
    *)
        echo "Unknown action: $1"
        exit 1
        ;;
esac
# 发送消息到FIFO
echo $message > "$FIFO"