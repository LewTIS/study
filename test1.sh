#!/bin/bash

#!/bin/bash

# 错误码
ERR_NO_INTERFACE=3       # 未找到默认网卡接口
ERR_NO_GATEWAY=4      # 未找到默认网关
ERR_NO_CARRIER=5        # 物理连接未连接
ERR_NO_IP=6            # 未配置IP地址
ERR_NO_LAN=7           # 无法访问本地网络
ERR_NO_INTERNET=8      # 无法访问互联网
ERR_DNS_FAILED=9       # DNS解析失败
# 2、3、4、5、6 -> ERR_LOCAL_NETWORK=0 
# 7、8 -> ERR_INTERNET=1

# 错误码(通知)
ERR_LOCAL_NETWORK=1 # 无法连接本地网络
ERR_INTERNET=2 # 无法连接互联网
LOG_FILE="/tmp/network.log"

log_error(){
    local error_code=$1
    local error_msg=$2
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Error $error_code: $error_msg" >> $LOG_FILE
}

check_local_network(){
    if ! systemctl is-active NetworkManager >/dev/null 2>&1; then
        echo "NetworkManager 服务未运行"
        echo "启动服务: sudo systemctl start NetworkManager"
        exit 1
    fi

    # 1. 检查所有网卡接口状态
    echo "1. 检查网卡接口状态："
    echo "----------------------"
    # 获取所有网络接口（除了lo）
    interfaces=$(nmcli device | grep -v "lo" | awk 'NR>1 {print $1,$2,$3}')
    echo "设备名称  类型  状态"
    echo "$interfaces"
    echo "----------------------"

    # 2. 获取默认网卡和网关
    DEFAULT_INTERFACE=$(ip route | grep default | awk '{print $5}')
    if [ -z "$DEFAULT_INTERFACE" ]; then
        echo "✗ 未找到默认网卡接口"
        log_error $ERR_LOCAL_NETWORK "本地网络未连接，未找到默认网卡接口"
        return $ERR_LOCAL_NETWORK
    fi

    GATEWAY_IP=$(nmcli device show $DEFAULT_INTERFACE | grep IP4.GATEWAY | awk '{print $2}')
    if [ -z "$GATEWAY_IP" ]; then
        echo "✗ 未找到默认网关"
        log_error $ERR_LOCAL_NETWORK "本地网络未连接，未找到默认网关"
        return $ERR_LOCAL_NETWORK
    fi

    echo "2. 默认网络配置："
    echo "默认网卡: $DEFAULT_INTERFACE"
    echo "默认网关: $GATEWAY_IP"
    echo "----------------------"

    # 3. 检查物理连接
    echo "3. 检查物理连接："
    CARRIER_FILE="/sys/class/net/$DEFAULT_INTERFACE/carrier"
    if [ -f "$CARRIER_FILE" ]; then
        carrier_status=$(cat "$CARRIER_FILE")
        if [ "$carrier_status" -eq 1 ]; then
            echo "✓ 网线已连接"
        else
            echo "✗ 网线未连接"
            echo "  请检查网线连接是否正常"
            log_error $ERR_LOCAL_NETWORK "本地网络未连接，检查网线连接"
            return $ERR_LOCAL_NETWORK
        fi
    fi
    # 4. 检查 IP 地址配置
    echo "4. 检查IP配置："
    ip_info=$(nmcli device show $DEFAULT_INTERFACE | grep IP4.ADDRESS | awk '{print $2}')
    if [ -n "$ip_info" ]; then
        echo "✓ IP地址已配置: $ip_info"
    else
        echo "✗ 未配置IP地址"
        log_error $ERR_LOCAL_NETWORK "本地网络未连接，未配置IP地址"
        return $ERR_LOCAL_NETWORK
    fi
    # 5. 测试局域网连通性
    echo "正在测试网关连通性..."
    if ping -c 4 -W 2 $GATEWAY_IP &> /dev/null; then
        echo "✓ 本地网络可访问"
    else
        log_error $ERR_LOCAL_NETWORK "本地网络未连接，测试网关连通性失败"
        return $ERR_LOCAL_NETWORK
    fi

    return 0;
}
check_internet(){
    echo "正在测试互联网连接..."
    if ping -c 4 -W 2 8.8.8.8 &> /dev/null; then
        echo "✓ 互联网可访问"
    else
        echo "✗ 无法访问互联网"
        log_error $ERR_INTERNET "无法访问互联网"
        return $ERR_INTERNET
    fi

    if ping -c 2 -W 2 www.baidu.com &> /dev/null; then
        echo "✓ DNS解析正常"
    else
        echo "✗ DNS解析失败"
        log_error $ERR_INTERNET "DNS解析失败"
        return $ERR_INTERNET
    fi
    
    return 0
}

#主流程
echo "=== 网络连接检查 ==="
check_local_network
local_status=$?


if [ $local_status -eq 0 ]; then
    check_internet
    internet_status=$?
    
    if [ $internet_status -eq 0 ]; then
        echo "✓ 网络检查完成: 本地网络和互联网连接正常"
        exit 0
    else
        echo "✗ 网络检查完成: 无法连接到互联网"
        exit $ERR_INTERNET
    fi
else
   echo "✗ 网络检查完成: 无法连接到本地网络"
    exit $ERR_LOCAL_NETWORK
fi

