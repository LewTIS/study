#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdarg.h>
#include <syslog.h>

// 文件系统条目定义
struct fs_entry {
    const char *path;           // 文件路径
    mode_t mode;               // 文件模式（目录/文件）
    char* (*getter)(const char*);  // 获取内容的函数
    int (*setter)(const char*, const char*); // 设置内容的函数
    const char *param;         // 参数（如网络接口名）
};

// 日志记录
static void log_error(const char *message) {
    time_t now;
    char timestamp[64];
    FILE *fp;

    time(&now);
    strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",localtime(&now));

    fp = fopen("/tmp/fuse.log","a");
    if (!fp){
        return;
    }

    fprintf(fp,"[%s] %s\n",timestamp,message);

    fclose(fp);
}

// 获取设备 IP 地址
static char* get_device_ip(const char* iface)
{
    static char ip_str[16];
    char cmd[256];
    FILE *fp;

    sprintf(ip_str, "0.0.0.0\n");
    if (!iface) {
        log_error("Invalid interface name");
        return ip_str;
    }
    //构造获取IP地址的命令
    snprintf(cmd, sizeof(cmd), "nmcli -g IP4.ADDRESS device show %s", iface);
    fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to get IP address");
        return ip_str;
    }
   
    // 读取输出
    if (fgets(ip_str, sizeof(ip_str), fp) != NULL) {
        char *slash = strchr(ip_str, '/');
        if (slash) *slash = '\0';
        strcat(ip_str, "\n");
    }
    
    pclose(fp);
    return ip_str;
}

// 设置设备 IP 地址
static int set_device_ip(const char* iface, const char* ip_str)
{
    char cmd[1024];
    int ret;
    FILE *fp;

    if (!iface || !ip_str) {
        log_error("Invalid parameters");
        return -1;
    }
    
    // 设置 IP 地址 
    ret = snprintf(cmd, sizeof(cmd), 
                  "nmcli device modify %s "
                  "ipv4.method manual "
                  "ipv4.addresses \"%s/24\" "
                  "ipv4.gateway \"10.0.2.2\" "
                  "ipv4.dns \"10.180.8.13,10.180.8.12\" 2>&1",
                  iface, ip_str);

    if (ret < 0 || ret >= sizeof(cmd)) {
        log_error("Command buffer too small");
        return -1;
    }
    // 执行命令并获取输出
    fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to execute command");
        return -1;
    }

    char output[512] = {0};
    while (fgets(output, sizeof(output), fp)) {
        output[sizeof(output)-1] = '\0';
        log_error(output);
    }

    ret = pclose(fp);
    if (ret != 0) {
        log_error("Command executed with error");
        return -1;
    }

    return 0;
}

// 获取时区
static char* get_timezone(const char* param)
{
    static char timezone[256];
    FILE *fp;
    char cmd[256];
    char buf[256];

    if(snprintf(cmd,sizeof(cmd),"timedatectl | grep 'Time zone' | awk -F': ' '{print $2}'") >= sizeof(cmd))
    {
        log_error("Command too long\n");
        return timezone;
    }
    // 执行命令 并获取输出
    fp = popen(cmd,"r");
    
   if(!fp)
    {
        fprintf(stderr,"Failed to execute command\n");
        return timezone;
    }
    
    // 读取输出
    if(fgets(buf,sizeof(buf),fp) != NULL)
    {
        if(snprintf(timezone,sizeof(timezone),"%s",buf) >= sizeof(timezone))
        {
            log_error("Timezone too long\n");
            sprintf(timezone,"CST");
        }
    }
    pclose(fp);
    return timezone;
}

// 设置时区
static int set_timezone(const char* param, const char* timezone)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "timedatectl set-timezone %s", timezone);
    return system(cmd);
}

// 文件系统条目表
static struct fs_entry fs_entries[] = {
    {"/network", S_IFDIR | 0755, NULL, NULL, NULL},
    {"/service", S_IFDIR | 0755, NULL, NULL, NULL},
    {"/network/LAN", S_IFREG | 0666, get_device_ip, set_device_ip, "enp0s3"},
    {"/service/timezone", S_IFREG | 0666, get_timezone, set_timezone, NULL},
    {NULL, 0, NULL, NULL, NULL}  // 结束标记
};

// 查找文件系统条目
static struct fs_entry* find_fs_entry(const char *path) {
    for (struct fs_entry *entry = fs_entries; entry->path != NULL; entry++) {
        if (strcmp(path, entry->path) == 0) {
            return entry;
        }
    }
    return NULL;
}

// 获取文件属性
static int network_getattr(const char *path, struct stat *stbuf)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "getattr called with path: %s", path);
    log_error(debug_msg);

    memset(stbuf, 0, sizeof(struct stat));
    stbuf->st_uid = getuid();
    stbuf->st_gid = getgid();
    time_t now = time(NULL);
    stbuf->st_ctime = now;
    stbuf->st_mtime = now;
    stbuf->st_atime = now;

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_size = 4096;
        return 0;
    }

    struct fs_entry *entry = find_fs_entry(path);
    if (entry) {
        stbuf->st_mode = entry->mode;
        
        if(S_ISDIR(entry->mode)) {
            stbuf->st_nlink = 2;
            stbuf->st_size = 4096;
        } else if(S_ISREG(entry->mode)) {
            stbuf->st_nlink = 1;
            if (entry->getter) {
                char *content = entry->getter(entry->param);
                stbuf->st_size = strlen(content);
            }else
                stbuf->st_size = 0;
        }
        return 0;
    }

    return -ENOENT;
}

// 读取目录
static int network_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "readdir called with path: %s", path);
    log_error(debug_msg);

    // 添加当前目录和父目录
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    // 对根目录特殊处理
    if (strcmp(path, "/") == 0) {
        // 添加顶层目录
        filler(buf, "network", NULL, 0);
        filler(buf, "service", NULL, 0);
        return 0;
    }

    // 对其他目录处理子项
    size_t path_len = strlen(path);
    for (struct fs_entry *entry = fs_entries; entry->path != NULL; entry++) {
        if (strncmp(path, entry->path, path_len) == 0) {
            const char *name = entry->path + path_len;
            if (name[0] == '/' && strchr(name + 1, '/') == NULL) {//只显示一级目录
                filler(buf, name + 1, NULL, 0);
            }
        }
    }

    return 0;
}
// 读取文件内容
static int network_read(const char *path, char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "read called with path: %s", path);
    log_error(debug_msg);

    struct fs_entry *entry = find_fs_entry(path);
    if (!entry)
        return -ENOENT;
    
    if (!S_ISREG(entry->mode))
        return -EISDIR;
    // 调用相应的 getter 函数获取文件内容
    if (entry->getter) {
        char *content = entry->getter(entry->param);
        size_t len = strlen(content);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, content + offset, size);
            return size;
        }
    }

    return 0;
}

// 写入文件内容
static int network_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "write called with path: %s", path);
    log_error(debug_msg);

    struct fs_entry *entry = find_fs_entry(path);
    if (entry && entry->setter) {
        char content[1024];
        if (size >= sizeof(content))
            return -EFBIG;

        memcpy(content, buf, size);
        content[size] = '\0';
        // 移除可能的换行符
        char *newline = strchr(content, '\n');
        if (newline)
            *newline = '\0';

        // 调用相应的 setter 函数写入文件内容
        if (entry->setter(entry->param, content) != 0)
            return -EIO;

        return size;
    }
    return -ENOENT;
}

// 截断文件
static int network_truncate(const char *path, off_t size)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "truncate called with path: %s", path);
    log_error(debug_msg);

    struct fs_entry *entry = find_fs_entry(path);
    if (entry && S_ISREG(entry->mode)) {
        return 0;
    }
    return -ENOENT;
}

// 打开文件
static int network_open(const char *path, struct fuse_file_info *fi)
{
    char debug_msg[256];
    snprintf(debug_msg, sizeof(debug_msg), "open called with path: %s", path);
    log_error(debug_msg); 

    struct fs_entry *entry = find_fs_entry(path);
    if (entry && S_ISREG(entry->mode)) {
        return 0;
    }
    return -ENOENT;                                            
}

static struct fuse_operations network_oper = {
    .getattr    = network_getattr,
    .readdir    = network_readdir,
    .open       = network_open,
    .read       = network_read,
    .write      = network_write,
    .truncate   = network_truncate,
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mountpoint>\n", argv[0]);
        return 1;
    }

    // 设置 FUSE 参数
    char *fuse_argv[] = {
        argv[0],          // 程序名
        argv[1],         // 挂载点
        "-o",           // 选项标记
        "allow_other",  // 允许其他用户访问
        NULL
    };
    int fuse_argc = sizeof(fuse_argv) / sizeof(fuse_argv[0]) - 1;

    // 打开日志
    log_error("Starting FUSE filesystem...");

    return fuse_main(fuse_argc, fuse_argv, &network_oper, NULL);
}