#!/bin/bash

# 提取dmesg日志中的错误信息
# 1.过滤出包含关键字"ERROR","FAILED","FATAL"的项目
# 2.将这些错误保存到一个名为dmesg_errors.txt的文件中
# 3.统计出日志中错误条目的总数，并将结果附加到dmesg_errors.txt文件的末尾

# sudo dmesg | grep -Ei "ERROR|FAILED|FATAL" > dmesg_errors.txt 

# echo "number of error:$(sudo dmesg | grep -i "ERROR"  | wc -l)" >>dmesg_errors.txt
statistics(){
    array=('ERROR' 'FAILED' 'FATAL')
    for char in ${array[@]}
    do
        sudo dmesg|grep -i $char >> dmesg_errors.txt
    done
        echo "The number of ERROR:$(sudo dmesg|grep -i "ERROR"|wc -l)">>dmesg_errors.txt
}
statistics

