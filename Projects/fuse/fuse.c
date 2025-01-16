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

// 日志函数
static void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsyslog(LOG_ERR, format, args);
    if (fuse_get_context()->fuse && fuse_get_context()->fuse->conf.foreground) {
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
    }
    va_end(args);
}

// 获取设备 IP 地址
static char* get_device_ip(const char* iface)
{
    static char ip_str[16];
    char cmd[256];
    FILE *fp;
    char buf[256];

    sprintf(ip_str, "0.0.0.0\n");
    if (!iface) {
        log_error("Invalid interface name");
        return ip_str;
    }

    snprintf(cmd, sizeof(cmd), "nmcli -g IP4.ADDRESS device show %s", iface);
    fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to execute command: %s", cmd);
        return ip_str;
    }

    if (fgets(buf, sizeof(buf), fp) != NULL) {
        char *slash = strchr(buf, '/');
        if (slash) *slash = '\0';
        snprintf(ip_str, sizeof(ip_str), "%s\n", buf);
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

    fp = popen(cmd, "r");
    if (!fp) {
        log_error("Failed to execute command");
        return -1;
    }

    char output[1024] = {0};
    while (fgets(output, sizeof(output)-1, fp)) {
        log_error("nmcli output: %s", output);
    }

    ret = pclose(fp);
    if (ret != 0) {
        log_error("Command failed with return code %d", ret);
        return -1;
    }

    return 0;
}

// 获取时区
static char* get_timezone(const char* param)
{
    static char tz[256];
    FILE *fp;
    
    fp = popen("timedatectl show --property=Timezone | cut -d= -f2", "r");
    if (!fp) {
        sprintf(tz, "Unknown\n");
        return tz;
    }
    
    if (fgets(tz, sizeof(tz), fp) == NULL) {
        sprintf(tz, "Unknown\n");
    }
    
    pclose(fp);
    return tz;
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

// 读取目录内容
static int network_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    size_t path_len = strlen(path);
    for (struct fs_entry *entry = fs_entries; entry->path != NULL; entry++) {
        if (strncmp(path, entry->path, path_len) == 0) {
            const char *name = entry->path + path_len;
            if (name[0] == '/' && strchr(name + 1, '/') == NULL) { // 只显示一级子目录
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
    struct fs_entry *entry = find_fs_entry(path);
    if (!entry)
        return -ENOENT;
    
    if (!S_ISREG(entry->mode))
        return -EISDIR;
    
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
    struct fs_entry *entry = find_fs_entry(path);
    if (entry && entry->setter) {
        char content[1024];
        if (size >= sizeof(content))
            return -EFBIG;

        memcpy(content, buf, size);
        content[size] = '\0';

        char *newline = strchr(content, '\n');
        if (newline)
            *newline = '\0';

        if (entry->setter(entry->param, content) != 0)
            return -EIO;

        return size;
    }
    return -ENOENT;
}

// 截断文件
static int network_truncate(const char *path, off_t size)
{
    struct fs_entry *entry = find_fs_entry(path);
    if (entry && S_ISREG(entry->mode)) {
        return 0;
    }
    return -ENOENT;
}

// 打开文件
static int network_open(const char *path, struct fuse_file_info *fi)
{
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
    openlog("network_fuse", LOG_PID | LOG_CONS, LOG_USER);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mountpoint>\n", argv[0]);
        return 1;
    }

    char *mountpoint = argv[1];
    char *default_args[] = {
        argv[0],
        mountpoint,
        "-o", 
        "allow_other,default_permissions,rw",
        NULL
    };
    int default_argc = sizeof(default_args) / sizeof(default_args[0]) - 1;

    int ret = fuse_main(default_argc, default_args, &network_oper, NULL);
    
    closelog();
    return ret;
}