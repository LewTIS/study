#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/ktime.h> 
#include <linux/timekeeping.h> 

/*
// jiffers:内核中的时间单位，记录了系统开机以来的滴答数，每发生一次时钟中断，jiffies加1
struct timer_list { //内核定时器结构体
    struct list_head entry;  //用于将定时器加入到定时器链表中
    unsigned long expires;  //定时器的到期时间(以jiffies为单位)
    void (*function)(struct timer_list *); //定时器到期要调用的回调函数
    unsigned int flags;  //定时器的标志位
}
*/

#define TIMER_INTERVAL 5 //定时器的时间间隔

static struct timer_list my_timer;//定义一个定时器

void my_timer_callback(struct timer_list *timer);
//定时器回调函数 打印当前时间
void my_timer_callback(struct timer_list *timer)
{
    struct timespec64 time;//表示时间的结构体，tv_sec表示秒，tv_usec表示微秒
    struct tm brkoen;//表示时间的结构体，tm_sec表示秒，tm_min表示分，tm_hour表示时，tm_mday表示日，tm_mon表示月，tm_year表示年
    long offset_seconds = 8 * 60 * 60;//时区偏移量

    ktime_get_real_ts64(&time);//内核函数，用于获取当前时间，并将其存储在struct timeval结构体中
    time.tv_sec += offset_seconds;//加上时区偏移量
    time64_to_tm(time.tv_sec,0,&brkoen);//时间转换函数，将struct timespec64结构体中的时间转换为struct tm结构体中的时间

    printk(KERN_INFO "Current time: %04ld-%02d-%02d %02d:%02d:%02d\n",
            brkoen.tm_year + 1900,//需要加上1900表示实际的年份
            brkoen.tm_mon + 1, //这里tm_mon表示的是0-11，所以需要加1表示实际的月份
            brkoen.tm_mday,
            brkoen.tm_hour,
            brkoen.tm_min,
            brkoen.tm_sec);
    //重新启动定时器--周期性触发
    //HZ是内核中的一个宏，表示每秒的时钟中断次数，这里表示TIMER_INTERVAL秒后再次触发定时器
    //当前系统HZ=1000，所以TIMER_INTERVAL * HZ = 5 * 1000 = 5000ms = 5s
    mod_timer(&my_timer,jiffies + TIMER_INTERVAL * HZ);
}

static int __init my_timer_init(void)
{
    printk(KERN_INFO "my_timer module init\n");

    timer_setup(&my_timer,my_timer_callback,0);//初始化定时器

    mod_timer(&my_timer,jiffies + TIMER_INTERVAL * HZ);//启动定时器

    return 0;
}

static void __exit my_timer_exit(void)
{
    printk(KERN_INFO "my_timer module exit\n");

    del_timer(&my_timer);//删除定时器
}
module_init(my_timer_init);
module_exit(my_timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("my_timer");
MODULE_DESCRIPTION("A simple timer module");