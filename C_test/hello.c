#include <stdio.h>
#include <time.h>
#include <unistd.h>
void timer()
{
    while (1)
    {
        time_t current_time = time(NULL);
        if (current_time == ((time_t)-1))
        {
            printf("get time error\n");
            return;
        }
        struct tm *local_time=localtime(&current_time);
        if(local_time==NULL)
        {
            printf("get local time error\n");
            return;
        }
        char time_str[128];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
        printf("%s\n", time_str);
        sleep(1); // Sleep for 1 second
    }
}
//实现一个定时器函数，每隔一秒输出一次当前时间

int main()
{
    time_t current_time;

    current_time = time(NULL);//获取当前时间，1970年1月1日到现在的秒数
    if(current_time == ((time_t)-1))
    {
        printf("get time error\n");
        return -1;
    }
    printf("%ld\n",current_time);
    timer();
    return 0;
}

