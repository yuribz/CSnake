#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "linkedlist.h"

typedef struct Queue {
    LinkedList* queue;
    int         size;
} Queue;

Queue* createQueue() ;
bool enqueue(Queue* queue, short val1, short val2, short val3, short val4) ;
bool dequeue(Queue* queue, short* data) ;
bool peekFirst(Queue* queue, short* data) ;
bool peekLast(Queue* queue, short* data) ;

#endif