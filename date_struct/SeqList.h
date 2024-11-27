#ifndef SEQLIST_H  
#define SEQLIST_H 
#include <stddef.h>
typedef int DateType;

typedef struct SeqList
{
    DateType* array;//指向一块动态开辟的内存
    size_t size;//当前顺序表中元素数
    size_t capacity;//顺序表容量
}SeqList;

//初始化
void SeqListInit(SeqList* ps);
//销毁
void SeqListDestroy(SeqList* ps);
//检查空间，不够则扩容
void CheckCapacity(SeqList* ps);

//尾插
void SeqListPushBack(SeqList* ps,DateType x);
//尾删
void SeqListPopBack(SeqList* ps);
//查找元素
int SeqListFind(SeqList* ps,DateType x);
//插入元素
void SeqListInsert(SeqList* ps,size_t pos,DateType x);
//删除元素
void SeqListErase(SeqList* ps,size_t pos);
//返回某位置元素
DateType SeqListAt(SeqList* ps,size_t pos);
//首元素
DateType SeqListFront(SeqList* ps);
//尾元素
DateType SeqListBack(SeqList* ps);
//顺序表存放元素数
size_t SeqListSize(SeqList* ps);
//容量
size_t SeqListCapactiy(SeqList *ps);
//是否为空
int SeqListEmpty(SeqList* ps);
#endif