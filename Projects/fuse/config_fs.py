#!/usr/bin/env python3

import os
import sys
import errno
import subprocess
from fuse import FUSE, FuseOSError, Operations
from time import time
import logging
from typing import Dict, Callable, Optional
from abc import ABC, abstractmethod
import stat

# 配置日志
logging.basicConfig(
    filename='/tmp/fuse.log',
    level=logging.DEBUG,
    format='%(asctime)s - %(message)s'
)

class VirtualFile(ABC):
    """虚拟文件基类"""
    def __init__(self):
        self.mode = 0
        self._content = ""

    @abstractmethod
    def read(self) -> str:
        """读取文件内容"""
        pass

    @abstractmethod
    def write(self, data: str) -> bool:
        """写入文件内容"""
        pass

class NetworkFile(VirtualFile):
    """网络配置文件"""
    def __init__(self, interface: str):
        super().__init__()
        self.mode = 0o666
        self.interface = interface

    def read(self) -> str:
        """获取IP地址"""
        try:
            cmd = f"nmcli -g IP4.ADDRESS device show {self.interface}"
            output = subprocess.check_output(cmd, shell=True).decode().strip()
            if '/' in output:
                output = output.split('/')[0]
            return output + '\n'
        except:
            return "0.0.0.0\n"

    def write(self, data: str) -> bool:
        """设置IP地址"""
        try:
            ip = data.strip()
            cmd = (f"nmcli device modify {self.interface} "
                  f"ipv4.method manual "
                  f"ipv4.addresses \"{ip}/24\" "
                  f"ipv4.gateway \"10.0.2.2\" "
                  f"ipv4.dns \"10.180.8.13,10.180.8.12\"")
            subprocess.check_call(cmd, shell=True)
            return True
        except Exception as e:
            logging.error(f"Failed to set IP: {str(e)}")
            return False

class TimezoneFile(VirtualFile):
    """时区配置文件"""
    def __init__(self):
        super().__init__()
        self.mode = 0o666

    def read(self) -> str:
        """获取时区"""
        try:
            cmd = "timedatectl show --property=Timezone | cut -d= -f2"
            return subprocess.check_output(cmd, shell=True).decode()
        except:
            return "Unknown\n"

    def write(self, data: str) -> bool:
        """设置时区"""
        try:
            timezone = data.strip()
            subprocess.check_call(f"timedatectl set-timezone {timezone}", shell=True)
            return True
        except:
            return False

class ConfigFS(Operations):
    """配置文件系统"""
    def __init__(self):
        self.files: Dict[str, VirtualFile] = {}
        self.fd = 0
        self._init_files()

    def _init_files(self):
        """初始化文件系统结构"""
        # 添加网络配置文件
        self.files['/network/LAN'] = NetworkFile('enp0s3')
        # 添加时区配置文件
        self.files['/service/timezone'] = TimezoneFile()

    def _get_dirs(self) -> set:
        """获取所有目录"""
        dirs = {'/'}
        for path in self.files.keys():
            parts = path.split('/')
            for i in range(len(parts)):
                if i > 0:
                    dirs.add('/'.join(parts[:i]))
        return dirs

    def getattr(self, path: str, fh=None):
        """获取文件属性"""
        logging.debug(f"getattr: {path}")
        
        st = {
            'st_atime': time(),
            'st_mtime': time(),
            'st_ctime': time(),
            'st_uid': os.getuid(),
            'st_gid': os.getgid()
        }

        # 处理目录
        if path in self._get_dirs():
            st['st_mode'] = stat.S_IFDIR | 0o755
            st['st_nlink'] = 2
            st['st_size'] = 4096
            return st

        # 处理文件
        if path in self.files:
            st['st_mode'] = stat.S_IFREG | self.files[path].mode
            st['st_nlink'] = 1
            st['st_size'] = len(self.files[path].read())
            return st

        raise FuseOSError(errno.ENOENT)

    def readdir(self, path: str, fh) -> list:
        """读取目录内容"""
        logging.debug(f"readdir: {path}")
        
        entries = ['.', '..']
        if path == '/':
            # 根目录下的目录
            entries.extend(set(p.split('/')[1] for p in self.files.keys()))
        else:
            # 子目录下的文件/目录
            prefix = path + '/'
            entries.extend(
                p[len(prefix):].split('/')[0]
                for p in self.files.keys()
                if p.startswith(prefix)
            )
        return entries

    def read(self, path: str, size: int, offset: int, fh) -> bytes:
        """读取文件内容"""
        logging.debug(f"read: {path}")
        
        if path in self.files:
            content = self.files[path].read()
            return content[offset:offset + size].encode()
        return b''

    def write(self, path: str, data: bytes, offset: int, fh) -> int:
        """写入文件内容"""
        logging.debug(f"write: {path}")
        
        if path in self.files:
            content = data.decode()
            if self.files[path].write(content):
                return len(data)
        return 0

    def truncate(self, path: str, length: int, fh=None):
        """截断文件"""
        pass

    def open(self, path: str, flags):
        """打开文件"""
        self.fd += 1
        return self.fd

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <mountpoint>")
        sys.exit(1)

    FUSE(ConfigFS(), sys.argv[1], foreground=False, allow_other=True)

if __name__ == '__main__':
    main()