#include"queue.h"  // maintaing the circular queue.

Queue QueueInit(int capacity)
{
    assert(capacity!=0);

    Queue Q = (Queue) malloc(sizeof(Queue_));
    assert(Q!=NULL);

    Q->capacity = capacity;
    Q->numelems = 0;

    Q->front = 0; // access elems from the front of the Queue .
    Q->rear = -1; // implementing circular Queue .
   
    (Q->QArray) = (QElement*) malloc(sizeof(QElement)*capacity);

    return Q;
}

int isQueFull(Queue Q)
{
    return (Q->numelems == Q->capacity);
}

int isQueEmpty(Queue Q)
{
    return (Q->numelems == 0);
}

void EnQueue(Queue Q,QElement E)
{
    assert(!isQueFull(Q));

    Q->rear = (Q->rear + 1)%(Q->capacity) ; // implementing circular queue
    (Q->QArray)[Q->rear] = E;
    Q->numelems++;

}

QElement DeQueue(Queue Q)
{
    assert(!isQueEmpty(Q));

    QElement E = (Q->QArray)[Q->front];
    Q->front = (Q->front + 1)%(Q->capacity); // remember for circular we have to use %(modulo) fxn
    Q->numelems --;
    return E;
}

int min(int a, int b)
{
    return (a<b) ? a : b;
}

void PrintQueue(Queue Q)
{
    int i;

    if (Q->numelems > 10) 
    {
        i =  (Q->rear-9)%Q->capacity;
        if (i < 0) i += Q->capacity;
    }
    else i = Q->front;
    
    for(int nelems = 1; nelems <= min(10, Q->numelems) ; i=(i+1)%Q->capacity, nelems++)
    {
        printf("%d %s\n",nelems, (Q->QArray)[i]);
    }
}

void ClearQueue(Queue Q)
{
    Q->front = 0;
    Q->rear = -1; // elems is not availabe. 
    Q->numelems = 0;
}

void DeleteQueue(Queue Q)
{
    free(Q->QArray);
    free(Q);
}