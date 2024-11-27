#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

//定义tasklet结构体
static struct tasklet_struct my_tasklet;

//tasklet处理函数
void my_tasklet_handler(unsigned long data)
{
    printk(KERN_INFO "Tasklet handler is running on cpu %d\n",smp_processor_id());

    mdelay(1000);
    printk(KERN_INFO "Tasklet handler finished\n");
}

static int __init my_init(void)
{

    //初始化 tasklet
    tasklet_init(&my_tasklet,my_tasklet_handler,0);
    
    //模拟中断处理
    printk(KERN_INFO "Simulating interrupt and scheduling tasklet\n");
    tasklet_schedule(&my_tasklet);
    
    //再次调度tasklet
    printk(KERN_INFO "Scheduling tasklet again\n");
    tasklet_schedule(&my_tasklet);
    return 0;
}

static void __exit my_exit(void)
{
    //取消tasklet
    tasklet_kill(&my_tasklet);

    printk(KERN_INFO "Module exited!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");

