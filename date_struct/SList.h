#ifndef SLIST_H
#define SLIST_H

typedef int DateType;

typedef struct SListNode
{
    struct SListNode *next;
    DateType data;
}SListNode;

//创建节点
SListNode* BuySListNode(DateType x);
//尾插
void SListPushBack(SListNode **pplist,DateType x);
//尾删
void SListPopBack(SListNode **pplist);
//头插
void SListPushFront(SListNode **pplist,DateType x);
//头删
void SListPopFront(SListNode **pplist);
//查找
SListNode* SListFind(SListNode *plist,DateType x);
//在pos节点后插入节点
void SListInsertAfter(SListNode *pplist,int pos,DateType x);
//删除pos后的节点
void SListEraseAfter(SListNode *pplist,int pos);
//链表元素数
int SListSize(SListNode *plist);
//销毁链表
void SListDestroy(SListNode **pplist);

void SListPrint(SListNode *plist);
#endif