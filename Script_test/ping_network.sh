#!/bin/bash

#编写脚本测试 192.168.56.0/24 整个网段中哪些主机处于开机状态,哪些主机处于关机状态
#测试192.168.56.100~192.168.56.110
#ping -w 2 192.168.56.101  # -w 设置回复超时时间为2秒

i=100;
while [ $i -le 111 ]
do
    ping -w 1 192.168.56.$i >/dev/null
    if [ $? -eq 0 ];then  #$?是一个特殊变量，它保存了上一个命令的退出状态，成功返回0，失败返回非0
        echo "192.168.56.$i is up"
    else
        echo "192.168.56.$i is down"
    fi
     
    ((i++)) #或let i++ 
done