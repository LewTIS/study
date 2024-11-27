
#obj-m += my_pci_driver.o
#obj-m +=tasklet.o
#obj-m +=sim_interrupt.o

#obj-m +=mygpio_dev.o

# obj-m += my_workqueue.o
# obj-m += my_timer.o 
 obj-m += hello.o
# obj-m += mychardev_01.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


#-C: 这是make命令的一个选项，用于指定Makefile文件所在的目录。-C选项后面跟着的是要切换到的目录路径。
#这意呀着make命令将在指定的目录下执行，而不是在当前目录下。
#/lib/modules/$(uname -r)/build: 这是一个路径，其中$(uname -r)是一个命令替换，uname -r会输出当前运行的内核版本。因此，/lib/modules/$(uname -r)/build通常指向当前内核版本对应的源代码树的目录。
#这个目录包含了编译内核模块所需的所有头文件和配置文件。
#make命令会先将pwd命令的输出（即当前目录的路径）替换到M=后面，然后传递给make命令。这样，make就知道在哪里查找内核模块的源代码了。
#modules: 这是传递给make命令的目标。在这个上下文中，modules告诉make要构建的是内核模块
