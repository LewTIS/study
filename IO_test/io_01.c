#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//按字符读写fgetc、fputc
void test1()
{
    //以读写的方式打开文件，没有就创建
    FILE *f_write=fopen("./test1.txt","w+");
    if(f_write==NULL)
    {
        printf("W_open file failed!!!\n");
        return;
    }
    char buf[]="Read and write as characters";
    for(int i=0;i<strlen(buf);i++)
    {
        fputc(buf[i],f_write);//以字符方式写入
    }

    fclose(f_write);

    FILE *f_read=fopen("./test1.txt","r");
    if(f_read==NULL)
    {
        printf("R_open file failed!!!\n");
        return;
    }
    char ch;
    while((ch=fgetc(f_read))!=EOF)//以字符方式读
    {
        printf("%c",ch);
    }
    fclose(f_read);
}

//按行读写fgets、fputs
/*
int  fgetc（FILE *stream)  
返回值为：以无符号char类型强制转换为int类型返回读取到字符,到底文件末尾，返回EOF
int fputc(int char,FILE *stream)
返回值：返回被写入的字符，发生错误则返回EOF
*/
void test2()
{

    //写入文件
    FILE *f_write=fopen("./test2.txt","w");
    if(f_write==NULL)
    {
        printf("W_open file failed!!!\n");
        return;
    }

    char *buf[]={
        "hello world\n",
        "hello world1\n",
        "hello world2\n",
    };
    int len=sizeof(buf)/sizeof(char *);
    for(int i=0;i<len;++i)
    {
        fputs(buf[i],f_write);
    }
    fclose(f_write);
    //读取文件
    FILE *f_read=fopen("./test2.txt","r");
    if(f_read==NULL)
    {
        printf("R_open file failed!!!\n");
        return;
    }
    while(!feof(f_read))
    {
        char buf[1024]={0};
        fgets(buf,1024,f_read);
        printf("%s",buf);
    }
    fclose(f_read);
}

//按块读写文件 fread、fwrite
void test3()
{
    //写文件
    FILE *f_write=fopen("./test3.txt","w+");
    if(NULL==f_write)
    {
        printf("W_open file failed!!!\n");
        return;
    }
    typedef struct Person{
        char name[16];
        int age;
    }Person;
    Person persons[5]=
    {
        {"zhangsan",25},
        {"lisi",22},
        {"wangwu",29},
        {"zhanghong",24},
        {"xiaojian",19}
    };
    int len=sizeof(persons)/sizeof(Person);
    //参数：数据地址，块大小，块的个数，文件流
    fwrite(persons,sizeof(Person),5,f_write);

    fclose(f_write);

    //读文件
    FILE *f_read=fopen("./test3.txt","r");
    if(f_read==NULL)
    {
        printf("R_open file failed!!!\n");
        return;
    }
    Person tmp[5];
    fread(tmp,sizeof(Person),len,f_read);
    for(int i=0;i<len;++i)
    {
        printf("name:%s,age:%d\n",tmp[i].name,tmp[i].age);
    }
    fclose(f_read);
}

//按格式化读写文件 fprintf、fscanf
void test4()
{
    //写文件
    FILE *f_write=fopen("./test4.txt","w");
    if(f_write==NULL)
    {
        printf("W_open file failed!!!\n");
        return;
    }
    fprintf(f_write,"hello world %d year - %d -month %d -day",2024,8,8);
    fclose(f_write);

    //读文件
    FILE *f_read=fopen("./test4.txt","r");
    if(NULL==f_read)
    {
        printf("R_open file failed!!!\n");
        return;
    }
    
    char buf[1024]={0};
    while(!feof(f_read))//直到文件结束标识符，循环结束
    {
        fscanf(f_read,"%s",buf);
        printf("%s ",buf);
    }
    fclose(f_read);
}
int main(int argc,char *argv[])
{
    //test1();
    //test2();
    //test3();
    test4();
}
