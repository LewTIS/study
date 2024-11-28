#!/bin/bash

SOURCE_DIR="/home/linux/share/CODE"
TARGET_DIR="/home/linux/study"
LOG_FILE="$HOME/sync.log"
#添加时间戳
echo "-------------------------------------------" >> "$LOG_FILE"
echo "$(date '+%Y-%m-%d %H:%M:%S') - 开始同步" >> "$LOG_FILE"

# 确保目标目录存在
if [ ! -d "$TARGET_DIR" ]; then
    echo "目标目录不存在: $TARGET_DIR" >> "$LOG_FILE"
    exit 1
fi

#同步文件
rsync -av --delete --exclude='.git/' "$SOURCE_DIR/" "$TARGET_DIR" >> "$LOG_FILE" 2>&1

#git 操作
cd "$TARGET_DIR" || {
    echo "无法切换到目标目录: $TARGET_DIR" >> "$LOG_FILE"
    exit 1
}

# 确认是否在 git 仓库中
if [ ! -d ".git" ]; then
    echo "错误：$TARGET_DIR 不是一个 git 仓库" >> "$LOG_FILE"
    exit 1
fi

git add . >>"$LOG_FILE" 2>&1
git commit -m "Daily update: $(date '+%Y-%m-%d %H:%M')" >> "$LOG_FILE" 2>&1
git push origin main >> "$LOG_FILE" 2>&1

echo "$(date '+%Y-%m-%d %H:%M:%S') - 同步完成" >> "$LOG_FILE"

