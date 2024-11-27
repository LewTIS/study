#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define TIMER_INTERVAL 5
//中断处理函数
static irqreturn_t my_interrupt_handler(int irq,void *dev_id)
{
    printk(KERN_INFO "Interrupt received on IRQ %d\n",irq);

    return IRQ_HANDLED;
}

static struct timer_list my_timer;//定义一个定时器

//用定时器模拟产生中断
void trigger_device_interrupt(struct timer_list *timer)
{
    printk(KERN_INFO "simulated interrupt triggered\n");
    mod_timer(&my_timer,jiffies + TIMER_INTERVAL * HZ );//每5秒触发一次
}
int default_irq=23;
int actual_irq;
static int __init my_init(void)
{
    int irq=0;
    unsigned long flags;
    
    int ret;

    //请求默认中断线
    ret = request_irq(default_irq,my_interrupt_handler,IRQF_SHARED,"my_driver",NULL);
    if(ret) {
        printk(KERN_ERR "Failed to request IRQ %d, trying fallback\n",default_irq);

        //尝试回退到其他中断线
        
        for(irq=0; irq < NR_IRQS; irq++) {
            if(irq != default_irq) {
                ret = request_irq(irq,my_interrupt_handler,IRQF_SHARED,"my_driver",NULL);
                if(!ret) {
                    printk(KERN_INFO "Using fallback IRQ %d\n",irq);
                    default_irq = irq;
                    break;
                }
            }
        }
        if(ret) {
            printk(KERN_ERR "Failed to request any vaild IRQ\n");
            return -ENODEV;
        }
    }
    //启用中断探测模式
    flags = probe_irq_on();

    //设置定时器模拟中断
    timer_setup(&my_timer,trigger_device_interrupt,0);
    mod_timer(&my_timer,jiffies + TIMER_INTERVAL * HZ );

    //使用中断探测模式并获取探测到的终端号
    actual_irq = probe_irq_off(flags);

    if(actual_irq > 0)
    {
        printk(KERN_INFO "Detected actual IRQ:%d\n",actual_irq);
        if(actual_irq != default_irq){
            printk(KERN_WARNING "Actual IRQ %d does not match requested IRQ %d\n",actual_irq,default_irq);
        }
    }else if(actual_irq == 0) {
        printk(KERN_ERR "No IRQ detected\n");
    }else{
        printk(KERN_ERR "Failed to detect IRQ, error: %d\n", actual_irq);
    }
    return 0;
}

static void __exit my_exit(void)
{

    del_timer(&my_timer);//删除定时器
    free_irq(default_irq,NULL);
    printk(KERN_INFO "my driver exited\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
