#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void reverse_print_num(int n)
{
	char buf[15];
	//将整数写入字符串内
	sprintf(buf,"%d",n);
	//itoa(n,buf,10);
	if(buf[0]=='-')
	{
		n=-n;
		putchar('-');	
	}
	while(n)
	{
		printf("%d",n%10);
		n/=10;
	}
}
/*
void reverse_str(char *str)
{
	
	int left=0;
	int right=strlen(str)-1;
	while(left<right)
	{
		char tmp=str[left];
		str[left]=str[right];
		str[right]=tmp;

		left++;
		right--;
	}
}
*/
void reverse_str(char *str)
{
	char *left=str;
	char *right=str+strlen(str)-1;
	while(left<right)
	{
		char tmp=*left;
		*left=*right;
		*right=tmp;

		left++;
		right--;
	}
}

void insert_num()
{
    int n;
    int num[10]={0};
    for(int i=0;i<9;++i)
    {
        scanf("%d",&num[i]);
    }
    scanf("%d",&n);
    int end=8;//有序数组最后一个元素下标
    while(end>-1&&num[end]>=n)//从前往后找小于n的元素
    {
        num[end+1]=num[end];//往后移动一位
        end--;
    }
    num[end+1]=n;
    for(int i=0;i<10;++i)
    {
        printf("%d\n",num[i]);
    }
}

//输入一个10进制正整数，然后输出它所对应的八进制数。
void deciaml_conversion1(int n)
{
	int i=n;
	int array[10]={0};
	int count=0,j=0;
	while(i)
	{
		count++;
		array[j++]=i%8;
		i/=8;
	}
	printf("%d的八进制数为：",n);
	for(int i=count-1;i>=0;--i)
	{
		printf("%d",array[i]);
	}
	printf("\n");
}
//10进制转换16进制
void deciaml_conversion2(int n)
{
	int i=n;
	char result[10];
	char map[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int j=0;
	while(i)
	{
		result[j++]=map[i%16];
		i/=16;
	}
	reverse_str(result);
	printf("%d的十六进制数为：",n);
	puts(result);
}
void deciaml_conversion3(int n)
{
	int i=n;
	int array[32]={0};
	int count=0,j=0;
	while(i)
	{
		count++;
		array[j++]=i%2;
		i/=2;
	}
	printf("%d的二进制数为：",n);
	for(int i=count-1;i>=0;--i)
	{
		printf("%d",array[i]);
	}
	printf("\n");
}
void deciaml_conversion(int n,int radix)
{
	if(radix==8)
	{
		deciaml_conversion1(n);
	}else if(radix==16)
	{
		deciaml_conversion2(n);
	}else if(radix==2)
	{
		deciaml_conversion3(n);
	}else
		return;
}
/*
输入一个华氏温度，要求输出摄氏温度。公式为C=5/9(F-32)
二级C语言-温度转换
保留两位小数
*/
void temperature_convert(double f)
{
	double c=5*(f-32)/9;
	printf("%.2f",c);
}
//输入两个正整数m和n，求其最大公约数和最小公倍数
//最大公约数
int max_divisor(int m,int n)
{
	int result;
	int t=m>n?m:n;
	for(int i=t;i>=0;--i)
	{
		if(m%i==0&&n%i==0)
		{
			result=i;
			break;
		}	
	}
	return result;
}
int min_multi(int m,int n)
{
	int result;
	int t=m>n?n:m;
	for(int i=t;i<=m*n;++i)
	{
		if(i%m==0&&i%n==0)
		{
			result=i;
			break;
		}
	}
	return result;
}

int test(int m,int n)
{
	printf("%d和%d的最小公倍数为：%d\n",m,n,min_multi(m,n));
	printf("%d和%d的最大公因数为：%d\n",m,n,max_divisor(m,n));
}
//阶乘
double factorial(int n)
{
	if(n==1)
		return 1;
	else
		return n*factorial(n-1);
}
double sum_factorial(int m)
{
	double sum=0;
	for(int i=1;i<=m;++i)
	{
		sum+=factorial(i);
	}
	return sum;
}
int main()
{
	int sum=sum_factorial(5);
	printf("%d\n",sum);
	//test(1,50);
	//insert_num();
	//deciaml_conversion(120,2);
	//temperature_convert(-40);




	/*
	int i;
	char buf[15];
	printf("enter a number:\n");
	scanf("%d",&i);
	//reverse_print_num(i);
	//printf("\n");

	
	sprintf(buf,"%d",i);
	if(buf[0]=='-')
		reverse_str(buf+1);
	else
		reverse_str(buf);
	
	printf("%s\n",buf);
	return 0;
	*/
}
