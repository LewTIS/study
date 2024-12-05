#!/bin/bash

echo "=== 网络连接检查 ==="

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
    exit 1
fi

GATEWAY_IP=$(nmcli device show $DEFAULT_INTERFACE | grep IP4.GATEWAY | awk '{print $2}')
if [ -z "$GATEWAY_IP" ]; then
    echo "✗ 未找到默认网关"
    exit 1
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

        # 4. 检查 IP 地址配置
        echo "4. 检查IP配置："
        ip_info=$(nmcli device show $DEFAULT_INTERFACE | grep IP4.ADDRESS | awk '{print $2}')
        if [ -n "$ip_info" ]; then
            echo "✓ IP地址已配置: $ip_info"

            # 5. 测试局域网连通性
            echo "5. 测试网络连通性："
            echo "正在测试网关连通性..."
            if ping -c 4 -W 2 $GATEWAY_IP &> /dev/null; then
                echo "✓ 本地网络可访问"
                
                # 6. 测试互联网连接
                echo "正在测试互联网连接..."
                if ping -c 4 -W 2 8.8.8.8 &> /dev/null; then
                    echo "✓ 互联网可访问"
                    
                    # 测试DNS
                    echo "测试DNS解析..."
                    if ping -c 2 -W 2 www.baidu.com &> /dev/null; then
                        echo "✓ DNS解析正常"
                    else
                        echo "✗ DNS解析失败"
                        echo "DNS服务器配置："
                        nmcli device show $DEFAULT_INTERFACE | grep IP4.DNS
                    fi
                else
                    echo "✗ 无法访问互联网"
                    echo "  可能的原因:"
                    echo "  - 网关配置错误"
                    echo "  - DNS配置问题"
                fi
            else
                echo "✗ 无法访问本地网络"
                echo "  可能的原因:"
                echo "  - 网关IP可能不正确"
                echo "  - 路由器/交换机可能故障"
            fi
        else
            echo "✗ 未配置IP地址"
            echo "  可能的原因:"
            echo "  - DHCP服务未运行"
            echo "  - 未配置静态IP"
        fi
    else
        echo "✗ 网线未连接"
        echo "  请检查网线连接是否正常"
    fi
else
    echo "✗ 接口 $DEFAULT_INTERFACE 不存在"
    echo "可用的网络接口:"
    nmcli device status
fi
