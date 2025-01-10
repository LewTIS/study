#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/stat.h>

//获取文件属性
static int hello_getattr(const char *path,struct stat *stbuf)
{
    memset(stbuf,0,sizeof(struct stat));

    //根目录
    if (strcmp(path,"/") == 0){
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    //hello.txt 文件
    if (strcmp(path,"/hello.txt") == 0){
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen("Hello World!\n");
        return 0;
    }

    return -ENOENT;
}
//读取目录内容
static int hello_readdir(const char *path,void *buf,fuse_fill_dir_t filler,
                        off_t offset,struct fuse_file_info *fi)
{
    if(strcmp(path,"/") != 0)
        return -ENOENT;
    filler(buf,".",NULL,0);
    filler(buf,"..",NULL,0);
    filler(buf,"hello.txt",NULL,0);

    return 0;
}
//读取文件内容
static int hello_read(const char *path,char *buf,size_t size,off_t offset,
                    struct fuse_file_info *fi)
{
    const char *hello_str = "Hello World!\n";
    size_t len = strlen(hello_str);
    
    if(strcmp(path,"/hello.txt") != 0)
        return -ENOENT;
    
    if(offset >= len)
        return 0;
    if (offset + size > len)
        size = len - offset;
    
    memcpy(buf,hello_str + offset,size);

    return size;
}
//FUSE 操作结构体
static struct fuse_operations hello_oper = {
    .getattr = hello_getattr,
    .readdir = hello_readdir,
    .read = hello_read,
};

int main(int argc,char *argv[])
{
    return fuse_main(argc,argv,&hello_oper,NULL);
}
