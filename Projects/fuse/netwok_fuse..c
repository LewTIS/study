#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <libnm/nm-client.h>

#define MAX_FILES 10

typedef struct {
    char *name;
    char *content;
    size_t size;
} File;

File files[MAX_FILES];
int file_count = 0;

// 获取文件属性
static int hello_getattr(const char *path, struct stat *stbuf)
{
    memset(stbuf, 0, sizeof(struct stat));

    // 根目录
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2 + file_count;  // 根目录 + "." + ".." + 文件数量
        return 0;
    }

    // network 目录
    if (strcmp(path, "/network") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2 + 2;  // network 目录 + "." + ".." + LAN + WiFi_sta
        return 0;
    }

    // hello.txt 文件
    if (strcmp(path, "/hello.txt") == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen("Hello World!\n");
        return 0;
    }

    // 其他文件
    for (int i = 0; i < file_count; i++) {
        if (strcmp(path + 1, files[i].name) == 0 || strcmp(path + 9, files[i].name) == 0) {
            stbuf->st_mode = S_IFREG | 0644;
            stbuf->st_nlink = 1;
            stbuf->st_size = files[i].size;
            return 0;
        }
    }

    return -ENOENT;
}

// 读取目录内容
static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    if (strcmp(path, "/") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, "hello.txt", NULL, 0);
        filler(buf, "network", NULL, 0);
        return 0;
    }

    if (strcmp(path, "/network") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, "LAN", NULL, 0);
        filler(buf, "WiFi_sta", NULL, 0);
        return 0;
    }

    return -ENOENT;
}

// 读取文件内容
static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    const char *hello_str = "Hello World!\n";
    size_t len = strlen(hello_str);

    if (strcmp(path, "/hello.txt") == 0) {
        if (offset >= len)
            return 0;
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
        return size;
    }

    for (int i = 0; i < file_count; i++) {
        if (strcmp(path + 1, files[i].name) == 0 || strcmp(path + 9, files[i].name) == 0) {
            if (offset >= files[i].size)
                return 0;
            if (offset + size > files[i].size)
                size = files[i].size - offset;
            memcpy(buf, files[i].content + offset, size);
            return size;
        }
    }

    return -ENOENT;
}

// 创建文件
static int hello_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    if (file_count >= MAX_FILES)
        return -ENOSPC;

    files[file_count].name = strdup(path + 1);
    files[file_count].content = malloc(1);  // 初始分配一个字节
    files[file_count].size = 0;
    file_count++;

    fi->fh = file_count - 1;  // 文件句柄

    return 0;
}

// 写入文件内容
static int hello_write(const char *path, const char *buf, size_t size, off_t offset,
                       struct fuse_file_info *fi)
{
    int file_index = fi->fh;

    if (file_index < 0 || file_index >= file_count)
        return -EBADF;

    if (offset + size > files[file_index].size) {
        files[file_index].content = realloc(files[file_index].content, offset + size);
        if (!files[file_index].content)
            return -ENOMEM;
        files[file_index].size = offset + size;
    }

    memcpy(files[file_index].content + offset, buf, size);

    // 设置 IP 地址
    if (strcmp(path, "/network/LAN") == 0) {
        set_ip_address("eth1", buf);
    } else if (strcmp(path, "/network/WiFi_sta") == 0) {
        set_ip_address("wlan0", buf);
    }

    return size;
}

// 释放文件描述符
static int hello_release(const char *path, struct fuse_file_info *fi)
{
    // 释放文件描述符时可以执行清理操作
    return 0;
}

// 获取 IP 地址
static int get_ip_address(const char *interface, char *buf, size_t size)
{
    NMClient *client;
    GError *error = NULL;
    const GPtrArray *devices;
    int i;

    client = nm_client_new(NULL, &error);
    if (error) {
        fprintf(stderr, "Failed to create NMClient: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    devices = nm_client_get_devices(client);
    for (i = 0; i < devices->len; i++) {
        NMDevice *device = g_ptr_array_index(devices, i);
        const char *dev_iface = nm_device_get_iface(device);

        if (g_strcmp0(dev_iface, interface) == 0) {
            const GPtrArray *ips = nm_device_ip4_config_get_addresses(nm_device_get_ip4_config(device, 0));
            if (ips && ips->len > 0) {
                NMIPAddress *ip = g_ptr_array_index(ips, 0);
                const char *ip_address = nm_ip_address_get_address(ip);
                strncpy(buf, ip_address, size);
                g_object_unref(client);
                return 0;
            }
        }
    }

    g_object_unref(client);
    return -1;
}

// 设置 IP 地址
static int set_ip_address(const char *interface, const char *ip)
{
    NMClient *client;
    GError *error = NULL;
    const GPtrArray *devices;
    int i;

    client = nm_client_new(NULL, &error);
    if (error) {
        fprintf(stderr, "Failed to create NMClient: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    devices = nm_client_get_devices(client);
    for (i = 0; i < devices->len; i++) {
        NMDevice *device = g_ptr_array_index(devices, i);
        const char *dev_iface = nm_device_get_iface(device);

        if (g_strcmp0(dev_iface, interface) == 0) {
            NMConnection *connection;
            NMSettingConnection *s_con;
            NMSettingIP4Config *s_ip4;
            char *uuid;
            GVariantBuilder ip4_addresses;
            GVariant *ip4_addresses_variant;

            connection = nm_simple_connection_new();
            s_con = nm_setting_connection_new();
            nm_connection_add_setting(connection, NM_SETTING(s_con));

            uuid = nm_utils_uuid_generate();
            g_object_set(s_con,
                         NM_SETTING_CONNECTION_ID, interface,
                         NM_SETTING_CONNECTION_UUID, uuid,
                         NM_SETTING_CONNECTION_TYPE, NM_SETTING_IP4_CONFIG_SETTING_NAME,
                         NULL);
            g_free(uuid);

            s_ip4 = nm_setting_ip4_config_new();
            nm_connection_add_setting(connection, NM_SETTING(s_ip4));

            g_variant_builder_init(&ip4_addresses, G_VARIANT_TYPE("aau"));
            g_variant_builder_add(&ip4_addresses, "(uua)",
                                  g_inet_addr_new_from_string(ip)->inet4, 24, 0);
            ip4_addresses_variant = g_variant_builder_end(&ip4_addresses);

            g_object_set(s_ip4,
                         NM_SETTING_IP4_CONFIG_METHOD, NM_SETTING_IP4_CONFIG_METHOD_MANUAL,
                         NM_SETTING_IP4_CONFIG_ADDRESSES, ip4_addresses_variant,
                         NM_SETTING_IP4_CONFIG_GATEWAY, "0.0.0.0",
                         NULL);

            nm_client_add_and_activate_connection(client, connection, device, NULL, NULL, NULL, &error);
            if (error) {
                fprintf(stderr, "Failed to set IP address: %s\n", error->message);
                g_error_free(error);
                g_object_unref(client);
                return -1;
            }

            g_object_unref(client);
            return 0;
        }
    }

    g_object_unref(client);
    return -1;
}

// 初始化函数
static void *hello_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    (void) conn;
    cfg->kernel_cache = 1;  // 启用内核缓存
    return NULL;
}

// FUSE 操作结构体
static struct fuse_operations hello_oper = {
    .init = hello_init,
    .getattr = hello_getattr,
    .readdir = hello_readdir,
    .read = hello_read,
    .create = hello_create,
    .write = hello_write,
    .release = hello_release,
};

int main(int argc, char *argv[])
{
    // 初始化网络文件
    files[file_count].name = strdup("network/LAN");
    files[file_count].content = malloc(1);  // 初始分配一个字节
    files[file_count].size = 0;
    file_count++;

    files[file_count].name = strdup("network/WiFi_sta");
    files[file_count].content = malloc(1);  // 初始分配一个字节
    files[file_count].size = 0;
    file_count++;

    return fuse_main(argc, argv, &hello_oper, NULL);
}