#!/bin/bash
#给定一个目录路径参数，统计指定目录下各类型文件的数量（如 .txt、.log、.sh 等）

#检查参数
if [ "$#" -ne 1 ];then
    echo "Usage:$0 <dir_path>"
    exit 1
fi

directory="$1"
#检查目录是否存在
if [ ! -d "$directory" ];then
    echo "$directory not exist"
    exit 1
fi

file_types=("txt" "log" "sh" "c" "h" "awk" "ko" "cmd")
for type in ${file_types[@]}
do
    count=$(find "$directory" -name "*.$type" | wc -l)
    echo "numbers of .$type file : $count"
done

