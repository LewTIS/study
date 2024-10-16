#!/bin/bash
#监控内存和磁盘容量，小于给定值时报警(给root用户发送邮件报警)
# 实时监控本机内存和硬盘剩余空间,剩余内存小于500M、根分区剩余空间小于1000M时,发送报警邮件给root管理员

# 查看根分区空间使用情况：df /  (df即disk free)  单位为kb
# 查看内存使用情况 free
if false;then
while true
do
    #提取根分区剩余空间  //是awk正则表达式分隔符 这里连续两个//，\对/转义 \/是表示\
    #df /输出结果匹配根分区的行的第四个字段是剩余空间 所有print $4
    disk_size=$(df / | awk '/\//{print $4}')

    #提取内存剩余空间
    mem_size=$(free | awk '/Mem/{print $4}')

    if [ $disk_size -lt 512000 ]||[ $mem_size -lt 1024000 ]
    then
        # -s后面跟邮件主题 root表示发送给root <<EOF表示将后面的输入作为邮件内容直到遇到EOF
        # 使用mail命令查看新邮件
        mail -s "Warning" root <<EOF  
    Insufficient resources,资源不足
EOF
fi
    sleep 6 #每6秒检查一次
done

fi
