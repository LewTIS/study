#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/delay.h>


static struct workqueue_struct *my_wq;//定义一个工作队列
static struct work_struct my_work1;//定义工作项1
static struct work_struct my_work2;//工作项2

//my_work1处理函数
static void my_work_handler1(struct work_struct *work)
{
    printk(KERN_INFO "Workqueue handler 1 executed!\n");
    msleep(2000);//模拟工作项1执行时间
}

//my_work2处理函数
static void my_work_handler2(struct work_struct *work)
{
    printk(KERN_INFO "Workqueue handler 2 executed!\n");
}

static int __init my_workqueue_init(void)
{
    printk(KERN_INFO "my_workqueue module init!\n");

    my_wq = create_singlethread_workqueue("my_singlethread_wq");//创建一个单线程工作队列,名称叫做my_singlethread_wq
    if(!my_wq)
    {
        printk(KERN_INFO "Failed to create workqueue!\n");
        return -ENOMEM;
    }

    INIT_WORK(&my_work1,my_work_handler1);//初始化工作项1
    INIT_WORK(&my_work2,my_work_handler2);//初始化工作项2

    queue_work(my_wq,&my_work1);//将工作项1加入到工作队列
    queue_work(my_wq,&my_work2);
    
    return 0;
}

static void  __exit my_workqueue_exit(void)
{
    printk(KERN_INFO "my workqueue module exit!\n");

    flush_workqueue(my_wq);//等待工作队列中的所有工作项执行完

    destroy_workqueue(my_wq);//销毁工作队列
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simple workqueue module!\n");
