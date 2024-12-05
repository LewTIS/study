#!/bin/bash

# 定义日志文件
LOG_FILE="/var/log/network_monitor.log"

# 定义错误代码
E_CONN_LOSS=1
E_LINK_FAIL=2
E_WIFI_DISCONNECTED=3
E_NO_ACTIVE_CONNECTION=4



# 获取当前活动连接
get_active_connection() {
    nmcli -t -f TYPE,STATE,DEVICE connection show --active | grep -E 'ethernet|wifi' | awk -F: '{print $1 ":" $3}'
}

# 检查互联网连接
check_internet() {
    echo "Checking internet connection..."
    if ping -c 1 8.8.8.8 &> /dev/null; then
        return 0
    else
        return $E_CONN_LOSS
    fi
}

# 检查本地网络连接
check_local_connection() {
    local ethernet_device=$(nmcli -t -f DEVICE,TYPE connection show --active | grep ethernet | cut -d: -f1 | head -n1)
    if [ -z "$ethernet_device" ]; then
        return $E_LINK_FAIL
    fi
    echo ethernet_device:$ethernet_device
    if [[ $(cat /sys/class/net/$ethernet_device/carrier 2>/dev/null) -ne 1 ]]; then
        return $E_LINK_FAIL
    fi

    return 0
}

# 检查 Wi-Fi 连接状态
check_wifi_connection() {
    local wifi_device=$(nmcli -t -f DEVICE,TYPE connection show --active | grep wifi | cut -d: -f1)
    if [ -z "$wifi_device" ]; then
        return $E_WIFI_DISCONNECTED
    fi

    local wifi_state=$(nmcli -t -f WIFI g)
    if [ "$wifi_state" != "enabled" ]; then
        return $E_WIFI_DISCONNECTED
    fi

    return 0
}

# 记录错误信息
log_error() {
    local error_code=$1
    local error_msg=$2
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Error $error_code: $error_msg" >> $LOG_FILE
}

# 记录网络切换
log_network_switch() {
    local new_connection=$1
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Network switched to: $new_connection" >> $LOG_FILE
}

# 主函数
main() {
    echo "11"
    # 获取当前活动连接
    current_connection=$(get_active_connection)
    echo "Current connection: $current_connection"

    while true; do
        # 获取当前活动连接
        new_connection=$(get_active_connection)
        echo "New connection: $new_connection"

        # 检查是否发生网络切换
        if [[ "$new_connection" != "$current_connection" ]]; then
            echo "Network switched to: $new_connection"
            log_network_switch "$new_connection"
            current_connection="$new_connection"
        fi

        # 如果没有活动连接
        if [ -z "$current_connection" ]; then
            echo "No active network connection"
            log_error $E_NO_ACTIVE_CONNECTION "No active network connection"
        else
            # 根据当前连接类型进行检测
            if echo "$current_connection" | grep -qi "wifi" ; then
                # 检查 Wi-Fi 连接状态
                check_wifi_connection
                wifi_status=$?

                if [[ $wifi_status -ne 0 ]]; then
                    log_error $E_WIFI_DISCONNECTED "Wi-Fi disconnected or disabled"
                else
                    # 检查互联网连接
                    check_internet
                    internet_status=$?

                    if [[ $internet_status -ne 0 ]]; then
                        log_error $E_CONN_LOSS "Wi-Fi connected but cannot reach the internet"
                    else
                        echo "$(date '+%Y-%m-%d %H:%M:%S') - Wi-Fi is connected and internet is accessible" >> $LOG_FILE
                        
                    fi
                fi
            elif echo "$current_connection" | grep -qi "ethernet"; then
                # 检查以太网连接状态
                echo "Checking Ethernet connection..."
                check_local_connection
                ethernet_status=$?
                echo "Ethernet status: $ethernet_status"

                if [[ $ethernet_status -ne 0 ]]; then
                    echo "Ethernet link failure: No physical connection"
                    log_error $E_LINK_FAIL "Ethernet link failure: No physical connection"
                else
                    # 检查互联网连接
                    check_internet
                    internet_status=$?
                    echo "Internet status: $internet_status"

                    if [[ $internet_status -ne 0 ]]; then
                        log_error $E_CONN_LOSS "Ethernet connected but cannot reach the internet"
                    else
                        echo "Ethernet is connected and internet is accessible"
                        echo "$(date '+%Y-%m-%d %H:%M:%S') - Ethernet is connected and internet is accessible" >> $LOG_FILE
                       
                    fi
                fi
            fi
        fi

        # 等待一段时间后再次检测
        sleep 6
    done
}

# 执行主函数
main

