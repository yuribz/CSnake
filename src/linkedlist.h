#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

typedef struct Node Node;

typedef struct LinkedList LinkedList;


LinkedList* createList();
bool get(LinkedList* ll, int idx, short* data);
bool add(LinkedList *ll, int idx, short val1, short val2, short val3, short val4);
bool append(LinkedList *ll, short val1, short val2, short val3, short val4);
bool delete(LinkedList* ll, int idx);
void printList(LinkedList* l);

#endif

