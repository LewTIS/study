#!/usr/bin/env python3

import os
import stat
import errno
import time
import logging
from fuse import FUSE, Operations, FuseOSError
from pydbus import SystemBus
from gi.repository import GLib

# 设置日志
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

class NetworkFUSE(Operations):
    def __init__(self):
        try:
            self.bus = SystemBus()
            self.nm = self.bus.get('org.freedesktop.NetworkManager')
        except Exception as e:
            logger.error(f"Failed to initialize NetworkManager: {e}")
            raise

    def get_device_ip(self, iface):
        """获取网络接口的 IP 地址"""
        try:
            devices = self.nm.GetDevices()
            for device_path in devices:
                device = self.bus.get('org.freedesktop.NetworkManager', device_path)
                if device.Interface == iface:
                    active_connection_path = device.ActiveConnection
                    if not active_connection_path:
                        logger.warning(f"No active connection for {iface}")
                        return "0.0.0.0\n"

                    active_connection = self.bus.get('org.freedesktop.NetworkManager', 
                                                   active_connection_path)
                    ip4_config_path = active_connection.Ip4Config
                    if not ip4_config_path:
                        logger.warning(f"No IP4 config for {iface}")
                        return "0.0.0.0\n"

                    ip4_config = self.bus.get('org.freedesktop.NetworkManager', 
                                            ip4_config_path)
                    addresses = ip4_config.AddressData
                    if addresses:
                        ip = addresses[0].get('address', '0.0.0.0')
                        logger.debug(f"Got IP for {iface}: {ip}")
                        return f"{ip}\n"
                    
            logger.warning(f"Interface {iface} not found")
            return "0.0.0.0\n"

        except Exception as e:
            logger.error(f"Error getting IP: {e}")
            return "0.0.0.0\n"

    def set_device_ip(self, iface, ip_str):
        """设置网络接口的 IP 地址"""
        try:
            devices = self.nm.GetDevices()
            for device_path in devices:
                device = self.bus.get('org.freedesktop.NetworkManager', device_path)
                if device.Interface == iface:
                    active_connection_path = device.ActiveConnection
                    if not active_connection_path:
                        logger.error(f"No active connection for {iface}")
                        return -1

                    active_connection = self.bus.get('org.freedesktop.NetworkManager', 
                                                   active_connection_path)
                    connection_path = active_connection.Connection
                    if not connection_path:
                        logger.error("No connection settings")
                        return -1

                    connection = self.bus.get('org.freedesktop.NetworkManager', 
                                            connection_path)
                    settings = connection.GetSettings()
                    
                    # 只更新 IP 地址，保持其他设置不变
                    ipv4_settings = settings.get('ipv4', {})
                    current_method = ipv4_settings.get('method', 'auto')
                    if current_method != 'manual':
                        ipv4_settings['method'] = 'manual'

                    # 保持现有网关和其他设置，只更新 IP 地址
                    current_addr = ipv4_settings.get('addresses', [[None, 24, None]])[0]
                    ipv4_settings['addresses'] = [[ip_str.strip(), current_addr[1], current_addr[2]]]
                    
                    settings['ipv4'] = ipv4_settings
                    
                    # 应用新设置
                    connection.Update(settings)
                    logger.debug(f"Successfully set IP for {iface} to {ip_str}")
                    return 0

            logger.error(f"Interface {iface} not found")
            return -1

        except Exception as e:
            logger.error(f"Error setting IP: {e}")
            return -1

    def getattr(self, path, fh=None):
        """获取文件属性"""
        logger.debug(f"getattr: {path}")
        
        # 根目录
        if path == '/':
            return dict(
                st_mode=stat.S_IFDIR | 0o755,
                st_nlink=2,
                st_size=0,
                st_ctime=time.time(),
                st_mtime=time.time(),
                st_atime=time.time(),
                st_uid=os.getuid(),
                st_gid=os.getgid()
            )
            
        # network 目录
        if path == '/network':
            return dict(
                st_mode=stat.S_IFDIR | 0o755,
                st_nlink=2,
                st_size=0,
                st_ctime=time.time(),
                st_mtime=time.time(),
                st_atime=time.time(),
                st_uid=os.getuid(),
                st_gid=os.getgid()
            )
            
        # LAN 文件
        if path == '/network/LAN':
            content = self.get_device_ip('enp0s3')
            return dict(
                st_mode=stat.S_IFREG | 0o666,
                st_nlink=1,
                st_size=len(content),
                st_ctime=time.time(),
                st_mtime=time.time(),
                st_atime=time.time(),
                st_uid=os.getuid(),
                st_gid=os.getgid()
            )
            
        raise FuseOSError(errno.ENOENT)

    def readdir(self, path, fh):
        """读取目录内容"""
        logger.debug(f"readdir: {path}")
        
        if path == '/':
            return ['.', '..', 'network']
        if path == '/network':
            return ['.', '..', 'LAN']
        return ['.', '..']

    def read(self, path, size, offset, fh):
        """读取文件内容"""
        logger.debug(f"read: {path}")
        
        if path == '/network/LAN':
            content = self.get_device_ip('enp0s3')
            return content[offset:offset + size].encode('utf-8')
        raise FuseOSError(errno.ENOENT)

    def write(self, path, data, offset, fh):
        """写入文件内容"""
        logger.debug(f"write: {path}")
        
        if path == '/network/LAN':
            ip_str = data.decode().strip()
            if self.set_device_ip('enp0s3', ip_str) == 0:
                return len(data)
            raise FuseOSError(errno.EIO)
        raise FuseOSError(errno.ENOENT)

    def open(self, path, flags):
        """打开文件"""
        logger.debug(f"open: {path}")
        if path != '/network/LAN':
            raise FuseOSError(errno.ENOENT)
        return 0

def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('mount_point')
    args = parser.parse_args()

    logging.getLogger().setLevel(logging.DEBUG)
    
    # 确保挂载点存在
    if not os.path.exists(args.mount_point):
        os.makedirs(args.mount_point)

    # 运行文件系统
    FUSE(NetworkFUSE(), args.mount_point, foreground=True, allow_other=True)

if __name__ == '__main__':
    main()