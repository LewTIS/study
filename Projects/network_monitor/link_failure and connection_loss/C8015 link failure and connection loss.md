### 1. 概述
偵測以太網接口的連接狀態，包括link failure和connection loss
### 2. 偵測內容
#### 2.1 Link failure 監測
- 定義：ethernet cable是否插入
- 監測原理：通過ifplugd監控carrier狀態
- 監測路徑：/sys/class/net/<interface>/carrier
- 狀態值：0：未連接，1：已連接
#### 2.2 Connection loss 監測
- 定義：網絡層連接狀態
- 監測內容：
    - 1.ip地址分配
    - 2.默認 gateway 分配
    - 3.gateway連通性
- 觸發條件：在Link 狀態為up時進行檢測
### 3. 流程圖
![!\[!\\[!\\\[alt text\\\](<link_failure and connection_loss.png>)\\](<link_failure and connection_loss.png>)\](<link_failure and connection_loss.png>)](总体设计.png)
### 3.實現方案
#### 3.1.配置ifplugd，指定監測的網絡接口以及狀態改變時執行的腳本(詳細配置這裡暫不寫出)
腳本內容如下：包含link failure和connection loss兩種情況
```bahs
#!/bin/sh
ETH="$1"
STATE="$2"

LOG_PATH="/var/log/ifplugd.log"

if [ "$STATE" = "up" ]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)" | sudo tee -a $LOG_PATH
    
elif [ "$STATE" = "down" ]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)" | sudo tee -a $LOG_PATH
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') - Unknown state: $STATE" | sudo tee -a $LOG_PATH
fi
```
#### 3.2 處理邏輯
**1. 腳本接收到ifplugd的狀態改變訊號，並且執行對應的腳本**
**2. Link Failure 檢測：**
```bash
if [ "$STATE" = "up" ]; then
    # 記錄carrier狀態（eth cable 插入）
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)"
    # ... 繼續Connection loss檢測
elif [ "$STATE" = "down" ]; then
    # 記錄carrier狀態（ethernet cable斷開）
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $ETH : carrier = $(cat /sys/class/net/${ETH}/carrier)"
    # 停止网关检测服务
    stop_polling_script
```
#### 3.3 腳本執行流程圖
![!\[!\\[!\\\[alt text\\\](eth_monitor.png)\\](eth_monitor.png)\](eth_monitor.png)](ifplud.png)
#### 3.4 gateway_ping.service

gateway_ping.service負責執行ping gateway的腳本，並檢測連通性，連通性失敗則寫入日誌通知

```bash
#!/bin/bash

INTERFACE=$1
LOG_FILE="/var/log/gateway_ping.log"

log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> "$LOG_FILE"
}

GATEWAY=$(ip route show default | awk '{print $3}')

while true; do
    if ping -c 1 "$GATEWAY" &> /dev/null; then
        log_message "Gateway $GATEWAY is reachable from interface $INTERFACE"
    else
        log_message "Gateway $GATEWAY is NOT reachable from interface $INTERFACE"
    fi
    sleep 60
done
```
#### 3.5 ping gateway腳本執行流程圖
![!\[!\\[alt text\\](ping_gateway.png)\](ping_gateway.png)](pinggateway1.png)
### 4. 測試結果
#### 4.1 ethernet cable插入和拔出
![alt text](<ethernet cable 插拔.png>)
#### 4.2 關閉DHCP Server，ethernet cable插入
![alt text](<IP gateway assign failed (1).png>)
#### 4.3 輪詢ping gateway
![alt text](<ping gateway.png>)






