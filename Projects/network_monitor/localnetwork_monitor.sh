#!/bin/bash

echo "=== 网络连接检查 ==="
echo

# 1. 检查所有网卡接口状态
echo "1. 检查网卡接口状态："
echo "----------------------"
interfaces=$(ip link show | grep -v "lo:" | grep "^[0-9]" | cut -d: -f2 | awk '{print $1}')
for interface in $interfaces; do
    echo "检查接口: $interface"
    if ip link show $interface | grep -q "state UP"; then
        echo "✓ 接口状态: 已启用 (UP)"
    else
        echo "✗ 接口状态: 已禁用 (DOWN)"
        echo "  建议: 尝试启用接口:"
        echo "  sudo ip link set $interface up"
    fi
    echo "----------------"
done

# 2. 获取默认网卡和网关
DEFAULT_INTERFACE=$(ip route | grep default | awk '{print $5}')
if [ -z "$DEFAULT_INTERFACE" ]; then
    echo "✗ 未找到默认网卡接口"
    exit 1
fi

GATEWAY_IP=$(ip route | grep default | awk '{print $3}')
if [ -z "$GATEWAY_IP" ]; then
    GATEWAY_IP="10.0.2.2"  # 默认网关
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
        ip addr show $DEFAULT_INTERFACE | grep "inet " &> /dev/null
        if [ $? -eq 0 ]; then
            IP_ADDR=$(ip addr show $DEFAULT_INTERFACE | grep "inet " | awk '{print $2}' | cut -d/ -f1)
            echo "✓ IP地址已配置: $IP_ADDR"

            # 5. 测试局域网连通性
            echo "5. 测试网络连通性："
            echo "正在测试网关连通性..."
            ping -c 4 -W 2 $GATEWAY_IP &> /dev/null
            if [ $? -eq 0 ]; then
                echo "✓ 本地网络可访问"
                
                # 6. 测试互联网连接
                echo "正在测试互联网连接..."
                ping -c 4 -W 2 8.8.8.8 &> /dev/null
                if [ $? -eq 0 ]; then
                    echo "✓ 互联网可访问"
                else
                    echo "✗ 无法访问互联网"
                    echo "  可能的原因:"
                    echo "  - 网关配置错误"
                    echo "  - DNS配置问题"
                    echo "  - 防火墙限制"
                fi
            else
                echo "✗ 无法访问本地网络"
                echo "  可能的原因:"
                echo "  - 网关IP可能不正确"
                echo "  - 路由器/交换机可能故障"
                echo "  - 防火墙可能阻止了ICMP"
            fi
        else
            echo "✗ 未配置IP地址"
            echo "  可能的原因:"
            echo "  - DHCP服务未运行"
            echo "  - 未配置静态IP"
            echo "  建议: 尝试执行 'sudo dhclient $DEFAULT_INTERFACE' 获取IP地址"
        fi
    else
        echo "✗ 网线未连接"
        echo "  请检查网线连接是否正常"
    fi
else
    echo "✗ 接口 $DEFAULT_INTERFACE 不存在"
    echo "可用的网络接口:"
    ip link show | grep -E '^[0-9]+:' | cut -d: -f2
fi
