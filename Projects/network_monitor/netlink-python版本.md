## 1.程序概述
    使用 Python 的 pyroute2 庫通過 Netlink 協議實現對網絡接口的物理連接狀態監控。
## 2.功能特點
- 監控指定網絡接口的物理連接狀態。
- 支持 ethernet 和 wifi 接口同時監控
- 實時檢測連接/斷開事件
- 記錄狀態變化到日誌文件
## 3.工作原理
### 3.1 Netlink 通信機制
- Netlink 是 Linux特有的進程間通信機制
- 基於套接字接口，支持異步通信
- 支持多播方式發送信息
- 用於內核與用戶空間的雙向通信
### 3.2 狀態監控機制
```python

# 創建 Netlink 套接字
self.ip = IPRoute()

# 訂閱網絡接口事件
self.ip.bind(groups=RTMGRP_LINK)

# 監聽事件
for msg in self.ip.poll():
    if msg['event'] == 'RTM_NEWLINK':
        # 處理狀態變化

```
### 3.3 連接狀態判斷
- ethernet 接口：
    - carrier=1:網線已插入
    - carrier=0:網線已拔出
- WiFi 接口：
    - carrier=1:已和WiFi AP 連接
    - carrier=0:未和WiFi AP 連接
## 4. 詳細設計
### 4.1 類結構
```python

class NetworkMonitor:
    def __init__(self):
        """初始化監控器
        - 創建 IPRoute 對象
        - 設置監控接口列表
        - 初始化接口狀態字典(記錄接口上一次狀態，防止重複通知)
        """
    def get_carrier_state(self, ifname):
        """獲取接口當前狀態(在程序初始運行時執行一次，獲取到初始狀態)
        通過讀取 /sys/class/net/<ifname>/carrier 文件獲取接口當前狀態
        """
    def init_interface_states(self): 
        """初始化所有監控接口的狀態"""

    def run(self):
        """開始監控
        - 訂閱RTMGRP_LINK事件
        - 處理狀態變化(比對當前狀態與上一次狀態，有變化則通知)
        - 記錄日誌
        """
    def cleanup(self):
        """清理資源
        - 關閉 Netlink 套接字
        """

```
### 4.2 事件處理流程
1. 接收 Netlink 消息（RTMGRP_LINK類型）
2. 過濾 RTM_NEWLINK 事件
3. 檢查接口名稱是否在監控列表中
4. 獲取 carrier 狀態
5. 比較當前狀態與上一次狀態
6. 更新狀態記錄
7. 記錄日誌
## 5. 程序實現
```python

#!/usr/bin/env python3

from pyroute2 import IPRoute
from pyroute2.netlink.rtnl import RTMGRP_LINK
import logging

# 配置日志
LOG_FILE = "/tmp/py.log"
logging.basicConfig(
    filename=LOG_FILE,
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

class NetworkMonitor:
    def __init__(self):
        self.ip = IPRoute() # 创建IPRoute对象，创建套接字与内核通信
        self.monitored_interfaces = ['wlan0', 'enp3s0'] # 监控的接口列表
        self.last_carrier = {} # 存储每个接口的上一次状态
        self.init_interface_states() # 初始化接口状态
    # 获取接口的carrier状态
    def get_carrier_state(self, ifname):
        try:
            with open(f"/sys/class/net/{ifname}/carrier") as f:
                return int(f.read().strip())
        except:
            return 0
    # 初始化接口状态 
    def init_interface_states(self): 
        for ifname in self.monitored_interfaces:
            carrier = self.get_carrier_state(ifname)
            self.last_carrier[ifname] = carrier

    def run(self):
        try:
            logging.info("Network monitoring started")
            self.ip.bind(groups=RTMGRP_LINK)
            
            while True:
                for msg in self.ip.poll(): # 等待事件
                    if msg['event'] == 'RTM_NEWLINK': # 接口状态变化事件
                        ifname = msg.get_attr('IFLA_IFNAME')
                        if ifname in self.monitored_interfaces:
                            carrier = msg.get_attr('IFLA_CARRIER') # 获取carrier状态
                            if carrier is not None:
                                if self.last_carrier.get(ifname) != carrier:
                                    status = "connected" if carrier == 1 else "disconnected" 
                                    logging.info(f"Interface: {ifname} - Status: {status}") # 记录日志
                                    print(f"Interface: {ifname} - Status: {status}")
                                    self.last_carrier[ifname] = carrier
                    
        except Exception as e:
            logging.error(f"Error in monitor: {e}")
        finally:
            self.cleanup()
    # 清理资源        
    def cleanup(self):
        self.ip.close()
        logging.info("Network monitoring stopped")

if __name__ == '__main__':
    try:
        monitor = NetworkMonitor()
        monitor.run()
    except KeyboardInterrupt:
        pass

```
## 6.參考資料
https://www.infradead.org/~tgr/libnl/doc/route.html
https://docs.pyroute2.org/iproute.html

