#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

void listen_device(const char *dev,int timeout)
{
    int retval;
    fd_set readfds;
    struct timeval tv;

    int fd=open(dev,O_RDONLY);
    
    struct input_event event;

    if(fd<0)
    {
        perror(dev);
        return;
    }
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd,&readfds);
        tv.tv_sec=timeout;
        tv.tv_usec=0;
        if((retval=select(fd+1,&readfds,NULL,NULL,&tv))==1)
        {
            if(read(fd,&event,sizeof(event))==sizeof(event))
            {
                if(event.type == EV_KEY)
                {
                    if(event.value==0 || event.value==1)
                    {
                        printf("key %d %s\n",event.code,event.value ? "Pressed":"Released");
                    }
                }
                else
                {
                    printf("type=%x %d %d\n", event.type, event.code, event.value);
                }
            }
        }
        else{
            break;
        }
    }
    close(fd);
}
void listen_keyboard(int timeout)
{
    listen_device("/dev/input/event2", timeout);
}
/*
int main(int argc, char **argv)
{
    listen_keyboard(20);
    printf("keyboard timeout\n");
    return 0;
}
*/
//读取键盘事件数据
void test()
{
    int fd=open("/dev/input/event2",O_RDONLY);
    int n;
    char buf[1024]={0};
    if(fd<0)
    {
        perror("open /dev/input/event2 failed!\n");
        close(fd);
        return;
    }
  
    for(int i=0;i<3;++i)
    {
        n=read(fd,buf,1024);
        if(n<0)
        {
            perror("read error!\n");
            close(fd);
            return;
        }  
        printf("%s ",buf);
  
    }
    
    close(fd);
}
void test2()
{
    char buf[256]={0};
    int n;
    int fd=open("/dev/ttyS0",O_RDWR|O_NOCTTY);
    if(fd<0)
    {
        perror("open /dev/ttyS0 failed!\n");
        return;
    }
    struct termios options;
    tcgetattr(fd,&options);
    options.c_cflag |=(CLOCAL|CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;
    tcsetattr(fd,TCSANOW,&options);
    /*
    int m=write(fd,w_buf,strlen(w_buf));
    if(m<0)
    {
        perror("write error!\n");
        close(fd);
        return;
    }
    */
    while(1)
    {
        n=read(fd,buf,sizeof(buf)-1);
        if(n>0)
        {
            printf("read data:%s\n",buf);
        }
    }
    close(fd);

}
//读取sda1的前16字节内容，以16进制显示
void test3()
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
void io_test()
{
    int fd;  
    char name[128] = {0};  
    unsigned int len = sizeof(name);  
  
    
    // 打开设备文件  
    fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);  
    if (fd < 0) {  
        perror("Failed to open device");  
        return;
    }  
  
    // 使用ioctl获取设备名称  
    if (ioctl(fd, EVIOCGNAME(len), name) < 0) {  
        perror("Failed to get device name");  
        close(fd);  
        return;
    }  
  
    // 打印设备名称  
    printf("Device name: %s\n", name);  
  
    // 关闭设备文件  
    close(fd);  
    return ;
}
void ioctl_test()
{
    int fd;
    char *device = "/dev/input/event2"; 
    int led_bits;

    // 打开输入设备
    fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the device");
        return ;
    }

    // 获取 LED 状态
    if (ioctl(fd, EVIOCGLED(sizeof(led_bits)), &led_bits) < 0) {
        perror("Error getting LED status");
        close(fd);
        return ;
    }

    // 打印 LED 状态
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
void ioctl_test2()
{
    int fd;
    int switch_states[100]; // 假设有足够的空间存储所有开关状态
    int len = sizeof(switch_states);

    // 打开输入设备
    fd = open("/dev/input/event2", O_RDONLY);
    if (fd < 0) {
        perror("Error opening the device");
        return ;
    }

    // 获取开关状态
    if (ioctl(fd, EVIOCGSW(len), &switch_states) < 0) {
        perror("Error getting switch states");
        close(fd);
        return ;
    }

    // 打印开关状态
    printf("Switch states:\n");
    // 遍历所有可能的开关状态
    for (int i = 0; i <100; ++i) {
        if (switch_states[i] == 1) {
            printf("Switch %d is ON\n", i);
        } else if (switch_states[i] == 0) {
            printf("Switch %d is OFF\n", i);
        }
    }

    // 关闭输入设备
    close(fd);
    return ;
}
const char* input_prop_to_str(int prop) {
    switch (prop) {
        case INPUT_PROP_POINTER: return "POINTER";
        case INPUT_PROP_DIRECT: return "DIRECT";
        case INPUT_PROP_BUTTONPAD: return "BUTTONPAD";
        case INPUT_PROP_SEMI_MT: return "SEMI_MT";
        case INPUT_PROP_TOPBUTTONPAD: return "TOPBUTTONPAD";
        case INPUT_PROP_POINTING_STICK: return "POINTING_STICK";
        case INPUT_PROP_ACCELEROMETER: return "ACCELEROMETER";
        // 添加更多属性对应的字符串
        default: return "UNKNOWN";
    }
}

void io_test2() {
    int fd;
    int prop[EV_MAX * (INPUT_PROP_MAX / sizeof(int))]; // 存储所有属性
    int len = sizeof(prop);

    // 打开输入设备
    fd = open("/dev/input/event2", O_RDONLY); // 替换 eventX 为实际的事件号
    if (fd < 0) {
        perror("Error opening the device");
        return ;
    }

    // 获取设备属性
    if (ioctl(fd, EVIOCGPROP(len), &prop) < 0) {
        perror("Error getting device properties");
        close(fd);
        return ;
    }

    // 打印设备属性
    printf("Device properties:\n");
    for (int i = 0; i < EV_MAX; ++i) {
        for (int j = 0; j < (INPUT_PROP_MAX / sizeof(int)); ++j) {
            int prop_value = prop[i * (INPUT_PROP_MAX / sizeof(int)) + j];
            if (prop_value != 0) {
                printf("%s: %d\n", input_prop_to_str(prop_value), prop_value);
            }
        }
    }

    // 关闭输入设备
    close(fd);
    return ;
}

int main()
{
    io_test2();
    //ioctl_test2();
    //ioctl_test();
    //test3();
    return 0;
}
