#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>


//读取sda1的前16字节内容，以16进制显示
void read_test()
{
    int fd=open("/dev/sda1",O_RDONLY);
    if(fd<0)
    {
        perror("open /dev/sda failed!\n");
        return;
    }
    char buf[16];
    int n=read(fd,buf,sizeof(buf));
    if(n<0)
    {
        perror("read failed!\n");
        close(fd);
        return;
    }
    
    for(size_t i=0;i<n;++i)
    {
        printf("%2x ",(unsigned char)buf[i]);
    }
    printf("\n");
}
void ioctl_test()
{
    int fd;
    char *device = "/dev/input/event2"; 
    int led_bits;

    // 打开键盘
    fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the device");
        return ;
    }

    // 获取键盘LED 状态
    if (ioctl(fd, EVIOCGLED(sizeof(led_bits)), &led_bits) < 0) {
        perror("Error getting LED status");
        close(fd);
        return ;
    }

    // 打印键盘上三个LED 状态
    printf("LED status:\n");
    if (led_bits & (1 << LED_NUML)) {
        printf("Num Lock is ON\n");
    } else {
        printf("Num Lock is OFF\n");
    }
    if (led_bits & (1 << LED_CAPSL)) {
        printf("Caps Lock is ON\n");
    } else {
        printf("Caps Lock is OFF\n");
    }
    if (led_bits & (1 << LED_SCROLLL)) {
        printf("Scroll Lock is ON\n");
    } else {
        printf("Scroll Lock is OFF\n");
    }

    // 关闭输入设备
    close(fd);
    return ;
}
int main()
{
    //read_test();

    ioctl_test();
}