#include <unistd.h> //open close的头文件
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>



/*
int open(const char *pathname,int flags);
int open(const hcar *pathname,int flags,mode_t mode);
int close(int fd);
参数：文件路径、读写方式、权限设置(一般O_CREAT,权限用8进制，如0664)
flag参数：
O_RDONLY:只读
O_WRONLY:只写
O_RDWR:读写
O_APPEND:追加
O_CREAT:文件存在就使用，不存在就创建
O_EXCL:文件不存在就创建，存在则返回错误信息
O_TRUNC:文件截断为0
O_NONBLOCK:非阻塞的方式操作
open 中mode的权限并不是文件真正权限，真正权限=mode&(~umask) 
如：mode=777，umask=002 ，~umask=755, 真正权限为777&755=755
*/

void test1()
{
    int fd=open("./demo.txt",O_RDWR | O_CREAT);
    printf("fd=%d\n",fd);

    if(fd ==-1)//打开文件失败
    {
        //出错会产生一个errno，对应不同的错误细节
        printf("open failed!!!\n");
        printf("errno=%d\n",errno);
    }

    close(fd);
    return;
}
void test2()
{
    char buf[1024];
    int ret=0;
    int fd=open("./dict.txt",O_RDWR | O_CREAT,0644);
    if(-1==fd)
    {
        printf("open file failed!!!\n");
        printf("errno=%d\n",errno);
        return;
    }
    printf("fd=%d\n",fd);
    int count=0;
    while((ret=read(fd,buf,sizeof(buf)))!=0)
    {
        count++;
        write(STDOUT_FILENO,buf,ret);
    }
    printf("%d\n",count);
    close(fd);
}
/*
通过fgetc、fputc实现文件copy功能
int  fgetc（FILE *stream)  
返回值为：以无符号char类型强制转换为int类型返回读取到字符,到底文件末尾，返回EOF
int fputc(int char,FILE *stream)
返回值：返回被写入的字符，发生错误则返回EOF
*/
void test3()
{
    FILE *fp=fopen("./test1.txt","r");
    if(fp==NULL)
    {
        perror("fopen error");
        exit(1);
    }


    FILE *fp_out=fopen("./test2.txt","w");
    if(fp==NULL)
    {
        perror("fopen error");
    }

    int n;
    while((n=fgetc(fp))!=EOF)//将test1.txtx内容读取，写入到test2.txt
    {
        fputc(n,fp_out);
    }
    fclose(fp);
    fclose(fp_out);
}

/*通过read、write实现文件copy功能
size_t read(int fildes,void *buf,size_t nbytes);
    从fildes文件读取nbytes个字节的数据，放到buf中
    返回值：0：没有读到任何数据，到达文件末尾
            -1：read调用出现错误
            >0:实际读取到的字节数
size_t write(int fildes,const void *buf,size_t nbytes);
    把缓冲区 buf 的前 nbytes 个字节写入与文件描述符 fildes 关联的文件中
    返回值：0：没有写入任何数据
            -1：write调用出现错误
            >0:实际写入的字节数
*/
void test4()
{
    int fd=open("./test4.txt",O_RDONLY);
    if(fd<0)
    {
        perror("open test4.txt failed!!!\n");
        return;
    }
    int fd_out=open("./test5.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fd_out<0)
    {
        perror("open test5.txt failed!!!\n");
        return;
    }

    char buf[1024]={0};
    size_t n;
    while((n=read(fd,buf,1024)))
    {
        if(n<0)
        {
            perror("read error");
            exit(1);
        }
        write(fd_out,buf,1024);
    }
    close(fd);
    close(fd_out);
}

/*
void perror(const char *s)
打印出错原因，将对应出错编号errno的描述打印出来
*/
void test5()
{
    int fd=open("./test6.txt",O_RDONLY);
    if(fd<0)
    {
        perror("open file failed");
        exit(1);
    }
}
/*
以非阻塞从终端读取：读不到，则返回，继续调用read
从普通文件读取通常不会阻塞，但是进行磁盘I/O操作，可能会发生阻塞
*/
#define MSG_TRY "try again\n"
void test6()
{
    char buf[10];
    int fd,n;
    //非阻塞方式从终端读取
    fd=open("/dev/tty",O_RDONLY|O_NONBLOCK);
    if(fd<0)
    {
        perror("open /dev/tty failed");
        exit(1);
    }
    else{
        printf("fd:%d\n",fd);
    }
    
tryagain:
    n=read(fd,buf,10);//当终端输入数据后，才会成功读取到数据
    if(n<0)
    {
        //判断读取错误是否是因无数据可读，不是则正常退出
        if(errno!=EAGAIN)
        {
            perror("read /dev/tty failed");
            close(fd);
            exit(1);
        }
        //终端无数据可读
        sleep(3);
        //向标准输出输入提示消息：继续尝试读取
        write(STDOUT_FILENO,MSG_TRY,strlen(MSG_TRY));
        goto tryagain;
    }
    write(STDOUT_FILENO,buf,n);
    close(fd);
}

/*
以阻塞方式从终端读取：读不到，则read阻塞，当终端输入数据，read返回
*/
void test7()
{
    int fd,n;
    char buf[20];
    fd=open("/dev/tty",O_RDONLY);
    if(fd<0)
    {
        perror("open /dev/tty failed");
        exit(1);
    }

    n=read(fd,buf,20);//阻塞读取
    write(STDOUT_FILENO,buf,n);//将读入的数据写入标准输出
    close(fd);
}

/*
非阻塞读终端和等待超时
*/
#define MSG_TIMEOUT "time out\n"
void test8()
{
    char buf[10];
    int i;
    int fd,n;

    fd=open("/dev/tty",O_RDONLY|O_NONBLOCK);

    if(fd<0)
    {
        perror("open /dev/tty failed");
        exit(1);
    }

    //timeout
    for(i=0;i<5;++i)//循环读取，超过5次，则超时
    {
        n=read(fd,buf,10);
        if(n>0)//读到数据则退出循环
        {
            break;
        }
        else//读取出错
        {
            
            if(errno!=EAGAIN)//非“文件无数据"错误，则打印错误信息后退出
            {
                perror("read /dev/tty");
                exit(1);
            }
            sleep(3);
            write(STDOUT_FILENO,MSG_TRY,strlen(MSG_TRY));//提示继续读取
        }
        
    }

    if(i==5)//超时
    {
        write(STDOUT_FILENO,MSG_TIMEOUT,strlen(MSG_TIMEOUT));
    }else
    {
        write(STDOUT_FILENO,buf,n);
    }
    close(fd);

}
/*
用于移动文件描述符所指向文件的读写位置(偏移量)
off_t lseek(int fd, off_t offset, int whence);
whence：指定offset的起始位置
        SEEK_SET:文件开头
        SEEK_CUR:文件指针当前位置
        SEEK_END:文件末尾
offset：相对于whence参数的偏移量，表示文件指定移动的距离
返回值：成功：返回距文件起始位置向后的偏移量
        失败:返回-1
*/

int main()
{
    //test4();
    test8();
}


