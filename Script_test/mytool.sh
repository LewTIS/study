#!/bin/bash 

#自定义命令行工具，封装常用的shell命令，可通过命令行参数选择不同的功能

# 显示帮助信息
show_help() {
    echo "------------------------------------------------"
    echo "Usage:$0 {list|backup|check|info|search|exit}"
    echo "command:"
    echo "  list     - 列出当前目录下的文件和目录"
    echo "  backup   - 备份指定目录到指定位置"
    echo "  check    - 检查指定文件是否存在"
    echo "  info     - 显示系统信息"
    echo "  search   - 在指定目录中搜索文件"
    echo "  exit     - 退出工具"
    echo "-------------------------------------------------"
}

# 列出当前目录下的文件和目录
list_files() {
    echo "当前目录下的文件和目录："
    ls -l
}

#备份指定目录到指定位置
backup_files() {
    read -p "请输入要备份的目录路径:" source_dir
    read -p "请输入备份文件存储路径:" backup_dir

    if [ ! -d "$source_dir" ];then
        echo "所要备份目录不存在"
        return
    fi

    if [ ! -d "$backup_dir" ];then
        mkdir -p "$backup_dir"
    fi

    #打包要备份的目录到存储路径
    if tar -cvzf "$backup_dir/backup_$(date +%Y%m%d_%H%M%S).tar.gz" -C "$source_dir" .;then
        echo "备份完成：$backup_dir/backup_$(date +%Y%m%d_%H%M%S).tar.gz"
    else
        echo "err:备份失败"
    fi
}

#检查指定文件是否存在
check_file_exist() {
    read -p "请输入文件绝对路径:" file_path
    if [ -e "$file_path" ];then
        echo "文件 $file_path 存在"
    else
        echo "文件 $file_path 不存在"
    fi
}

#显示系统信息
show_info() {
    echo "系统信息:"
    echo "操作系统: $(uname -o)"
    echo "内核版本: $(uname -r)"
    echo "主机名: $(hostname)"
    echo "CPU信息: $(lscpu | grep 'Model name')"
    echo "内存信息: $(free -h | awk 'NR<=2')"
}

#在指定目录搜索文件
search_file() {
    read -p "请输入要搜索的目录：" dir_path
    read -p "请输入要搜索的部分文件名或全名：" file_name

    if [ ! -d "$dir_path" ];then
        echo "err:$dir_path 不存在"
        return
    fi
    echo "搜索结果:"
    find "$dir_path" -type f -name "*$file_name*" 2>/dev/null
}


#主函数
main() {
    while true;do
        
        show_help
        read -p "请输入命令：" command

        case "$command" in
            "help")
                show_help
                ;;
            "list")
                list_files
                ;;
            "backup")
                backup_files
                ;;
            "check")
                check_file_exist
                ;;
            "info")
                show_info
                ;;
            "search")
                search_file
                ;;
            "exit")
                echo "退出工具"
                exit 0
                ;;
            *)
                echo "无效命令,请重试"
                show_help
                ;;
        esac
    done
}
main "$@"


