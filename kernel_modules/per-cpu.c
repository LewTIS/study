#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/percpu.h>
#include <linux/smp.h>
#include <linux/interrupt.h>

//定义一个per-cpu变量
static DEFINE_PER_CPU(int,my_percpu_var);

static void work_on_percpu_var(void)
{
    int cpu;
    unsigned long flags;

    local_irq_save(flags);
    
}

static int __init my_module_init(void)
{
    int cpu;
    printk(KERN_INFO "my_module init\n");

    for_each_possible_cpu(cpu)
    {
        per_cpu(my_percpu_var,cpu) = 0;
    }

    //模拟对per-cpu变量的操作
    work_on_percpu_var();
    return 0;
}