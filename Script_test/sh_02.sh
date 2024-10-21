#!/bin/bash

# 定义一个函数处理文件内容的读取和输出
function read_and_printf_file {
    local input_file=$1 #定义一个局部变量，将函数的第一个参数($1)赋值给它
    if [ ! -f "$input_file" ]; then #检查文件是否存在。-f选项用于检查文件是否存在且是普通文件
        echo "Error:input file does not exist"
        exit 1 #退出，状态码为1，表示运行出错
    fi
    #逐行读取并打印
    #使用read命令从标准输入逐行读取文件内容，这里将文件重定向为read的输入，将读到的一行内容给变量line
    # IFS=' ',IFS 设置为空字符串。这样做的目的是告诉 read 命令不要使用任何字符作为字段分隔符，
    #-r 选项防止对反斜杠进行转义处理
    # 即整行输入被视为一个单一的字段，不进行分割。这样，read 命令将整行内容作为一个整体赋值给变量 line。
    while IFS= read -r line  #这里也可以不要IFS 即while read -r line
    do
        echo "$line"
    done < "$input_file" #将该文件重定向为read命令的输入
}

input_file=""
#"i:"告诉getopts处理的选项是-i，并且-i后面需要一个参数,为冒号后的内容
while getopts "i:" opt #getopts命令用于解析命令行选项
do 
    case $opt in #使用case语句根据$opt(当前处理的选项)的值执行不同的操作
        i) 
            input_file=$OPTARG    #如果$opt的值是i，则执行下面的操作，将$OPTARG(包含了-i选项后面的参数，即文件路径)赋值给input_file
            ;;
        \?) 
        # 当getopts遇到它不认识的选项时(如 -j)，它会设置opt为'?'' 
            echo "$0:illegal option " >&2
            echo "Uage:$0 -i input_file" >&2
            exit 1
            ;;
        :)
        # 当getopts遇到一个需要参数但没有提供参数的选项时(如 -i），它会设置opt为':'
        # 这通常是因为用户只写了 -i 而没有指定输入文件的路径
            echo "$0:option -$opt requires an argument" >&2
            echo "Uage:$0 -i input_file" >&2
            exit 1
            ;;
    esac
done
# -z 判断字符串是否为空，即input_file是否为空
if [ -z "$input_file" ]; then  
    echo "Usage: $0: [-i input file]"  
    exit 1  
fi  
#调用函数处理文件，并传递给他一个参数
read_and_printf_file "$input_file"