#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char name[40];
    int age;
}person;
int main()
{




    /*
    //memmove(void *destinaton,const void *source,size_t num);//源数据与目标数据内存空间重叠时，使用memmove
    char str1[]="hello world";
    memmove(str1,str1+6,5);
    printf("%s\n",str1);
    

    /*
    //void *memcpy(void *destination,const void *source,size_t num);拷贝num个字节的数据到destination
    person p1,p2;
    char myname[]="Pierre de Fermat";
    memcpy(p1.name,myname,strlen(myname)+1);
    p1.age=29;
    memcpy(&p2,&p1,sizeof(person));
    printf("p2: %s,%d\n",p2.name,p2.age);
    /*
    int arr1[10]={1,2,3,4,5,6,7,8,9,10};
    int arr2[]={11,12,13,14,15};
    memcpy(arr1,arr2,sizeof(int)*5);
    for(int i=0;i<10;++i)
    {
        printf("%d ",arr1[i]);
    }
    */
    /*
    char str1[20]="hello";
    char *str2="abcdef";
    memcpy(str1,str2,8);
    printf("%s\n",str1);
    */
    
    /*
    //int tolower(int c)//将大写字母转换为小写字母
    //int toupper(int c)//将小写字母转换为大写字母
    int i=0;
    char str[]="Hello World!";
    while(str[i])
    {
        //str[i]=tolower(str[i]);
        str[i]=toupper(str[i]);
        i++;
    }
    puts(str);
    
    /*
    //char *strtok(char *str,const char *sep)//切割字符串，将str切割成一个个子串，sep为分隔符
    char str[]="- This, a sample string.";
    char *pch=strtok(str," ,.-");//第一次调用时传入被切割字符串的首地址，后面调用时传入NULL
    while(pch!=NULL)
    {
        printf("%s\n",pch);
        pch=strtok(NULL," ,.-");
    }

    /*
    //strstr(const char* str1,const char *str2); 查找子字符串.返回str2第一次在str1出现的位置
    char str[]="This is a simple string";
    char *pch=strstr(str,"simple");
    if(pch!=NULL)
    {
        strncpy(pch,"sample",6);
    }
    puts(str);
    /*
    //strncmp(char *str1,const char *str2,size_t num); 比较前num个字符
    const char* str1="abcdef";
    const char* str2="abc";
    if(strncmp(str1,str2,3)==0)
    {
        printf("str1==str2\n");
    }else if(strncmp(str1,str2,3)>0)
    {
        printf("str1>str2\n");

    }else{
        printf("str1<str2\n");

    }


    /*
    //strncat(char *destination,const char *source,size_t num); 拼接num个字符从源字符串到目标空间
    char str1[20]="hello";
    char *str2="abcdef";
    strncat(str1,str2,8);
    printf("%s\n",str1);

    /*
    //char *strncpy(char *destination,const char *source,size_t num); 拷贝num的字符从源字符串到目标空间
    //若源字符串的长度小于num，则拷贝完源字符串后，在后面追加0，直到num个,不拷贝\0
    char str1[20]="hello";
    char *str2="abcdef";
    strncpy(str1,str2,8);
    printf("%s\n",str1);
    
    /*
    //int strcmp(const char *str1,const char *str2);
    const char *str1="abcdef";
    const char *str2="abc";
    if(strcmp(str1,str2)==0)
    {
        printf("str1=str2\n");  
    }else if(strcmp(str1,str2)>0){
        printf("str1>str2\n");
    }else{
        printf("str1<str2\n");
    }
    /*
    //char* strcat(char *destination,const char *source);
    char str1[20]="hello";
    char *str2="abcdef";
    strcat(str1,str2);
    printf("%s\n",str1);

    /*
    //char * strcpy(char *destination,const char *source)
    char str1[20];
    char *str2="abcdef";
    strcpy(str1,str2);
    printf("%s\n",str1);

    /*
    //size_t strlen(const char *str);
    const char *str1="abcdef";
    const char *str2="abc";
    if(strlen(str1)>strlen(str2))
    {
        printf("str1>str2\n");
    }else{
        printf("str1<str2\n");
    }
    return 0;
    */
}
