#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

typedef struct Stack {
    LinkedList* stack;
    int         size;
} Stack;

short* pop(Stack stack) {
    
}

int main(int argc, char *argv[]){
    LinkedList *l = createList();
    append(l, 1, 0, 0, 0);

    printList(l);


}