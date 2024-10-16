#!/bin/bash

#统计字母a-z在word.txt中出现的次数

#判断是否有参数

if [ -z $1 ];then
    echo "请输入文件参数"
fi
#判断文件是否存在
if [ ! -e $1 ];then
    echo "文件不存在"
    exit
fi
#1.使用grep寻找文件中所有字母a, grep -io a input.txt 
#   忽略大小写使用-i选项，-o仅输出与正则表达式匹配的部分
#2.通过管道符将grep的输出传递给wc -l ，统计出现次数(grep是每个字符单独匹配输出一行，所有统计行数就是次数)
#3.结合for循环，遍历字母a-z
#4.使用sort根据出现次数排序 sort -rn -k 2 -t - tmp.txt
# -r降序排序 -n按数值排序 -k 2指定排序的键是第二个字段 -t - 指定字段分隔符为':'，若数据空格分隔则不用-t
statistics(){
    for char in {a..z} #循环字母a-z
    do 
        echo "$char:$(grep -io $char $1 | wc -l)" >> tmp.txt  #将结果重定向到input.txt中
    done
    sort -rn -k 2 -t : tmp.txt
    rm -f tmp.txt
}

statistics $1
