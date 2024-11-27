#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char write_buf[1024] = "hello,World!";
    char read_buf[1024];
    ssize_t bytes_written, bytes_read;
    off_t seek_result;
    fd=open("/dev/mychardev_01",O_RDWR);
    if(fd<0)
    {
        perror("Failed to open device!\n");
        return -1;
    }

    bytes_written=write(fd,write_buf,strlen(write_buf));
    printf("Wrote %zd bytes.\n",bytes_written);

    seek_result = lseek(fd, 0, SEEK_SET);
    if (seek_result < 0) {
        perror("Seek failed");
        close(fd);
        return -1;
    }
    bytes_read=read(fd,read_buf,sizeof(read_buf));
    if(bytes_read>0)
    {
        read_buf[bytes_read] = '\0';//字符串结束标记
        printf("Read %zd bytes: %s\n",bytes_read,read_buf);
    }else if(bytes_read == 0)
    {
        printf("No data read\n");
    }else
        perror("Read failed\n");

    close(fd);
    return 0;
}
