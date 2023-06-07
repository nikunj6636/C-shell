#ifndef _QUEUE

#define _QUEUE

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef char* QElement; // QElement is the vertex here.

typedef struct Queue_ // implementing a circular queue data type.
{
    int capacity;
    int numelems;

    int front;
    int rear;
    
    QElement* QArray;
}Queue_;

typedef struct Queue_* Queue;

Queue QueueInit(int capacity);   // structured thinking by DSA Sir of data types.

void EnQueue(Queue Q,QElement E);

QElement DeQueue(Queue Q);

int isQueFull(Queue Q);

int isQueEmpty(Queue Q);

void PrintQueue(Queue Q);

void ClearQueue(Queue Q);

void DeleteQueue(Queue Q);

#endif