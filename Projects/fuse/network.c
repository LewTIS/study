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

// 文件内容缓存
//static char lan_content[1024] = "192.168.1.100\n";
//static char timezone_content[1024] = "Asia/Shanghai (CST, +0800)\n";

// 获取设备 IP 地址
static char *get_device_ip(const char* iface)
{
    static char ip_str[16];  // 存储 IP 地址的缓冲区
    char cmd[256];
    FILE *fp;
    char buf[256];

    // 初始化为默认值
    sprintf(ip_str, "0.0.0.0\n");

    if (!iface) {
        fprintf(stderr, "Invalid interface name\n");
        return ip_str;
    }
    // 构造命令
    if (snprintf(cmd, sizeof(cmd), "nmcli -g IP4.ADDRESS device show %s", iface) >= sizeof(cmd)) {
        fprintf(stderr, "Command too long\n");
        return ip_str;
    }

    // 执行命令
    fp = popen(cmd, "r");
    if (!fp) {
        fprintf(stderr, "Failed to execute command: %s\n", cmd);
        return ip_str;
    }

    // 读取输出
    if (fgets(buf, sizeof(buf), fp) != NULL) {
        char *slash = strchr(buf, '/');
        if (slash) *slash = '\0';
        if (snprintf(ip_str, sizeof(ip_str), "%s\n", buf) >= sizeof(ip_str)) {
            fprintf(stderr, "IP address too long\n");
            sprintf(ip_str, "0.0.0.0\n");
        }
    }

    pclose(fp);
    return ip_str;
}

// 设置设备 IP 地址
static int set_device_ip(const char* iface, const char* ip_str)
{
    char cmd[1024];  
    char conn_name[256];
    FILE *fp;
    int ret;

    if (!iface || !ip_str) {
        fprintf(stderr, "Invalid parameters\n");
        return -1;
    }

    // 获取连接名称
    snprintf(cmd, sizeof(cmd), "nmcli -g GENERAL.CONNECTION device show %s", iface);
    fp = popen(cmd, "r");
    if (!fp) {
        fprintf(stderr, "Failed to get connection name\n");
        return -1;
    }

    if (fgets(conn_name, sizeof(conn_name), fp) == NULL) {
        fprintf(stderr, "Failed to read connection name\n");
        pclose(fp);
        return -1;
    }
    pclose(fp);
    
    // 去掉连接名称末尾的换行符
    conn_name[strcspn(conn_name, "\n")] = 0;

    
    // 1. 修改连接  设置ip地址
    ret = snprintf(cmd, sizeof(cmd), 
                  "nmcli connection modify \"%s\" "
                  "ipv4.method manual "
                  "ipv4.addresses \"%s/24\" "
                  "ipv4.gateway \"10.0.2.2\" "
                  "ipv4.dns \"8.8.8.8,8.8.4.4\"",
                  conn_name, ip_str);
    if (ret >= sizeof(cmd)) {
        fprintf(stderr, "Command buffer too small\n");
        return -1;
    }

    if (system(cmd) != 0) {
        fprintf(stderr, "Failed to modify connection\n");
        return -1;
    }

    // 2. 重新激活连接
    ret = snprintf(cmd, sizeof(cmd), "nmcli connection up \"%s\"", conn_name);
    if (ret >= sizeof(cmd)) {
        fprintf(stderr, "Command buffer too small\n");
        return -1;
    }

    if (system(cmd) != 0) {
        fprintf(stderr, "Failed to activate connection\n");
        return -1;
    }

    return 0;
}
//获取系统时区
static char *get_timezone()
{
    static char timezone[256]="CST";
    char cmd[256];
    FILE *fp;
    char buf[256];
    
    // 构造命令-获取系统时区
    if(snprintf(cmd,sizeof(cmd),"timedatectl | grep 'Time zone' | awk -F': ' '{print $2}'") >= sizeof(cmd))
    {
        fprintf(stderr,"Command too long\n");
        return timezone;
    }
    // 执行命令
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
            fprintf(stderr,"Timezone too long\n");
            sprintf(timezone,"CST");
        }
    }
    pclose(fp);
    return timezone;
}

// 设置系统时区
//sudo timedatectl set-timezone 时区名称
static int set_timezone(const char *timezone)
{
    char cmd[256];
    int ret;

    if(!timezone)
    {
        fprintf(stderr,"Invalid parameters\n");
        return -1;
    }

    ret = snprintf(cmd, sizeof(cmd),"timedatectl set-timezone %s", timezone);
    if(ret >= sizeof(cmd))
    {
        fprintf(stderr,"Command buffer too small\n");
        return -1;
    }

    if (system(cmd) != 0)
    {
        fprintf(stderr,"Failed to execute command\n");
        return -1;
    }

    /*
    char *new_timezone = get_timezone();
    size_t len = strlen(new_timezone);
    if(len > 0 && new_timezone[len-1] == '\n')
    {
        new_timezone[len-1] = '\0';
    }
    snprintf(timezone_content,sizeof(timezone_content),"%s\n",new_timezone);
    */
    return 0;
}

//获取目录/文件的属性
static int network_getattr(const char *path, struct stat *stbuf)
{
    memset(stbuf, 0, sizeof(struct stat));

    // 根目录
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_uid = getuid();  // 添加用户ID
        stbuf->st_gid = getgid();  // 添加组ID
        return 0;
    }

    // network 目录
    if (strcmp(path, "/network") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
        return 0;
    }

    // service 目录
    if (strcmp(path,"/service") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
        return 0;
    }

    // LAN 文件
    if (strcmp(path, "/network/LAN") == 0) {
        char *lan_content = get_device_ip("enp0s3");
        stbuf->st_mode = S_IFREG | 0666;  
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(lan_content);
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
        return 0;
    }

    // timezone 文件
    if(strcmp(path, "/service/timezone") == 0) {
        char *timezone = get_timezone();
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(timezone) + 1;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
        return 0;
    }

    return -ENOENT;
}


// 读取目录内容
static int network_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    if (strcmp(path, "/") == 0) {
        filler(buf, "network", NULL, 0);
        filler(buf, "service", NULL, 0);
        return 0;
    }

    if (strcmp(path, "/network") == 0) {
        filler(buf, "LAN", NULL, 0);
        return 0;
    }
    if (strcmp(path,"/service") == 0) {
        filler(buf, "timezone", NULL, 0);
        return 0;
    }

    return -ENOENT;
}

// 读取文件内容
static int network_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    char *content;
    size_t len;
    (void) fi;

    if (strcmp(path, "/network/LAN") == 0) {
        content = get_device_ip("enp0s3");
        len = strlen(content);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, content + offset, size);
        } else
            size = 0;
        return size;
    }
    if (strcmp(path,"/service/timezone") == 0) {
        content = get_timezone();
        len = strlen(content);
        if(offset < len) {
            if(offset + size > len)
                size = len - offset;
            memcpy(buf, content + offset, size);
        }else
            size = 0;
        return size;
    }

    return -ENOENT;
}

// 写入文件内容
static int network_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi)
{
    char ip_str[16];
    (void) fi;
    (void) offset;

    if (strcmp(path, "/network/LAN") == 0) {
        if (size >= sizeof(ip_str))
            return -EFBIG;

        memcpy(ip_str, buf, size);
        ip_str[size] = '\0';

        // 移除可能的换行符
        char *newline = strchr(ip_str, '\n');
        if (newline)
            *newline = '\0';

        // 设置设备 IP
        if (set_device_ip("enp0s3", ip_str) != 0)
            return -EIO;

        return size;
    }
    
    if (strcmp(path,"/service/timezone") == 0) {
        
        if (set_timezone(buf) != 0)
            return -EIO;
        
        return size;
    }

    return -ENOENT;
}
// 打开文件
static int network_open(const char *path, struct fuse_file_info *fi)
{
     if (strcmp(path, "/network/LAN") == 0 || strcmp(path, "/service/timezone") == 0) {
        return 0;
    }

    return -ENOENT;
}
static int network_truncate(const char *path, off_t size)
{
    if (strcmp(path, "/network/LAN") == 0) {
        
        return 0;
    }
    if (strcmp(path,"/service/timezone") == 0) {
        
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
    // 定义默认选项
    char *default_opts[] = {
        argv[0],                  // 程序名
        "-o",
        "allow_other,default_permissions,rw",
        argv[argc-1],            // 挂载点
        NULL
    };
    int new_argc = 4;            // 新参数数量

    // 使用新的参数数组
    return fuse_main(new_argc, default_opts, &network_oper, NULL);
}