#ifndef _STACK_H
#define _STACK_H



typedef int DateType;

//栈：一种线性结构，只能在栈顶插入或删除元素
//采用顺序表模拟实现
typedef struct Stack
{
    DateType *array;
    int size;
    int capacity;
}Stack;
//初始化
void stackInit(Stack* ps);
//入栈
void stackPush(Stack* ps,DateType x);
//出栈
void stackPop(Stack* ps);
//获取栈顶元素
DateType stackTop(Stack* ps);
//获取栈中有效元素个数
int stackSize(Stack* ps);
//检测栈是否为空
int stackEmpty(Stack* ps);
//栈的销毁
void stackDestroy(Stack* ps);




#endif