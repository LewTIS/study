#include "SeqList.h"
#include <stdio.h>
#include <assert.h>
#include <malloc.h>



//初始化
void SeqListInit(SeqList* ps)
{
    assert(ps);
    ps->array=(DateType*)malloc(3*sizeof(DateType));
    if(NULL==ps->array)
    {
        printf("空间开辟失败！！！\n");
        return;
    }
    ps->size=0;
    ps->capacity=3;
}

//销毁
void SeqListDestroy(SeqList* ps)
{
    assert(ps);
    if(ps->array)
    {
        free(ps->array);
        ps->array=NULL;
        ps->size=0;
        ps->capacity=0;
    }   
}
//检查空间，不够则扩容
void CheckCapacity(SeqList* ps)
{
    assert(ps);
    if(ps->size==ps->capacity)
    {
        int newCapacity=2*(ps->capacity);
        DateType* tmp=realloc(ps->array,newCapacity*sizeof(DateType));
        if(NULL==tmp)
        {
            printf("扩容失败!!!\n");
        }
        ps->array=tmp;
        ps->capacity=newCapacity;
    }
}

//尾插
void SeqListPushBack(SeqList* ps,DateType x)
{
    assert(ps);
    CheckCapacity(ps);
    ps->array[ps->size]=x;
    ps->size++;
}
//尾删
void SeqListPopBack(SeqList* ps)
{
    assert(ps);
    if(SeqListEmpty(ps))
        return;
    ps->size--;
}
//查找元素
int SeqListFind(SeqList* ps,DateType x)
{
    assert(ps);
    
    for(int i=0;i<ps->size;++i)
    {
        if(x==ps->array[i])
            return i;
    }
    return -1;
}
//插入元素
void SeqListInsert(SeqList* ps,size_t pos,DateType x)
{
    assert(ps);
    if(pos<0||pos>ps->size)
    {
        printf("SeqListInsert:越界了!!!\n");
        return;
    }
    CheckCapacity(ps);
    //将pos及其之后的元素往后移动一个位置
    for(int i=ps->size-1;i>=pos;--i)
    {
        ps->array[i+1]=ps->array[i]; 
    }
    ps->array[pos]=x;
    ps->size++;

}
//删除元素
void SeqListErase(SeqList* ps,size_t pos)
{
    assert(ps);
    if(pos>=ps->size)
    {
        printf("SeqListErase:越界!!!\n");
        return;
    }
    //pos后面元素开始，往前移动一位 
    for(int i=pos+1;i<ps->size;++i)
    {
        ps->array[i-1]=ps->array[i];
    }
    ps->size--;
}
//返回某位置元素
DateType SeqListAt(SeqList* ps,size_t pos)
{
    assert(ps);
    assert(pos<ps->size);
    return ps->array[pos];
}
//首元素
DateType SeqListFront(SeqList* ps)
{
    assert(ps);
    return ps->array[0];
}
//尾元素
DateType SeqListBack(SeqList* ps)
{
    assert(ps);
    return ps->array[ps->size-1];
}
//顺序表存放元素数
size_t SeqListSize(SeqList* ps)
{
    assert(ps);
    return ps->size;
}
//容量
size_t SeqListCapactiy(SeqList *ps)
{
    assert(ps);
    return ps->capacity;
}
//是否为空
int SeqListEmpty(SeqList* ps)
{
    assert(ps);
    return 0==ps->size;
}
void SeqListPrint(SeqList* ps)
{
    for(int i=0;i<ps->size;++i)
    {
        printf("%d ",ps->array[i]);
    }
    printf("\n");
}

int main()
{
    SeqList list;
    SeqList *ps=&list;

    SeqListInit(ps);
    SeqListPushBack(ps,1);
    SeqListPushBack(ps,2);
    SeqListPushBack(ps,3);
    SeqListPushBack(ps,4);
    
    SeqListPrint(ps);
    int size=SeqListSize(ps);
    printf("顺序表元素数为：%d\n",size);

    int capacity=SeqListCapactiy(ps);
    printf("顺序表容量为：%d\n",capacity);

    int front=SeqListFront(ps);
    printf("顺序表首元素为：%d\n",front);

    int pos=SeqListFind(ps,3);
    printf("元素3的位置为：%d\n",pos);

    DateType back=SeqListBack(ps);
    printf("顺序表尾元素为：%d\n",back);

    //SeqListPopBack(ps);//删除尾元素

    SeqListInsert(ps,1,7);
    SeqListPrint(ps);

    SeqListErase(ps,4);
    SeqListPrint(ps);
}