#include <stdio.h>

int add(int a,int b)
{
    return a+b;
}

int sub(int a,int b)
{
    return a-b;
}

int mul(int a,int b)
{
    return a*b;
}

int div(int a,int b)
{
    return a/b;
}

int main()
{
    int x,y;
    int input=1;
    int ret=0;
    int (*prr[5])(int x,int y)={0,add,sub,mul,div};//函数指针数组，存放这四个运算函数地址
    while(input)
    {
        printf("*******************************\n");
        printf("*****1.add*********2.sub*******\n");
        printf("*****3.mul*********4.div*******\n");
        printf("请选择: \n");
        scanf("%d",&input);
        if(input>0&&input<5)
        {
            printf("请输入两个数:\n");
            scanf("%d%d",&x,&y);
            ret=(*prr[input])(x,y);//调用对应的函数进行计算
            printf("计算结果为%d\n",ret);
        }else{
            printf("输入有误，请重新输入!\n");
            input=1;
        }   
    }
}