#ifndef QUEUE_H
#define QUEUE_H

typedef int DateType;

typedef struct Qnode
{
    struct Qnode* next;
    DateType data;
}Qnode;
typedef struct Queue
{
    Qnode* back;
    Qnode* front;
}Queue;


//队列初始化
void QueueInit(Queue* qs);
//尾插
void QueuePush(Queue* qs,DateType x);
//头删
void QueuePop(Queue* qs);
//头部元素
DateType QueueFront(Queue* qs);
//尾部元素
DateType QueueBack(Queue* qs);
//队内元素数
int QueueSize(Queue* qs);
//判空
int QueueEmpty(Queue* qs);
//销毁
void QueueDestroy(Queue* qs);

#endif