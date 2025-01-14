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
#include <NetworkManager.h>
#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-ethernet.h>
#include <nm-device-wifi.h>
#include <nm-ip-config.h>


// 文件内容
static char lan_content[1024] = "192.168.1.100\n";
static char wifi_content[1024] = "192.168.2.100\n";

static NMClient *nm_client = NULL;

// 获取设备IP地址
// ... 前面的代码保持不变 ...

// 获取设备 IP 地址
static char* get_device_ip(const char* iface)
{
    NMDevice *device;
    NMIPConfig *ip4_config;
    GPtrArray *addresses;
    static char ip_str[16];  // 存储 IP 地址的缓冲区
    
    // 获取网络设备
    device = nm_client_get_device_by_iface(nm_client, iface);
    if (!device) {
        sprintf(ip_str, "0.0.0.0\n");
        return ip_str;
    }

    // 获取 IPv4 配置
    ip4_config = nm_device_get_ip4_config(device);
    if (!ip4_config) {
        sprintf(ip_str, "0.0.0.0\n");
        return ip_str;
    }

    // 获取地址列表
    addresses = nm_ip_config_get_addresses(ip4_config);
    if (!addresses || !addresses->len) {
        sprintf(ip_str, "0.0.0.0\n");
        return ip_str;
    }

    // 获取第一个地址
    NMIPAddress *addr = g_ptr_array_index(addresses, 0);
    const char *ip_string = nm_ip_address_get_address(addr);
    
    // 复制 IP 地址并添加换行符
    snprintf(ip_str, sizeof(ip_str), "%s\n", ip_string);
    
    return ip_str;
}

// 设置设备 IP 地址
static int set_device_ip(const char* iface, const char* ip_str)
{
    NMDevice *device;
    NMActiveConnection *active_connection;
    NMRemoteConnection *connection;  
    NMSettingIPConfig *s_ip4;
    GError *error = NULL;
    
    // 获取网络设备
    device = nm_client_get_device_by_iface(nm_client, iface);
    if (!device) {
        return -1;
    }

    // 获取当前活动连接
    active_connection = nm_device_get_active_connection(device);
    if (!active_connection) {
        return -1;
    }

    // 获取连接
    connection = NM_REMOTE_CONNECTION(nm_active_connection_get_connection(active_connection));  // 修改为向下转换
    if (!connection) {
        return -1;
    }

    // 获取 IPv4 配置
    s_ip4 = nm_connection_get_setting_ip4_config(NM_CONNECTION(connection));
    if (!s_ip4) {
        s_ip4 = (NMSettingIPConfig *)nm_setting_ip4_config_new();
        nm_connection_add_setting(NM_CONNECTION(connection), NM_SETTING(s_ip4));
    }

    // 清除现有 IP 地址
    nm_setting_ip_config_clear_addresses(s_ip4);

    // 添加新地址
    NMIPAddress *addr;
    addr = nm_ip_address_new(AF_INET, ip_str, 24, &error);
    if (!addr) {
        if (error) {
            g_error_free(error);
        }
        return -1;
    }

    nm_setting_ip_config_add_address(s_ip4, addr);
    nm_ip_address_unref(addr);

    // 保存并应用更改
    if (!nm_remote_connection_commit_changes(connection, TRUE, NULL, &error)) {  // 修改为正确的参数
        if (error) {
            g_error_free(error);
        }
        return -1;
    }
    return 0;
}


// 获取文件属性
static int network_getattr(const char *path, struct stat *stbuf)
{
    memset(stbuf, 0, sizeof(struct stat));

    // 根目录
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    // network 目录
    if (strcmp(path, "/network") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }
    // service 目录
    if (strcmp(path,"/service") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    // LAN 文件
    if (strcmp(path, "/network/LAN") == 0) {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(lan_content);
        return 0;
    }

    // WiFi_sta 文件
    if (strcmp(path, "/network/WiFi_sta") == 0) {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(wifi_content);
        return 0;
    }

    return -ENOENT;
}

// 读取目录内容
static int network_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    // 未用到的参数，避免编译器警告
    (void) offset;
    (void) fi;

    // 添加 . 和 ..
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    // 根目录
    if (strcmp(path, "/") == 0) {
        filler(buf, "network", NULL, 0);
        return 0;
    }

    // network 目录
    if (strcmp(path, "/network") == 0) {
        filler(buf, "LAN", NULL, 0);
        filler(buf, "WiFi_sta", NULL, 0);
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

    // LAN 文件
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

    // WiFi_sta 文件
    if (strcmp(path, "/network/WiFi_sta") == 0) {
        content = get_device_ip("wlan0");
        len = strlen(content);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, content + offset, size);
        } else
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
    (void) offset;  // 我们忽略偏移，总是完整替换 IP

    // 确保输入的 IP 地址不会太长
    if (size >= sizeof(ip_str))
        return -EFBIG;

    // 复制并确保字符串结束
    memcpy(ip_str, buf, size);
    ip_str[size] = '\0';

    // 移除可能的换行符
    char *newline = strchr(ip_str, '\n');
    if (newline)
        *newline = '\0';

    // LAN 文件 - 设置 enp0s3 的 IP
    if (strcmp(path, "/network/LAN") == 0) {
        if (set_device_ip("enp0s3", ip_str) != 0)
            return -EIO;
        return size;
    }

    // WiFi_sta 文件 - 设置 wlan0 的 IP
    if (strcmp(path, "/network/WiFi_sta") == 0) {
        if (set_device_ip("wlan0", ip_str) != 0)
            return -EIO;
        return size;
    }

    return -ENOENT;
}

// 打开文件
static int network_open(const char *path, struct fuse_file_info *fi)
{
    if (strcmp(path, "/network/LAN") != 0 && 
        strcmp(path, "/network/WiFi_sta") != 0)
        return -ENOENT;

    return 0;
}

// 截断文件
static int network_truncate(const char *path, off_t size)
{
    // LAN 文件
    if (strcmp(path, "/network/LAN") == 0) {
        if (size >= sizeof(lan_content))
            return -EFBIG;
        lan_content[size] = '\0';
        return 0;
    }

    // WiFi_sta 文件
    if (strcmp(path, "/network/WiFi_sta") == 0) {
        if (size >= sizeof(wifi_content))
            return -EFBIG;
        wifi_content[size] = '\0';
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
    GError *error = NULL;

    // 初始化 NetworkManager 客户端
    nm_client = nm_client_new(NULL, &error);
    if (!nm_client) {
        fprintf(stderr, "Failed to create NM client: %s\n",
                error ? error->message : "unknown");
        if (error)
            g_error_free(error);
        return 1;
    }

    int ret = fuse_main(argc, argv, &network_oper, NULL);

    // 清理
    g_object_unref(nm_client);

    return ret;
}