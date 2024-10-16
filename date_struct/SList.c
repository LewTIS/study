#include "SList.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

//不带头单链表
//创建节点
SListNode* BuySListNode(DateType x)
{
    SListNode* newNode=(SListNode *)malloc(sizeof(SListNode));
    if(NULL==newNode)
    {
        printf("创建节点失败!\n");
        return NULL;
    }
    newNode->data=x;
    newNode->next=NULL;
    return newNode;
}
//尾插
void SListPushBack(SListNode **pplist,DateType x)
{
    assert(pplist);
    SListNode *node=BuySListNode(x);

    if(NULL==*pplist)//链表为空
    {
        *pplist=node;
    }else{
        //遍历链表，找到最后一个节点
        SListNode *cur=*pplist;
        while(cur->next)
        {
            cur=cur->next;
        }
        cur->next=node;
    }
}
//尾删
void SListPopBack(SListNode **pplist)
{
    assert(pplist);
    if(NULL==*pplist)//无节点
        return;
    else if((*pplist)->next==NULL)//只有一个节点
    {
        free(*pplist);
        *pplist=NULL;
        return;
    }else{
        //有两个及以上节点
        //找到最后一个节点的前一个节点，将next置为空，
        SListNode *pre=NULL;
        SListNode *cur=*pplist;
        while(cur->next)
        {
            pre=cur;
            cur=cur->next;
        }
        pre->next=NULL;
        free(cur);
    }
}
//头插
void SListPushFront(SListNode **pplist,DateType x)
{
    assert(pplist);
    SListNode *node=BuySListNode(x);

    node->next=*pplist;
    *pplist=node;
}
//头删
void SListPopFront(SListNode **pplist)
{
    assert(pplist);
    if(NULL==*pplist)//无节点
        return;
    else if(NULL==(*pplist)->next)//只有一个节点
    {
        free(*pplist);
        *pplist=NULL;
    }else{//2个及以上节点
        SListNode *delNode=*pplist;
        *pplist=delNode->next;
        free(delNode);
    }
}
//查找
SListNode* SListFind(SListNode *plist,DateType x)
{
    assert(plist);

    SListNode* cur=plist;
    while(cur)
    {
        if(cur->data==x)
            return cur;
        cur=cur->next;
    }
    return NULL;
}
//在第pos个节点后插入一个节点
void SListInsertAfter(SListNode *pplist,int pos,DateType x)
{
    assert(pplist);
    SListNode *cur=pplist;
    while(pos)
    {
        cur=cur->next;
        pos--;
    }
    SListNode *node=BuySListNode(x);
    node->next=cur->next;
    cur->next=node;
}
//删除第pos个节点的后一个节点
void SListEraseAfter(SListNode *pplist,int pos)
{

    assert(pplist);
    SListNode *cur=pplist;
    while(pos)
    {
        cur=cur->next;
        pos--;
    }
    if(cur->next==NULL)
        return;
    else{
        SListNode *delNode=cur->next;
        cur->next=delNode->next;
        free(delNode);
    }

    
}
//链表元素数
int SListSize(SListNode *plist)
{
    assert(plist);
    int count=0;
    SListNode *cur=plist;
    while(cur)
    {
        count++;
        cur=cur->next;
    }
    return count;
}
//销毁链表
void SListDestroy(SListNode **pplist)
{
    assert(pplist);

    SListNode *cur=*pplist;
    while(cur)
    {
        *pplist=(*pplist)->next;
        free(cur);
        cur=*pplist;
    }
}
void SListPrint(SListNode *plist)
{
    SListNode *cur=plist;
    while(cur->next)
    {
        printf("%d->",cur->data);
        cur=cur->next;
    }
    printf("%d",cur->data);
    printf("\n");
}
int main()
{
    SListNode *list;
    SListPushBack(&list,1);
    SListPushBack(&list,2);
    SListPushBack(&list,3);
    SListPrint(list);

    //SListPopBack(&list);
    SListPushFront(&list,0);
    SListPrint(list);

    SListPopFront(&list);
    SListPrint(list);
    //SListNode *node=SListFind(list,2);
    //printf("%d\n",node->data);
    SListInsertAfter(list,1,8);
    SListPrint(list);

    SListEraseAfter(list,1);
    SListPrint(list);

    int count=SListSize(list);
    printf("链表元素数为:%d\n",count);

    SListDestroy(&list);
}