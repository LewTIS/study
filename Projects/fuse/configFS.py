#!/usr/bin/env python3

import os
import sys
import errno
import subprocess
import yaml
from fuse import FUSE, FuseOSError, Operations
from time import time
import logging
from typing import Dict, Any
from abc import ABC, abstractmethod
import stat
from threading import Timer
import threading

# 配置日志
logging.basicConfig(
    filename='/tmp/fuse.log',
    level=logging.DEBUG,
    format='%(asctime)s - %(message)s'
)

class VirtualFile(ABC):
    """虚拟文件基类"""
    def __init__(self, config: Dict[str, Any]):
        self.mode = config.get('mode', 0o666)
        self.config = config

    @abstractmethod
    def read(self) -> str:
        pass

    @abstractmethod
    def write(self, data: str) -> bool:
        pass

class CommandFile(VirtualFile):
    """基于命令的配置文件"""
    def read(self) -> str:
        try:
            read_cmd = self.config['read_cmd']
            output = subprocess.check_output(read_cmd, shell=True).decode().strip()
            if self.config.get('process_output'):
                locals = {'output': output}
                exec(self.config['process_output'], {}, locals)
                output = locals['output']
            return output + '\n'
        except Exception as e:
            logging.error(f"Read failed: {str(e)}")
            return self.config.get('default_value', 'Error\n')

    def write(self, data: str) -> bool:
        try:
            if 'write_cmd' not in self.config:
                return False
            write_cmd = self.config['write_cmd'].format(value=data.strip())
            subprocess.check_call(write_cmd, shell=True)
            return True
        except Exception as e:
            logging.error(f"Write failed: {str(e)}")
            return False

class ConfigFS(Operations):
    """配置文件系统"""
    def __init__(self, config_path: str):
        self.config_path = config_path
        self.files: Dict[str, VirtualFile] = {}
        self.fd = 0
        self.lock = threading.Lock()
        self._load_config()
        self._start_config_monitor()

    def _load_config(self):
        """从配置文件加载文件系统结构"""
        try:
            with open(self.config_path, 'r') as f:
                config = yaml.safe_load(f)

            with self.lock:
                self.files.clear()
                for file_path, file_config in config['files'].items():
                    if not file_path.startswith('/'):
                        file_path = '/' + file_path
                    self.files[file_path] = CommandFile(file_config)
            
            logging.info("Configuration reloaded successfully")
        except Exception as e:
            logging.error(f"Failed to load config: {str(e)}")

    def _start_config_monitor(self):
        """启动配置文件监控"""
        def check_config():
            try:
                self._load_config()
            finally:
                Timer(5.0, check_config).start()  # 每5秒检查一次

        Timer(5.0, check_config).start()

    def _get_dirs(self) -> set:
        """获取所有目录"""
        with self.lock:
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

        with self.lock:
            if path in self._get_dirs():
                st['st_mode'] = stat.S_IFDIR | 0o755
                st['st_nlink'] = 2
                st['st_size'] = 4096
                return st

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
        with self.lock:
            if path == '/':
                entries.extend(set(p.split('/')[1] for p in self.files.keys()))
            else:
                prefix = path + '/'
                entries.extend(set(
                    p[len(prefix):].split('/')[0]
                    for p in self.files.keys()
                    if p.startswith(prefix)
                ))
        return entries

    def read(self, path: str, size: int, offset: int, fh) -> bytes:
        """读取文件内容"""
        logging.debug(f"read: {path}")
        
        with self.lock:
            if path in self.files:
                content = self.files[path].read()
                return content[offset:offset + size].encode()
        return b''

    def write(self, path: str, data: bytes, offset: int, fh) -> int:
        """写入文件内容"""
        logging.debug(f"write: {path}")
        
        with self.lock:
            if path in self.files:
                content = data.decode()
                if self.files[path].write(content):
                    return len(data)
        return 0

    def truncate(self, path: str, length: int, fh=None):
        pass

    def open(self, path: str, flags):
        self.fd += 1
        return self.fd

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <config_file> <mountpoint>")
        sys.exit(1)

    FUSE(ConfigFS(sys.argv[1]), sys.argv[2], foreground=False, allow_other=True)

if __name__ == '__main__':
    main()