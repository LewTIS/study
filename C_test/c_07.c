#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define MAX(a,b) (a)*(b)
struct student
{
    int num;
    char str[256];
};
union Un1 
{ 
 char c[5]; 
 int i; 
}; 
union Un2 
{ 
 short c[7]; 
 int i; 
}; 

enum week
{
    MON,
    TUE,
    WED,
    TUR,
    FRI,
    SAT,
    SUN
};

struct A{
    double a;
    char b;
    short c;
};
struct B{
    char a;
    struct A b;
    short c;
};
int get_max(int **array,int m, int n)
{
    int max=array[0][0];
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(max<array[i][j])
                max=array[i][j];
        }
    }
    return max;
}
int main()
{

    int array[2][2]={1,2,3,4};
    int **array2=(int **)malloc(sizeof(int *)*2);
    for(int i=0;i<2;++i)
    {
        array2[i]=(int *)malloc(sizeof(int *)*2);
    }
    int m=2;
    for(int i=0;i<2;++i)
    {
        for(int j=0;j<2;++j)
        {
            array2[i][j]=m++;
        }
    }
    int max=get_max(array2,2,2);
    printf("%d\n",max);
    int a=141;
    int b=153;
    a=a|b;
    printf("%d\n",a);
    int res=MAX(2+1,4+1);
    printf("%d\n",res);
    struct A A1;
    struct B B1;
    printf("%ld\n",sizeof(A1));
    printf("%ld\n",sizeof(B1));
    enum week w=TUE;
    printf("%d\n",w);
    //下面输出的结果是什么？
    printf("%ld\n", sizeof(union Un1)); 
    printf("%ld\n", sizeof(union Un2)); 
    struct student s1={20,"lihua"};
    s1.num=10;
    //strcpy(s1.str,"xiaoli");
    printf("%d\n",s1.num);
    printf("%s\n",s1.str);

    //未显示指定数组字符数组的空间数，最后面需加上'\0',否则strlen计算出错
    char c[]={'a','b','c','d'};
    char c1[]="abc";//自带'\0'
    printf("%ld\n",strlen(c));
    printf("%ld\n",strlen(c1));
    /*
    int num;
    //scanf("%*d%*d%d",&num);
    //printf("%d\n",num);
    int i=0;
    scanf("%d",&num);
    switch(num)
    {
        case 1:
            printf("one\n");
            break;
        case 2:
            printf("two\n");
            i+=1;
            break;
        case 3:
            printf("three\n");
            break;
        default:
            printf("other\n");
    }
    printf("%d\n",i);
    */
    return 0;
    
}