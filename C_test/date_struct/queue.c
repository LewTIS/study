#include "queue.h"
#include <malloc.h>
#include <stdio.h>
#include <assert.h>




//队列初始化
void QueueInit(Queue* qs)
{
    assert(qs);
    qs->front=NULL;
    qs->back=NULL;
}
//创建节点
Qnode* BuyQueueNode(DateType x)
{
    Qnode* newNode=(Qnode* )malloc(sizeof(Qnode));
    if(NULL==newNode)
    {
        printf("创建失败!!!\n");
        return NULL;
    }
    newNode->next=NULL;
    newNode->data=x;
    return newNode;
}
//尾插
void QueuePush(Queue* qs,DateType x)
{
    assert(qs);
    Qnode* newNode=BuyQueueNode(x);
    if(NULL==qs->front)//队列为空
    {
        qs->front=newNode;
        qs->back=newNode;
    }else
    {
        qs->back->next=newNode;
        qs->back=newNode;
    }
}
//头删
void QueuePop(Queue* qs)
{
    assert(qs);
    if(qs->front==NULL)//队列为空
    {
        return;
    }else if(qs->front==qs->back){//有一个节点
        free(qs->front);
        qs->front=NULL;
        qs->back=NULL;
    }else{
        Qnode* delNode=qs->front;
        qs->front=delNode->next;
        free(delNode);
    }
}
//头部元素
DateType QueueFront(Queue* qs)
{
    assert(qs);
    return (qs->front)->data;
}
//尾部元素
DateType QueueBack(Queue* qs)
{
    assert(qs);
    return (qs->back)->data;
}
//队内元素数
int QueueSize(Queue* qs)
{
    assert(qs);
    Qnode *cur=qs->front;
    int count=0;
    while(cur)
    {
        count++;
        cur=cur->next;
    }
    return count;
    
}
//判空
int QueueEmpty(Queue* qs)
{
    return qs->front==NULL;
}
//销毁
void QueueDestroy(Queue* qs)
{
    assert(qs);
    Qnode* cur=qs->front;
    while(cur)
    {
        qs->front=cur->next;
        free(cur);
        cur=qs->front;
    }
}
void QueuePrint(Queue* qs)
{
    Qnode *cur=qs->front;
    while(cur->next)
    {
        printf("%d->",cur->data);
        cur=cur->next;
    }
    printf("%d\n",cur->data);
}


int main()
{
    Queue q;
    QueueInit(&q);

    QueuePush(&q,0);
    QueuePush(&q,1);
    QueuePush(&q,2);
    QueuePush(&q,3);
    QueuePrint(&q);

    QueuePop(&q);
    printf("头删一个元素后，队列中元素为：\n");
    QueuePrint(&q);

    int front=QueueFront(&q);
    printf("队头元素为：%d\n",front);
    int back=QueueBack(&q);
    printf("队尾元素为：%d\n",back);

    int size=QueueSize(&q);
    printf("队内元素数为：%d\n",size);

    QueueDestroy(&q);

}