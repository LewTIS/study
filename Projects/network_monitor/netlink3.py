#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import asyncio
import selectors
from datetime import datetime
from pyroute2 import IPRoute
import logging

# 配置日志
LOG_FILE = "/var/log/network-monitor.log"
logging.basicConfig(
    filename=LOG_FILE,
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

class NetworkMonitor:
    def __init__(self):
        self.ip = IPRoute()
        self.selector = selectors.DefaultSelector()
        self.monitored_interfaces = ['wlan0', 'enp3s0']
        
    def handle_event(self, fileobj, mask):
        """处理网络事件"""
        for msg in self.ip.get():
            if msg['event'] == 'RTM_NEWLINK':
                ifname = msg.get_attr('IFLA_IFNAME')
                if ifname in self.monitored_interfaces:
                    # 获取carrier状态
                    carrier = msg.get_attr('IFLA_CARRIER')
                    if carrier is not None:  # 只在carrier状态变化时记录
                        status = "connected" if carrier == 1 else "disconnected"
                        logging.info(f"Interface: {ifname} - Status: {status}")

    async def monitor(self):
        """开始监控网络接口"""
        try:
            logging.info("Network monitoring started")
            
            # 绑定netlink socket
            self.ip.bind()
            
            # 注册文件描述符到selector
            self.selector.register(
                self.ip.fileno(),
                selectors.EVENT_READ,
                self.handle_event
            )
            
            while True:
                # 等待事件
                events = await asyncio.get_event_loop().run_in_executor(
                    None,
                    self.selector.select
                )
                
                # 处理事件
                for key, mask in events:
                    callback = key.data
                    callback(key.fileobj, mask)
                    
        except Exception as e:
            logging.error(f"Error in monitor: {e}")
        finally:
            self.cleanup()
            
    def cleanup(self):
        """清理资源"""
        self.selector.close()
        self.ip.close()
        logging.info("Network monitoring stopped")

async def main():
    """主函数"""
    monitor = NetworkMonitor()
    try:
        await monitor.monitor()
    except asyncio.CancelledError:
        monitor.cleanup()

if __name__ == '__main__':
    try:
        loop = asyncio.get_event_loop()
        loop.run_until_complete(main())
    except KeyboardInterrupt:
        pass
    finally:
        loop.close()