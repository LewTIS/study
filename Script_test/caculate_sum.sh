#!/bin/bash

#提示用户输入一个正整数 num ，然后计算从 1+2+3 加到给定正整数。 
#必须对给定的 num 值进行是否为正整数判断，如果不是正整数则重新输入。



while true
do
    read -p "请输入一个正整数：" num
    if [ $num -lt 0 ];then
        echo "输入有误!"
        continue;
    else
        sum=0;
        for((i=1;i<=$num;++i))
        do
            sum=$(expr $sum + $i)
        done
        echo "The sum is:$sum"
        exit    
    fi 
done