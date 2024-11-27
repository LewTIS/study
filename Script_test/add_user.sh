#!/bin/bash

#编写脚本:提示用户输入用户名和密码,脚本自动创建相应的账户及配置密码。
#如果用户不输入账户名,则提示必须输入账户名并退出脚本;如果用户不输入密码,
#则统一使用默认的 123456 作为默认密码。

#创建用户：sudo useradd -m -p "密码" user  -m创建家目录 -p指定密码 user创建的用户名
default_passwd="123456"
read -p "请输入用户名和密码：" user passwd
if [ -z "$user" ]; then
    echo "必须输入用户名"
    exit 1
else
    if [ -z "$passwd" ];then
        passwd=$default_passwd
    fi
    sudo useradd -m -p $passwd $user
fi




