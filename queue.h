#ifndef _QUEUE
#define _QUEUE

#include "tree.h"
 
typedef pTreeNode TYPEDATA;

typedef struct Element
{
    TYPEDATA data; // this can be any data type
    int priority;
} Element, *pElement;
 
typedef struct PriorityQueue
{
    Element *arr;         // array of pq_element-s
    int size;                // number of elements in Priority Queue
    int alloc;            // number of elements memory was allocated for
} PriorityQueue, *pPriorityQueue;

 
//-------------------   PROTOTYPE  ------------------------------------------------------------
pPriorityQueue Init_PQ(int size);
void Enqueue(pPriorityQueue q, TYPEDATA data, int priority);
TYPEDATA Dequeue(pPriorityQueue q);

#endif 

 
