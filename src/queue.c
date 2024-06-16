#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"

typedef struct Queue {
    LinkedList* queue;
    int         size;
} Queue;

bool enqueue(Queue queue, short val1, short val2, short val3, short val4) {
    return true;
}

bool dequeue(Queue queue, short* data) {
    return true;
};

bool peekFirst(Queue queue, short* data) {
    return true;
};

bool peekLast(Queue queue, short* data) {
    return true;
};