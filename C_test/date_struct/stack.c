#include "stack.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

//初始化
void stackInit(Stack* ps)
{
    assert(ps);
    ps->array=(DateType*)malloc(sizeof(Stack)*3);
    if(NULL==ps->array)
    {
        printf("空间开辟失败!!!\n");
        return;
    }
    ps->size=0;
    ps->capacity=3;
}
//扩容
void checkCapacity(Stack* ps)
{
    assert(ps);
    if(ps->size==ps->capacity)
    {
        int newCapacity=2*ps->capacity;
        DateType* tmp=realloc(ps->array,newCapacity);
        if(NULL==tmp)
        {
            printf("扩容失败!!!\n");
            return;
        }
        ps->array=tmp;
        ps->capacity=newCapacity;
    }
}
//入栈
void stackPush(Stack* ps,DateType x)
{
    assert(ps);
    checkCapacity(ps);
    ps->array[ps->size]=x;
    ps->size++;
}
//出栈
void stackPop(Stack* ps)
{
    assert(ps);
    if(stackEmpty(ps))
        return;
    ps->size--;
}
//获取栈顶元素
DateType stackTop(Stack* ps)
{
    assert(ps);
    return ps->array[ps->size-1];
}
//获取栈中有效元素个数
int stackSize(Stack* ps)
{
    assert(ps);
    return ps->size;
}
//检测栈是否为空
int stackEmpty(Stack* ps)
{
    assert(ps);
    return 0==ps->size;
}
//栈的销毁
void stackDestroy(Stack* ps)
{
    assert(ps);
    free(ps->array);
    ps->array=NULL;
    ps->size=0;
    ps->capacity=0;
}
void stackPrint(Stack* ps)
{
    assert(ps);
    for(int i=0;i<ps->size;++i)
    {
        printf("%d ",ps->array[i]);
    }
    printf("\n");
}
int main()
{
    Stack s;

    stackInit(&s);
    if(stackEmpty(&s))
        printf("栈为空\n");
    stackPush(&s,0);
    stackPush(&s,1);
    stackPush(&s,2);
    stackPush(&s,3);
    stackPush(&s,4);

    stackPrint(&s);

    //stackPop(&s);
    int top=stackTop(&s);
    printf("栈顶元素为:%d\n",top);
    int size=stackSize(&s);
    printf("栈内元素数为:%d\n",size);

    stackDestroy(&s);

}