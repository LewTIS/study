#!/bin/bash
if false;then
#编写进度条脚本，完成一个文件拷贝的进度显示
process_bar()
{
    while :
    do
        echo -n '#'
        sleep 0.0001
    done 
}
process_bar & # $表示放在后台运行
bar_pid=$! # $!保存最近放到后台运行的进程的PID

cp -a $1 $2

kill $bar_pid
echo -e "\nCopy complete!"
fi

if false;then
# 进度条,动态时针版本
# 定义一个显示进度的函数,屏幕快速显示|  / ‐ \
rotate_line(){
    INTERVAL=0.5 #间隔时间
    count=0

    while :
    do
        count=$((count+1)); #每次循环 count+1
        case $count in
            1)
                echo -e '|\b\c'  打印'-' /b表示退格 \c表示不换行
                sleep $INTERVAL
                ;;
            2)
                echo -e '/\b\c'
                sleep $INTERVAL
                ;;
            3)
                echo -e '-\b\c'
                sleep $INTERVAL
                ;;
            4)
                echo -e '\\\b\c'
                sleep $INTERVAL
                ;;
            *) # 当count为其他值时，将count重新赋值为0
                count=0
                ;;  
        esac
    done
}
rotate_line

# 99乘法表
for i in $(seq 9)
do
    for j in $(seq $i)
    do
        echo -n "$j*$i = $((i*j)) "
    done
    echo
done


#批量修改扩展名脚本,如批量将txt文件修改文doc文件
#脚本名 txt doc (将txt的扩展名修改文doc)
# $1第一个参数表示需要修改的扩展名 $2第二个参数表示修改后的扩展名
for i in $(ls *.$1)  #将所有txt文件的文件名保存到i中
do
    mv $i ${i%.*}.$2  # ${i$.*}  表示从尾部开始删除i的第一个.及其后面字符
done



#编写一个点名器脚本

# 该脚本,需要提前准备一个 user.txt 文件
# 该文件中需要包含所有姓名的信息,一行一个姓名,脚本每次随机显示一个姓名
# 打印某行内容：awk "NR==行号" 文件名 sed -n "np" 文件名 
# 统计行数wc -l 文件名
# 随机数生成：$RANDOM

if [ -z $1 ];then
    echo "Usage:$0 filename"
    exit
fi


line=$(cat $1 | wc -l) #统计文件行数
num=$(((RANDOM % line+1))) 

#sed -n "$num p" $1
#awk 'NR==$num' $1
awk -v num=$num '{if(NR==num) print $0}' $1 #-v传递num参数给awk



#100以内正整数加和
sum=0
for i in $(seq 100)
do
    sum=$((sum+i))
done

echo "sum=$sum"



#统计当前 Linux 系统中可以登录计算机的账户有多少个

echo "当前Linux系统中可以登录计算机的账户有:"
#sudo cat /etc/passwd | wc -l
awk -F : '{print $1}' /etc/passwd | wc -l

fi #(对应前面的if false)

#统计/var/log 有多少个文件,并显示这些文件名
echo "/var/log 共有文件："
num=$(sudo find /var/log -type f | wc -l)
echo $num







