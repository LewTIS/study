#!/usr/bin/env python3

import os
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
        self.ip = IPRoute()
        self.monitored_interfaces = ['wlan0', 'enp3s0']
        self.last_carrier = {}
        self.init_interface_states()
        
    def get_carrier_state(self, ifname):
        try:
            with open(f"/sys/class/net/{ifname}/carrier") as f:
                return int(f.read().strip())
        except:
            return 0
        
    def init_interface_states(self):
        for ifname in self.monitored_interfaces:
            carrier = self.get_carrier_state(ifname)
            self.last_carrier[ifname] = carrier

    def run(self):
        try:
            logging.info("Network monitoring started")
            self.ip.bind(groups=RTMGRP_LINK)
            
            while True:
                for msg in self.ip.poll(): # 等待事件，这里也可以使用get()方法 
                    if msg['event'] == 'RTM_NEWLINK':
                        ifname = msg.get_attr('IFLA_IFNAME')
                        if ifname in self.monitored_interfaces:
                            carrier = msg.get_attr('IFLA_CARRIER')
                            if carrier is not None:
                                if self.last_carrier.get(ifname) != carrier:
                                    status = "connected" if carrier == 1 else "disconnected"
                                    logging.info(f"Interface: {ifname} - Status: {status}")
                                    print(f"Interface: {ifname} - Status: {status}")
                                    self.last_carrier[ifname] = carrier
                    
        except Exception as e:
            logging.error(f"Error in monitor: {e}")
        finally:
            self.cleanup()
            
    def cleanup(self):
        self.ip.close()
        logging.info("Network monitoring stopped")

if __name__ == '__main__':
    try:
        monitor = NetworkMonitor()
        monitor.run()
    except KeyboardInterrupt:
        logging.info("Program stopped by user")