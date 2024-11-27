#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//void *malloc(size_t size) 分配一段连续内存
//void free(voifd *ptr)  释放内存
//void *calloc(size_t num,size_t size) 分配一段连续内存，并初始化为0
/*
//void *realloc(void *ptr,size_t size) 
//对已分配的内存进行重新分配，并将原空间的内容拷贝到新空间，返回新分配空间的起始位置，
//1.若原空间后有足够空间，则在原有内存后追加
//2.若原空间后没有足够空间，则在堆上找一个合适大小的连续空间使用
*/
void test()
{
    int i=0;
    int *p=(int *)malloc(10*sizeof(int));
    if(NULL==p)
    {
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<=10;i++)//越界访问
    {
        *(p+i)=i;
        printf("%d ",*(p+i));
    }
    free(p);
}
void GetMemory1(char *p)//char **p
{
    p=(char *)malloc(100);//*p=(char *)malloc(100);
}
void test1()
{
    char *str=NULL;
    GetMemory1(str);//GetMemory(&str);要在另一个函数内对str进行修改，需要传递指针的指针
    strcpy(str,"hello world");
    puts(str);
}
char *GetMemory2(void)
{
    //char p[]="hello world";
    char *p=(char *)malloc(100);
    if(NULL == p)
    {
        return NULL;
    }
    strcpy(p,"hello world");
    return p;//返回函数内部指向局部变量的指针，函数结束时，局部变量会被销毁，指针指向的内存空间被释放，返回的指针指向未知
}
void test2()
{
    char *str=NULL;
    str=GetMemory2();
    puts(str);
}
void GetMemory3(char **p,int num)
{
    *p=(char *)malloc(num);
}
void test3()
{
    char *str=NULL;
    GetMemory3(&str,100);
    strcpy(str,"hello");
    puts(str);
    free(str);
}
void test4()
{
    char *str=(char *)malloc(100);
    strcpy(str,"hello");
    free(str);
    if(str!=NULL)//str指向的空间已被释放，此时str指向未知，是一个野指针
    {
        strcpy(str,"world");
        puts(str);
    }
}
typedef struct st_type
{
    int i;
    int a[];//柔型数组成员，柔性数组成员必须在结构体的最后
}type_a;

int main()
{
    int i=0;
    type_a *p=(type_a *)malloc(sizeof(type_a)+100*sizeof(int));
    p->i=100;
    for(i=0;i<100;i++)
    {
        p->a[i]=i;
    }
    free(p);
    printf("%ld\n",sizeof(type_a));
    //test4();
    return 0;
    /*
    int *ptr=NULL; 
    ptr=(int *)malloc(sizeof(int)*5);
    //ptr=(int *)calloc(5,sizeof(int));
    if(ptr!=NULL)
    {
        int i=0;
        for(int i=0;i<5;i++)
        {
           *(ptr+i)=0;
           printf("%d ",*(ptr+i));
        }
    }
    free(ptr);
    ptr=NULL;
    */
   // test();
   
}
