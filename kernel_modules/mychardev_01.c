#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/cdev.h>
#include <linux/fs.h>



//在/dev/底下创建一个 字符设备 device node , 並且完整相關的IO操作

MODULE_LICENSE("GPL");
#define DEVICE_NAME "mychardev_01"
#define DEVICE_CLASS "mycharclass_01"

#define BUFFER_SIZE 1024
static char device_buffer[BUFFER_SIZE];//内核空间的数据缓冲区
static size_t buffer_index = 0;//当前缓冲区中实际数据长度
//1. define file_operartions and cdev structure 與對應的function
static int my_open(struct inode *inode,struct  file *file) {
    printk(KERN_INFO "%s: open\n",DEVICE_NAME);
    return 0;
}
static int my_release(struct inode *inode,struct file *file) {
    printk(KERN_INFO "%s: close\n",DEVICE_NAME);
    return 0;
}
static ssize_t my_read(struct file *file,char __user *buf,size_t len,loff_t *offset) {
    
    size_t bytes_to_read;
    //offset:文件当前读写位置
    printk(KERN_INFO "%s: read\n",DEVICE_NAME);
    
    //检查是否还有数据可读
    if(*offset >= buffer_index)
        return 0;//文件结束
    
    //实际可读取字节数
    bytes_to_read = min(len,buffer_index - *offset);

    //将数据拷贝到用户空间
    if(copy_to_user(buf,device_buffer + *offset,bytes_to_read))
    {
        printk(KERN_ALERT "%s: copy_to_user failed\n",DEVICE_NAME);
        return -EFAULT;
    }
    //更新偏移量
    *offset += bytes_to_read;

    printk(KERN_INFO "%s: read %zu bytes\n", DEVICE_NAME,bytes_to_read);
    return bytes_to_read;
}
static ssize_t my_write(struct file *file,const char __user *buf,size_t len,loff_t *offset) {
    
    size_t bytes_to_write;
    printk(KERN_INFO "%s: write\n",DEVICE_NAME);
    

    //检查缓冲区空间
    bytes_to_write = min(len,(size_t)(BUFFER_SIZE - *offset));
    if(bytes_to_write == 0)
    {
        printk(KERN_ALERT "%s: buffer full\n",DEVICE_NAME);
        return -ENOSPC;
    }

    if(copy_from_user(device_buffer + *offset,buf,bytes_to_write))
    {
        printk(KERN_ALERT "%s: copy_from_user failed\n",DEVICE_NAME);
        return -EFAULT;
    }

    *offset += bytes_to_write;
    buffer_index = max(buffer_index,(size_t)*offset);//更新实际数据长度

    printk(KERN_INFO "%s: write %zu bytes\n", DEVICE_NAME,bytes_to_write);
    return bytes_to_write;
}

static loff_t my_llseek(struct file *file, loff_t offset, int whence) {
    loff_t newpos;
    switch(whence) {
        case SEEK_SET:
            newpos = offset;
            break;
        case SEEK_CUR:
            newpos = file->f_pos+offset;
            break;
        case SEEK_END:
            newpos = buffer_index + offset;
            break;
        default:
            return -EINVAL;
    }
    if(newpos < 0)
        return -EINVAL;
    if(newpos > BUFFER_SIZE)
        newpos = BUFFER_SIZE;
    file->f_pos = newpos;
    return newpos;
}


//定義了驅動程式中處理各種文件操作的函數
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
    .llseek = my_llseek,
};
static dev_t dev_num;//设备号变量 高位-主设备号 低位-次设备号
static struct cdev my_cdev;//字符设备在内核中的表示
static struct class *cdevdev_class=NULL;//设备类。指向新创建的 struct class 结构
//2.Assign major/minor numbers for device node
static int __init my_init(void) {
    int ret;
    
    //动态分配设备号
    ret=alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret < 0) {
        printk(KERN_ALERT "%s: failed to allocate major number\n",DEVICE_NAME);
        return ret;
    }

    printk(KERN_INFO "Allocated major device number: %d\n", MAJOR(dev_num));

        
    //3.Initialize the cdev Structure and Add It to the Kernel
    //void cdev_init(struct cdev *cdev, struct file_operations *fops);
    cdev_init(&my_cdev,&fops);//字符设备初始化
    my_cdev.owner = THIS_MODULE;//实现该字符设备驱动的模块

    //int cdev_add(struct cdev *dev, dev_t num, unsigned int count)
    ret = cdev_add(&my_cdev,dev_num,1);//将该字符设备添加到内核
    if(ret < 0) {
        unregister_chrdev_region(dev_num,1);
        printk(KERN_ALERT "mychardev:failed to add cdev\n");
        return ret;
    }
    printk(KERN_ALERT "%s: cdev_add \n",DEVICE_NAME);

    //4.Device node creation
    //create class
    cdevdev_class = class_create(THIS_MODULE,DEVICE_CLASS); //创建设备类，在 /sys/class/ 中 create
    if(IS_ERR(cdevdev_class)) {
        printk(KERN_ERR "failed to create device class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num,1);
        return PTR_ERR(cdevdev_class);
    }
    printk(KERN_INFO "%s: class_create /sys/class/%s \n",DEVICE_NAME,DEVICE_CLASS);

    //create device node
    //创建设备节点，并将该设备节点与指定的设备类（通过 class 参数传入）相关联
    //struct device *device_create(struct class *class, struct device *parent,  
    //                         dev_t devt, void *drvdata, const char *fmt,...);

    if(device_create(cdevdev_class,NULL,dev_num,NULL,DEVICE_NAME) == NULL) {
        printk(KERN_ERR "failed to create device\n");
        device_destroy(cdevdev_class,dev_num);
        class_destroy(cdevdev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num,1);
        return -1;
    }
    printk(KERN_INFO "%s:  device_create \n", DEVICE_NAME );
    return 0;
}
static void __exit my_exit(void) {

    device_destroy(cdevdev_class,dev_num);
    class_destroy(cdevdev_class);
    cdev_del(&my_cdev);//从内核中删除cdev
    unregister_chrdev_region(dev_num,1);//注销设备号

    printk(KERN_INFO "%s:module unloaded\n",DEVICE_NAME);
}


module_init(my_init);        
module_exit(my_exit);