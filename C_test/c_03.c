#include <stdio.h>

int main()
{
    char *c[]={"ENTER","NEW","POINT","FIRST"};
    char **cp[]={c+3,c+2,c+1,c};// FIEST的首地址 POINT的首地址 NEW的首地址 ENTER的首地址
    char ***cpp=cp; //cpp存储c+3的地址，c+3存储FIEST的首地址
    printf("%s\n",**++cpp);//POINT,此时cpp指向c+2
    printf("%s\n",*--*++cpp+3);//ER  cpp指向c+1
    printf("%s\n",*cpp[-2]+3);//ST   cpp指向c+3
    printf("%s\n",cpp[-1][-1]+1);

    /*
    char *a[]={"work","at","Alibaba"};//字符指针数组，a[0]指向work，a[1]指向at，a[2]指向Alibaba
    char **pa=a;//定义一个二级指针pa指向字符指针数组a，pa的值是"work"的首地址
    printf("%s\n",(*pa));
    pa++;//pa指向a[1]，存储a[1]的地址，即at的首地址
    printf("%s\n",(*pa));//对pa解引用，得到a[1]的值，即at

    /*
    int aa[2][5]={1,2,3,4,5,6,7,8,9,10};
    int *ptr1=(int *)(&aa+1);
    int *ptr2=(int *)((aa+1));
    printf("%d %d\n",*(ptr1-1),*(ptr2-1));

    /*
    int a[3][2]={(0,1),(2,3),(4,5)};
    int *p;
    p=a[0];
    printf("%d",p[0]);

    /*
    int a[4]={1,2,3,4};
    int *ptr1=(int *)(&a+1);
    int *ptr2=(int *)((int)a+1);//将数组名a(int *)强制转换为int类型，可能丢失信息，然后+1，以字节为单位
    printf("%x,%x\n",ptr1[-1],*ptr2);

    /*
    int a[5]={1,2,3,4,5};
    int *ptr=(int *)(&a+1);
    printf("%d,%d\n",*(a+1),*(ptr-1));
    */
    return 0;
}