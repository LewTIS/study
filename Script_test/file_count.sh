#!/bin/bash 
#文件计数器，接收一个目录路径参数，并输出该目录下的文件数量和子目录数量

#检测参数

if [  "$#" -ne 1 ];then
    echo "Usage:$0 <directory_path>"
    exit 1
fi

#检查目录是否存在
directory="$1"
if [ ! -d "$directory" ];then
    echo "$directory not exist"
    exit 1
fi

#使用find在该目录下查找，设定查找深度为1，即在当前目录查找，不递归到下一层目录
#包含隐藏文件和目录 要去除，可在find中添加  ! -name ".*"
file_count=$(find "$directory" -maxdepth 1 -type f | wc -l)
directory_count=$(find "$directory" -maxdepth 1 -type d | wc -l) #包含了当前目录

echo "file_count=$file_count"
echo "directory_count=$((directory_count-1))" #减去当前目录

