#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/interrupt.h>

//定义支持的PCI设备ID表
static const struct pci_device_id my_pci_ids[]={
    { PCI_DEVICE(PCI_VENDOR_ID_INTEL,0x1234) },//intel id, 设备id 0x1234
    { PCI_DEVICE(PCI_VENDOR_ID_INTEL,0x1235) },
    {0,}//终止符，表示列表结束
};
//将设备ID表注册到内核
MODULE_DEVICE_TABLE(pci,my_pci_ids);

//中断处理函数
static irqreturn_t my_interrupt_handler(int irq,void *dev_id)
{
    struct pci_dev *dev = (struct pci_dev *)dev_id;

    printk(KERN_INFO "my_pci_driver:Interrupt received on device %04x:%04x\n",dev->vendor,dev->device);
    return IRQ_HANDLED;
}

//探测函数，当内核检测到匹配的PCI设备时调用
static int my_pci_probe(struct pci_dev *dev,const struct pci_device_id *id)
{
    int ret;
    unsigned long io_base;
    void __iomem *io_addr;

    printk(KERN_INFO "my_pci_probe:Found device with ID %04x:%04x\n",dev->vendor,dev->device);

    //启用PCI设备
    ret = pci_enable_device(dev);
    if(ret)
    {
        printk(KERN_ERR "my_pci_probe:Failed to enable PCI device\n");
        return ret;
    }

    //设备PCI设备的DMA属性
    //pci_set_dma_mask用于设置PCI设备的DMA掩码
    //DMA_BIT_MASK宏用于生成一个DMA掩码，该掩码允许设备访问特定数量的地址线
    ret = pci_set_dma_mask(dev,DMA_BIT_MASK(64)); 
    if(ret)
    {
        ret = pci_set_dma_mask(dev,DMA_BIT_MASK(32));
        if(ret)
        {
            printk(KERN_ERR "my_pci_probe:No suitable DMA available\n");
            goto err_disable_device;
        }
    }

    //请求I/O端口资源
    ret = pci_request_regions(dev, "my_pci_driver");
    if(ret)
    {
        printk(KERN_ERR "my_pci_probe:Failed to request I/O regions\n");
        goto err_disable_device;
    }
    //获取I/O端口基地址
    io_base = pci_resource_start(dev,0);
    if(!io_base)
    {
        printk(KERN_ERR "my_pci_probe:No I/O region found\n");
        goto err_release_regions;
    }

    io_addr = ioremap(io_base,pci_resource_len(dev,0));
    if(!io_addr)
    {
        printk(KERN_ERR "my_pci_probe: Failed to map I/O address\n");
        goto err_release_regions;
    }


    ret = request_irq(dev->irq,my_interrupt_handler,IRQF_SHARED,"my_pci_driver",dev);
    if(ret)
    {
        printk(KERN_ERR "my_pci_probe:Failed to request IRQ\n");
        goto err_unmap_io;
    }

    return 0;

err_unmap_io:
    iounmap(io_addr);
err_release_regions:
    pci_release_regions(dev);
err_disable_device:
    pci_disable_device(dev);

    return ret;

}

//移除函数，当设备被卸载时调用
static void my_pci_remove(struct pci_dev *dev)
{

    free_irq(dev->irq,dev);

    iounmap((void __iomem *)pci_resource_start(dev,0));

    pci_release_regions(dev);

    pci_disable_device(dev);

}

//定义PCI驱动程序结构体
static struct pci_driver my_pci_driver = {
    .name = "my_pci_driver",
    .id_table = my_pci_ids,
    .probe = my_pci_probe,
    .remove = my_pci_remove,
};

static int __init my_pci_init(void)
{
    int ret;
    //注册PCI驱动程序
    ret = pci_register_driver(&my_pci_driver);
    if(ret)
    {
        printk(KERN_ERR "my_pci_init:Failed  to register PCI driver\n");
        return ret;
    }
    printk(KERN_INFO "my_pci_init:PCI driver register successfully\n");
    return 0;
}

static void __exit my_pci_exit(void)
{
    pci_unregister_driver(&my_pci_driver);
    printk(KERN_INFO "my_pci_exit: PCI driver unregistered successfully\n");
}

module_init(my_pci_init);
module_exit(my_pci_exit);

MODULE_LICENSE("GPL");