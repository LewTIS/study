#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#define GPIO_PIN 17
#define DEVICE_NAME "mygpio_dev"

static int irq_number;
static int irq_counter = 0;

//中断处理函数
static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
    irq_counter++;
    printk(KERN_INFO "GPIO %d: Interrupt occurred %d times\n", GPIO_PIN, irq_counter);
    return IRQ_HANDLED;
}

//模拟触发中断的函数
static void simulate_gpio_interrupt(void)
{
    //模拟上升沿信号
    gpio_set_value(GPIO_PIN,0);//设置GPIO引脚为低电平
    mdelay(10);
    gpio_set_value(GPIO_PIN,1);
}

//初始化模块
static int __init my_init(void)
{
    int ret;
    printk(KERN_INFO "Initializing mygpio_dev module\n");

    if(!gpio_is_valid(GPIO_PIN))
    {
        printk(KERN_ALERT "Invalid GPIO %d\n", GPIO_PIN);
        return -ENODEV;
    }
    ret = gpio_request(GPIO_PIN,DEVICE_NAME);
    if(ret)
    {
        printk(KERN_ALERT "Failed to request GPIO %d,error %d\n", GPIO_PIN,ret);
        return ret;
    }

    //调整GPIO为输出模式
    gpio_direction_output(GPIO_PIN,0);

    //获取IRQ号
    irq_number = gpio_to_irq(GPIO_PIN);
    if(irq_number < 0)
    {
        printk(KERN_ALERT "Failed to get IRQ number for GPIO %d,error %d\n", GPIO_PIN,irq_number);
        gpio_free(GPIO_PIN);
        return irq_number;
    }

    //请求IRQ
    ret = request_irq(irq_number,my_irq_handler,IRQF_TRIGGER_RISING,DEVICE_NAME,NULL);
    if(ret)
    {
        printk(KERN_ALERT "Failed to request IRQ %d,error %d\n", irq_number,ret);
        gpio_free(GPIO_PIN);
        return ret;
    }
    printk(KERN_INFO "Requested IRQ %d for GPIO %d\n",irq_number,GPIO_PIN);

    //模拟触发中断
    simulate_gpio_interrupt();

    return 0;
}

static void __exit my_exit(void)
{
    free_irq(irq_number,NULL);
    gpio_free(GPIO_PIN);
    printk(KERN_INFO "Freed IRQ %d and GPIO %d\n",irq_number,GPIO_PIN);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");