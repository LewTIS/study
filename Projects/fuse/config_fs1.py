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

# 配置日志
logging.basicConfig(
    #filename='/tmp/config_fs.log',
    filename ='/var/log/config_fs.log',
    level=logging.DEBUG,
    format='%(asctime)s - %(message)s'
)


class VirtualFile(ABC):
    """虚拟文件基类"""
    def __init__(self, config: Dict[str, Any]): #接收一个字典类型的参数
        self.mode = config.get('mode', 0o666) 
        self.config = config    

    @abstractmethod
    def read(self) -> str:
        """读取文件内容"""
        pass

    @abstractmethod
    def write(self, data: str) -> bool:
        """写入文件内容"""
        pass

# 负责执行读取和写入文件
class CommandFile(VirtualFile):
    """基于命令的配置文件"""
    def read(self) -> str:
        try:
            read_cmd = self.config['read_cmd']
            result = subprocess.run(read_cmd, shell=True, capture_output=True, text=True) #执行read_cmd命令
            output = result.stdout.strip() #获取输出内容
            #output = subprocess.check_output(read_cmd, shell=True).decode().strip()
            if self.config.get('process_output'):
                # 执行自定义的输出处理
                locals = {'output': output}
                exec(self.config['process_output'], {}, locals) #执行自定义的输出处理，获取output变量'/'之前的内容
                output = locals['output']
            return output + '\n'
        except Exception as e:
            logging.error(f"Read failed: {str(e)}")
            return self.config.get('default_value', 'Error') + '\n'

    def write(self, data: str) -> bool:
        try:
            if 'write_cmd' not in self.config:
                return False
            write_cmd = self.config['write_cmd'].format(value=data.strip()) 
            result = subprocess.run(write_cmd,shell=True,check=True)
            #subprocess.check_call(write_cmd, shell=True) # 执行write_cmd命令，将data的内容写入文件
            return True
        except Exception as e:
            logging.error(f"Write failed: {str(e)}")
            return False

class FSConfig(Operations):  #继承自Operations类，包含FUSE的基本接口
    """配置文件系统"""
    def __init__(self, config_path: str):
        self.files: Dict[str, VirtualFile] = {}  #文件路径和文件对象的映射
        self.fd = 0
        self._load_config(config_path)

    # 从配置文件加载文件系统结构，将配置文件中的文件信息加载到self.files字典中，key为文件路径，value为CommandFile对象
    def _load_config(self, config_path: str):
        """从配置文件加载文件系统结构"""
        try:
            with open(config_path, 'r') as f: 
                config = yaml.safe_load(f) # 读取和解析配置文件 

            for file_path, file_config in config['files'].items():
                if not file_path.startswith('/'):
                    file_path = '/' + file_path
                self.files[file_path] = CommandFile(file_config) # 每个文件对应一个CommandFile对象，方便进行读取和写入

        except Exception as e:
            logging.error(f"Failed to load config: {str(e)}")
            sys.exit(1)

    def _get_dirs(self) -> set:  
        """获取所有目录 {'/','/network','/service'}"""
        # 辅助getattr方法，判断是否是目录
        dirs = {'/'}
        for path in self.files.keys():
            parts = path.split('/') # 根据'/'将路径分割为多个部分
            for i in range(len(parts)):
                if i > 0:
                    dirs.add('/'.join(parts[:i])) # 将目录添加到dirs集合中
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

        # 目录
        if path in self._get_dirs():
            st['st_mode'] = stat.S_IFDIR | 0o755
            st['st_nlink'] = 2  # 硬链接数，最少为2
            st['st_size'] = 4096
            return st
       
        # 文件
        if path in self.files:
            st['st_mode'] = stat.S_IFREG | self.files[path].mode
            st['st_nlink'] = 1
            st['st_size'] = len(self.files[path].read())  # 获取文件内容的长度，调用CommandFile类的read方法
            return st

        raise FuseOSError(errno.ENOENT)

    def readdir(self, path: str, fh) -> list:
        """读取目录内容"""
        logging.debug(f"readdir: {path}")
        
        entries = ['.', '..']
        # 根目录下的子目录
        if path == '/':
            entries.extend(set(p.split('/')[1] for p in self.files.keys())) # 只获取一级目录
        else: # 子目录下的目录和文件
            prefix = path + '/' # 给当前访问的目录加上'/'，如：/network/
            # 筛选以prefix开头的目录和文件，去掉prefix后，再以'/'分割，取第一个元素，得到子目录或文件名
            # 如：/network/LAN -> LAN 
            entries.extend(set(
                p[len(prefix):].split('/')[0]
                for p in self.files.keys()
                if p.startswith(prefix)
            ))
        return entries

    def read(self, path: str, size: int, offset: int, fh) -> bytes: #返回bytes类型，而不是str类型，FUSE接口要求
        """读取文件内容"""
        logging.debug(f"read: {path}")
        
        if path in self.files:
            content = self.files[path].read()
            return content[offset:offset + size].encode() # 将字符串转换为字节串
        return b''

    def write(self, path: str, data: bytes, offset: int, fh) -> int:
        """写入文件内容"""
        logging.debug(f"write: {path}")
       
        if path not in self.files:
            raise FuseOSError(errno.ENOENT)
        
        if not(self.files[path].mode & stat.S_IWUSR): # 判断该文件，用户是否可写
            raise FuseOSError(errno.EACCES)
        
        content = data.decode()
        if self.files[path].write(content):
            return len(data)
        
        raise FuseOSError(errno.EIO) # 写入失败

    def truncate(self, path: str, length: int, fh=None):
        """截断文件"""
        pass

    def open(self, path: str, flags):
        """打开文件"""
        self.fd += 1
        return self.fd  # 返回文件描述符
    def getxattr(self, path: str, name: str, position=0):
        """获取扩展属性"""
        return b'' # 返回空字节串，表示没有扩展属性
def main():
    # 检查命令行参数
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <config_file> <mountpoint>")
        sys.exit(1)

    FUSE(FSConfig(sys.argv[1]), sys.argv[2], foreground=False, allow_other=True) # 创建FUSE文件系统实例，并挂载到指定的挂载点上

if __name__ == '__main__':
    main()