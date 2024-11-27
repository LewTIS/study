#include <stdio.h>
#include <string.h>
#include <math.h>
void swap(int *left,int *right)
{
    int tmp=*left;
    *left=*right;
    *right=tmp;
}
//将二维数组行列互换
void transport(int (*arr)[4],int row,int col)
{
    for(int i=0;i<row-1;++i)
    {
        for(int j=i+1;j<col;++j)
        {
            swap(&arr[i][j],&arr[j][i]);
        }
    }
}
//数字分离输出
void number_part(int num)
{
    int result[10]={0};
    int i=0;

    while(num)
    {
        result[i++]=num%10;
        num/=10;
    }
    for(int j=i-1;j>=0;--j)
    {
        printf("%d ",result[j]);
    }
    printf("\n");
}

//统计水仙花数1-100000
int is_narcissistic_number(int n)
{
    //1.统计位数
    int count=1;
    int i=n;
    while(i/10)
    {
        count++;
        i/=10;
    } 
    //2.各位的位数次方之和相加是否等于n
    i=n;
    int sum=0;
    while(i)
    {
        sum+=pow(i%10,count);
        i/=10;
    }
    if(n==sum)
        return 1;
    return 0;
}
int sum_factors(int num)
{
    int sum=0;
    for(int i=1;i<num;++i)
    {
        if(num%i==0)
            sum+=i;
    }
    return sum;
}
//3000以内的亲密数
void intimate_number()
{
    for(int i=1;i<=3000;++i)
    {
        int j=sum_factors(i);
        if(j>i&&j<=3000&&sum_factors(j)==i)
        {
            printf("(%d,%d) ",i,j);
        }
    }
}
int is_leap_year(int year)
{
    if((year%4==0&&year%100!=0)||year%400==0)
        return 1;
    return 0;
}
int caculate_date(int year,int month,int day)
{
    int map[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int count=0;
    count+=day;
    for(int i=1;i<month;++i)
    {
        count+=map[i];
    }
    if(month>2&&is_leap_year(year))
        count+=1;

    int count2=count;
    //1900.1.1是星期一
    for(int i=1900;i<year;++i)
    {
        if(is_leap_year(i))
            count2+=366;
        else
            count2+=365;
    }
    return count2%7;
}


int main()
{


    int a,n;
    scanf("%d%d",&a,&n);
    int sum=0;
    int tmp=0;
    for(int i=0;i<n;++i)
    {
        tmp=tmp*10+a;
        printf("%d\n",tmp);
        sum+=tmp;
    }
    printf("%d",sum);


    //int day=caculate_date(2024,9,1);
   //printf("%d\n",day);
    //intimate_number();
    /*
    int num=1234;
    //number_part(num);
    for(int i=100;i<10000;++i)
    {
        if(is_narcissistic_number(i))
            printf("%d ",i);
    }
    /*
    int arr[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    transport(arr,4,4);
    for(int i=0;i<4;++i)
    {
        for(int j=0;j<4;++j)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }*/
    return 0;
}