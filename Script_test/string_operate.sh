#!/bin/bash
#变量 string="Bigdata process framework is Hadoop,Hadoop is an open source project" 
#执行脚本后，打印输出 string 字符串变量，并给用户以下选项：

#(1)、打印 string 长度
#(2)、删除字符串中所有的 Hadoop 
#(3)、替换第一个 Hadoop 为 Mapreduce 
#(4)、替换全部 Hadoop 为 Mapreduce 

#用户输入数字 1|2|3|4 ，可以执行对应项的功能；输入 q|Q 则退出交互模式。

string="Bigdata process framework is Hadoop,Hadoop is an open source project"
len_string(){
    echo ${#string}
}
# 将所有Hadoop替换为空，相当于删除
delete_Hadoop(){
    echo ${string//Hadoop/}
}

replace_first_Hadoop(){
    echo ${string/Hadoop/Mapreduce}
}

replace_all_Hadoop(){
    echo ${string//Hadoop/Mapreduce}
}

print_tips(){
    echo "(1):打印字符串长度"
    echo "(2):删除字符串中所有的 Hadoop"
    echo "(3):替换第一个 Hadoop为 Mapreduce"
    echo "(4):替换全部Hadoop为 Mapreduce"
    echo "(q|Q):退出程序"
}
#用户输入数字 1|2|3|4，可以执行对应项的功能；输入 q|Q 则退出交互模式。
while true
do
    echo "$string" #打印字符串
    print_tips
    read -p "请输入1|2|3|4:" num
    case $num in
        1)
            len_string
         ;;
        2)
            delete_Hadoop
         ;;
        3)
            replace_first_Hadoop
         ;;
        4)
            replace_all_Hadoop
         ;;
        q|Q)
            exit
         ;;
        *)
            echo "输入有误!"
    esac
done
        
