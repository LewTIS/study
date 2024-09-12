#include <stdio.h>
#pragma pack(2)
struct s1{
    char c1;
    int i;
    char c2;
};

struct s2{
    char c1;
    char c2;
    int i;
};

struct s3
{
    //double d;
    char c;
    int i;
};

struct s4
{
    char c1;
    struct s3 s3;
    int d;
};
struct A{
    int _a:2;
    int _b:5;
    int _c:10;
    int _d:30;
};
enum Color{
    RED=1,
    GREEN=4,
    BLUE
};
void printf_arr1(int arr[3][5],int row,int col){
    int i=0;
    for(int i=0;i<row;++i){
        for(int j=0;j<col;++j){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}
void printf_arr2(int (*arr)[5],int row,int col){
    int i=0;
    for(int i=0;i<row;++i){
        for(int j=0;j<col;++j){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}
void test(char **p){}
/* 
    parr3是一个有10个元素的数组，每个元素的类型为数组指针，指向有5个int类型元素的数组
*/
int (*parr3[10])[5];

/*
signal是一个函数，有两个参数，分别是int和一个函数指针，signal的返回值是一个函数指针，
指向参数为int，返回值为void的函数
*/
void (*signal(int , void(*)(int)))(int);

//(*(void (*p)())0)();
int main()
{
    //char *arr1[10];
    //test(arr1);

    int arr[3][5]={1,2,3,4,5,6,7,8,9,10};
    printf_arr1(arr,3,5);
    printf_arr2(arr,3,5);
    /*
    printf("%d %d %d\n",RED,GREEN,BLUE);
    struct s1 ss1;
    struct s2 ss2;
    struct s3 ss3;
    struct s4 ss4;
    struct A a1;
    printf("%ld\n",sizeof(a1));
    printf("%ld\n",sizeof(ss1));
    printf("%ld\n",sizeof(ss2));
    printf("%ld\n",sizeof(ss3));
    printf("%ld\n",sizeof(ss4));
    */
}
