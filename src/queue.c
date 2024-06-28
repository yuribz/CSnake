#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"
#include "queue.h"

Queue* createQueue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->queue = createList();
    return queue;
}

bool enqueue(Queue* queue, short val1, short val2, short val3, short val4) {
    return append(queue->queue, val1, val2, val3, val4);
    queue->size = queue->queue->size;
}

bool dequeue(Queue* queue, short* data) {
    get(queue->queue, 0, data);
    queue->size = queue->queue->size;
    return delete(queue->queue, 0);
};

bool peekFirst(Queue* queue, short* data) {
    return get(queue->queue, 0, data);
};

bool peekLast(Queue* queue, short* data) {
    return get(queue->queue, -1, data);
};