#!/bin/bash


#getopts:用于在脚本内部解析脚本命令行参数，通常与while循环一起使用
#       取出脚本所有带有前置连词线(-)的参数
# 例：./sh_06.sh -h val1 -i val2  参数h和i分别携带一个参数值
while getopts "h:i:" opt  #h:i:  后面加冒号表示这个参数带有参数值
do
    case $opt in
        h)
            echo "h value is $OPTARG" # 若参数有参数值，$OPTARG保存的就是参数值。
            ;;
        i)
            echo "i value is $OPTARG"
            ;;
        ?)  #处理输入不在判断之内的参数
            echo "Usage:$0 -h par1 -i par2"
            exit
            ;;
        :) # 对于应有参数值的参数，若后面没跟参数值，getopts将 opt 设为':' 将OPTARG 设为空 #该选项可省略，除非自定义错误信息
            echo "Option -$OPTARG requires an argument." 
            exit
            ;;
    esac
done
shift $(($OPTIND - 1))  #shift $((OPTIND -1)) 命令用于移除所有已经被 getopts 处理的选项和参数
echo $1 # 剩下getopts未处理的参数 $1 $2也改变

