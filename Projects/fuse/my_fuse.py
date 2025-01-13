#!/usr/bin/env python3
from fuse import FUSE,FuseOSError,Operations
from time import time
import os
import errno
import stat

class NetworkFS(Operations):
    def __init__(self):
        self.files = {
            '/network/LAN':'192.168.1.100\n',
            '/network/WiFi_sta':'192.168.2.100\n'
        }

        # 文件属性
        self.fd = 0
        now = time()
        self.file_attrs = {
            '/':dict(st_mode=(stat.S_IFDIR | 0o755),st_nlink=2,st_size=0,
                    st_ctime=now,st_mtime=now,st_atime=now,
                    st_uid=os.getuid(),st_gid=os.getgid()),
            '/network':dict(st_mode=(stat.S_IFDIR | 0o755),st_nlink=2,st_size=0,
                    st_ctime=now,st_mtime=now,st_atime=now,
                    st_uid=os.getuid(),st_gid=os.getgid()),
            '/network/LAN':dict(st_mode=(stat.S_IFREG | 0o666),st_nlink=1,st_size=len('192.168.1.100\n'),
                    st_ctime=now,st_mtime=now,st_atime=now,st_uid=os.getuid(),st_gid=os.getgid()),
            '/network/WiFi_sta':dict(st_mode=(stat.S_IFREG | 0o666),st_nlink=1,st_size=len('192.168.2.100\n'),
                    st_ctime=now,st_mtime=now,st_atime=now,st_uid=os.getuid(),st_gid=os.getgid()),
        }
    
    #获取文件属性
    def getattr(self,path,fh=None):
        if path not in self.file_attrs:
            raise FuseOSError(errno.ENOENT)
        return self.file_attrs[path]

    #获取目录内容
    def readdir(self,path,fh):
        if path == '/':
            return ['.','..','network']
        elif path == '/network':
            return ['.','..','LAN','WiFi_sta']
        else:
            raise FuseOSError(errno.ENOENT)
    
    #读取文件内容
    def read(self,path,size,offset,fh):
        if path not in self.files:
            raise FuseOSError(errno.ENOENT)
        return self.files[path][offset:offset+size]
    
    # 写入文件内容
    def write(self,path,data,offset,fh):
        if path not in self.files:
            raise FuseOSError(errno.ENOENT)
        
        content = data.decode('utf-8')
        #更新文件内容
        self.files[path] = content
        self.file_attrs[path]['st_size'] = len(content)
        self.file_attrs[path]['st_mtime']= time()
        
        return len(data)
    def open(self,path,flags):
        self.fd += 1
        return self.fd
    
def main():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('mount_point')
    args = parser.parse_args()

    FUSE(NetworkFS(),args.mount_point,foreground=True,allow_other=True,default_permissions=False)
if __name__ == '__main__':
    main()