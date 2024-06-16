#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"

typedef struct Stack {
    LinkedList* stack;
    int         size;
} Stack;

/*
    Takes a look at the last element in the stack and copies its value
    to the passed in data pointer.

    Returns true if the peek operation is successful. Returns false if the stack
    is empty (since there is nothing to peek at).
*/
bool peek(Stack* stack, short* data) {
    if (stack->size == 0) {
        return false;
    }
    else {
        return get(stack->stack, -1, data);
    }
}

/*
    Peeks at the last value in the stack and removes it from the list.

    Returns true if successful (the value is returned and the value is deleted).
    Returns false if the stack is empty.
*/
bool pop(Stack* stack, short* data) {
    if (stack->size == 0) {
        return false;
    }
    else {
        peek(stack, data);
        delete(stack->stack, -1);
        stack->size--;
        return true;
    }
}

/*
    Adds the short values at the end of the stack. Always returns true.
*/
bool push(Stack* stack, short val1, short val2, short val3, short val4){
    append(stack, val1, val2, val3, val4);
    return true;
}

int main(int argc, char *argv[]){
    LinkedList *l = createList();
    append(l, 1, 0, 0, 0);

    printList(l);
}