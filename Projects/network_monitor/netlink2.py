#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
from datetime import datetime
from pyroute2 import IPRoute, IPDB
import logging

# 配置日志
LOG_FILE = "/tmp/network-status.log"
logging.basicConfig(
    filename=LOG_FILE,
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

def get_carrier_state(interface):
    """获取接口的carrier状态"""
    try:
        with open(f"/sys/class/net/{interface}/carrier") as f:
            return f.read().strip() == "1"
    except:
        return False
      
def monitor_interfaces():
    """监控网络接口状态"""
    ip = IPRoute()
    
    # 初始化状态
    interfaces = {
        'wlan0': {'last_state': get_carrier_state('wlan0')},
        'enp3s0': {'last_state': get_carrier_state('enp3s0')}
    }
    
    try:
        logging.info("Network monitoring started")
        
        # 订阅网络事件
        ip.bind()  # 绑定到netlink
        
        while True:
            # 阻塞等待消息
            for msg in ip.get():
                if msg['event'] == 'RTM_NEWLINK':  # 只处理链路状态变化事件
                    ifname = msg.get_attr('IFLA_IFNAME')
                    
                    # 只处理关注的接口
                    if ifname in interfaces:
                        # 获取当前状态
                        current_state = get_carrier_state(ifname)
                        last_state = interfaces[ifname]['last_state']
                        
                        # 状态发生变化
                        if current_state != last_state:
                            status = "connected" if current_state else "disconnected"
                            logging.info(f"Interface: {ifname} - Status: {status}")
                            
                            # 更新状态记录
                            interfaces[ifname]['last_state'] = current_state
                    
    except Exception as e:
        logging.error(f"Error in monitor_interfaces: {e}")
    finally:
        ip.close()

if __name__ == '__main__':
    try:
        monitor_interfaces()
    except KeyboardInterrupt:
        logging.info("Network monitoring stopped")