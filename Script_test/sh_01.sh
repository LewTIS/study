#!/bin/bash  
  
# 定义一个函数来处理文件内容的读取和输出  
function read_and_print_file {  
    local input_file=$1  
    if [ ! -e "$input_file" ]; then  
        echo "Error: File '$input_file' does not exist."  
        exit 1  
    fi  
  
    # 逐行读取文件并打印  
    while IFS= read -r line  
    do  
        echo "$line"  
    done < "$input_file"  
}  
  
# 初始化变量  
input_file=""  
  
# 使用 getopts 来解析命令行选项  
while getopts "i:" opt; do  
    case $opt in  
        i)  
            input_file=$OPTARG  
            ;;  
        \?)  
            echo "Invalid option: -$OPTARG" >&2  
            echo "Usage: $0: [-i input file]" >&2  
            exit 1  
            ;;  
        :)  
            echo "Option -$OPTARG requires an argument." >&2  
            echo "Usage: $0: [-i input file]" >&2  
            exit 1  
            ;;  
    esac  
done  
  
# 检查是否指定了输入文件  
if [ -z "$input_file" ]; then  
    echo "Usage: $0: [-i input file]"  
    exit 1  
fi  
  
# 调用函数处理文件  
read_and_print_file "$input_file"