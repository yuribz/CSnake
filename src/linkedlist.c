#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    short       *data;
} Node;

const int DATASIZE = 4 * sizeof(short);
const int NODESIZE = sizeof(Node);
const int LISTSIZE = sizeof(LinkedList); 
const int SHORTSIZE = sizeof(short);

// Create a datum with text, int and double data
// PRIVATE
short *fillData(short val1, short val2, short val3, short val4) {
    short* data = malloc(DATASIZE);

    data[0] = val1;
    data[1] = val2;
    data[2] = val3;
    data[3] = val4;

    return data;
}

// Create a node with optional next and prev pointers,
// as well as a datum of data.
// PRIVATE
Node* createNode(Node *prev, Node *next, short *data) {
    Node* node = (Node*) malloc(NODESIZE);

    node->data = data;
    node->prev = prev;
    node->next = next;

    return node;
}

// Creates an empty linked list and returns the pointer to it.
LinkedList* createList(){
    LinkedList* list = (LinkedList*) malloc(LISTSIZE);

    list->first = NULL;
    list->last  = NULL;
    list->size  = 0;
    
    return list;
}

// Finds the node at the specified index. Returns NULL if it doesn't exist.
// PRIVATE.
Node *findNode(LinkedList* ll, int idx) {
    idx = idx == -1 ? ll->size - 1 : idx;

    Node *curNode;

    if (idx == -1) { curNode = ll->last; }
    else {
        curNode = ll->first;

        int curIdx = 0;

        while (curIdx != idx && curNode != NULL){
            curNode = curNode->next;
            curIdx++;
        }
    }
    
    return curNode;
}

/*
    Adds node at the specified index. Index of -1 or equal to size inserts at the end. Returns 1 if insertion successful, returns 0 if insertion failed.
    
    Using -1 for appending is recommended! Using append method does the same thing as add(ll, -1, ...).
*/
bool add(LinkedList *ll, int idx, short val1, short val2, short val3, short val4){
    
    // Insertion successful if the index is -1 or within the domain of the list.
    if (idx == -1 || (idx >= 0 && idx <= ll->size)){

        // printf("Inserting a new node with index %.3d\n", idx);

        Node* node; 
        short* data = fillData(val1, val2, val3, val4);


        // To fit the switch-case pattern, do this, since
        // plugging in size as idx is the same as -1.
        if (idx == ll->size) idx = -1;

        switch (idx) {
            // At the end
            case -1: {
                node = createNode(ll->last, NULL, data);

                // Check if the list is empty (i.e. last element does not exist).
                if (ll->last == NULL){
                    ll->first = node;
                    ll->last = node;
                }
                else {
                    // The new node is already pointing its prev to the last node.
                    // Rewire last's next to be the new node. Make the new node the last.

                    ll->last->next = node;
                    ll->last = node;
                }
                
                break;
            }
            // At the beginning
            case 0: {

                node = createNode(NULL, ll->first, data);

                // Check if the list is empty (i.e. first element does not exist).
                if (ll->first == NULL){
                    ll->first = node;
                    ll->last = node;
                }
                else {
                    // The new node is already pointing its next to the first node.
                    // Rewire first's prev to be the new node. Make the new node the first.
                    ll->first->prev = node;
                    ll->first = node;
                }

                break;

            }
            // In the middle, BEFORE the currently existing node at that idx.
            // If the function gets here, that means the list is non-empty, as any
            // idx above 0 called on an empty list would return FALSE.
            default: {
                Node *nodeAtIdx = findNode(ll, idx);

                // Make a new node with its prev/next connections.
                // Connect the new node's neighbors to itself.
                node = createNode(nodeAtIdx->prev, nodeAtIdx, data);
                node->prev->next = node;
                node->next->prev = node;

                break;
            }
        }

        ll->size++;
        return true;
    }
    // Insertion not succesful if the index is invalid.
    else {
        return false;
    }
    
}

/*
    Gets the contents of the data of the node at the specified index.
    Takes in an argument which is the short array pointer where the contents of
    the node are meant to be stored. If the node is NULL, returns FALSE and does not modify
    the array.

    Accessing the node at index -1 will get the last node in the list (hello, Python!).
*/
bool get(LinkedList* ll, int idx, short* data) {
    idx = idx == -1 ? ll->size - 1 : idx;
    
    Node* node = findNode(ll, idx);

    if (node == NULL) {
        return false;
    }
    // If the node exist, copy values from the node's data to the
    // data pointer that was passed in.
    else {
        // for (int i = 0; i < DATASIZE / SHORTSIZE; i++){
        //     data[i] = node->data[i];
        // }
        memcpy(data, node->data, DATASIZE);
        return true;
    }
}

// Adds node at the end. Simply calls addNode with -1 index.
bool append(LinkedList *ll, short val1, short val2, short val3, short val4){
    return add(ll, -1, val1, val2, val3, val4);
}

/*
    Finds the node at the specified index and removes it from the list.

    Accessing the node at index -1 gets the last node.

    The function returns TRUE if the node at the index exists and was successfully deleted.
    If the node does not exist, returns FALSE and does not do anything to the list.
*/
bool delete(LinkedList* ll, int idx){
    Node* node = findNode(ll, idx);

    

    if (node == NULL){
        return false;
    }
    else {
        Node* prev = node->prev;
        Node* next = node->next;

        if(prev != NULL) prev->next = next;
        if(next != NULL) next->prev = prev;

        if(node == ll->last) ll->last = prev;
        if(node == ll->first) ll->first = next;

        ll->size--;
        free(node);

        return true;
    } 
}

// Takes in a list and prints information about it, as well as its contents
void printList(LinkedList* l){
    printf("This list has %d nodes!\n", l->size);
    printf("The contents of nodes (as short values) are:\n\n");

    Node* curNode = l->first;
    
    while (curNode != NULL){
        short* data = curNode->data;

        for(int i = 0; i < DATASIZE / SHORTSIZE; i++){
            printf("%.6hd", data[i]);
            if (i != DATASIZE - 1) printf("\t");
        }

        printf("\n");

        curNode = curNode->next;
    }

    printf("\n");


    printf("This is the first node in the list: %p\n", l->first);
    printf("This is the last node in the list: %p\n", l->last);
}

// int main(int argc, char *argv[]) 
// {
//     LinkedList *l = createList();
//     add(l, 0, 0, 0, 0, 0);
//     add(l, 1, 1, 0, 0, 0);
//     add(l, 2, 65, 0, 0, 0);
//     add(l, 2, 0, 0, 0, 0);
//     add(l, -1, 3, 0, 0, 0);

//     printList(l);
    
//     bool del = delete(l, -1);

//     printList(l);

//     return 0;
// }