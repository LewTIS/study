#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include  <linux/slab.h> 
MODULE_LICENSE("GPL");

static char *whom="world";  //模块参数定义
static int howmany=1;
module_param(howmany,int,0664);//宏，模块参数注册
module_param(whom,charp,0664);


static struct kobject *hello_kobj;

//show和store函数是用于处理通过sysfs接口导出的kobject属性的读取和写入操作的回调函数。
//show函数返回whom和howmany的值，store函数更新whom和howmany的值。
static ssize_t hello_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
    return snprintf(buf, PAGE_SIZE, "whom:%s howmany:%d\n", whom, howmany);
}
static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t len)
{
    char *input;
    char *whom_str;
    char *input_ptr;
    int new_howmany;//存储解析出的howmany值

    if(len >= PAGE_SIZE)
        return -EINVAL;
    
    input = kmalloc(len + 1, GFP_KERNEL);
    if(!input)
    {
        return -ENOMEM;
    }
    
    strncpy(input, buf, len);
    input[len] = '\0';

    //解析输入数据
    input_ptr=input;
    whom_str = strsep(&input_ptr, " ");
    if(!whom_str || !*whom_str)
    {
        kfree(input);
        return -EINVAL;
    }
    if(kstrtoint(input_ptr, 10, &new_howmany))//内核函数，字符串转换为整数
    {
        kfree(input);
        return -EINVAL;
    }
    //更新模块参数
    strncpy(whom,whom_str,PAGE_SIZE);
    whom[PAGE_SIZE - 1] = '\0';
    howmany = new_howmany;

    kfree(input);
    return len;
}
static struct kobj_attribute hello_attr=__ATTR(hello,0664,hello_show,hello_store);

static int __init hello_init(void)
{
    int ret;
    int i=0;
    //创建一个新的 kobject 实例，并将其添加到内核的 kobject 层次结构中
    //失败返回NULL
    hello_kobj=kobject_create_and_add("hello_module",kernel_kobj);
    if(!hello_kobj)
    {
        printk(KERN_ERR "Failed to create kobject!\n");
        return -ENOMEM;
    }

    //在sysfs文件系统中为特定的kobject创建一个新的文件
    //成功返回0
    ret=sysfs_create_file(hello_kobj,&hello_attr.attr);
    if (ret) {  
        printk(KERN_ERR "Failed to create sysfs file\n");  
        kobject_put(hello_kobj);     
        return ret;  
    }  

    printk(KERN_INFO "Hello module loaded\n");
    for(;i<howmany;i++){
        printk(KERN_ALERT "Hello,%s\n",whom);
    }
    return 0;
}

static void __exit hello_exit(void)
{
    //删除之前通过sysfs_create_file创建的sysfs文件
    sysfs_remove_file(hello_kobj,&hello_attr.attr);
    kobject_put(hello_kobj);
    printk(KERN_ALERT "Goodbye,cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);