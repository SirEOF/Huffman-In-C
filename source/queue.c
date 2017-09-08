#include "queue.h"

/* Priority queue implementations:
 *   Heap: tree data structure such as parent node always greater than child node.
 *         use an array, where root is at index 0, left child at index 1, 
 *         right child at index 2, left child of left child at index 3...
 *         Array can be dynamically resized.
 *         This is ascending heap or min heap has lowest priority element on top. It supports getMin,
 *         which is called Dequeue here
 */ 
 
//---------------------------------------------------------------------------------------- 
pPriorityQueue Init_PQ(int size)
{
    pPriorityQueue new_pq;
    new_pq = (pPriorityQueue) malloc(sizeof(PriorityQueue));
    new_pq->arr = (pElement) malloc(sizeof(Element) * size);
    new_pq->alloc = size;
    new_pq->size = 0;     
    return new_pq;
}
 
//---------------------------------------------------------------------------------------- 
void Enqueue(pPriorityQueue q, TYPEDATA data, int priority)
{
    int s, f; // indices to traverse tree (s son, f father)
  
    // check if we need to reallocate memory
    if (q->size >= q->alloc)
    {
        q->alloc *= 2;
        q->arr = (pElement) realloc(q->arr, sizeof(pElement) * q->alloc);
    }
    q->size++; // one more element in PQ
    s = q->size - 1; // new element is placed at bottom of tree/array
    f = (s-1)/2; //pq_array[f] is father of pq_array[s]
    while ((s > 0) && (priority < q->arr[f].priority))
    {
        q->arr[s] = q->arr[f]; // shift father down
        s = f; // son takes place of father
        f = (s-1)/2; // father at this new position of son
    }
    //place element here
    //printf("Inserting data %d and priority %d in heap at index %d\n", data, priority, s);
    q->arr[s].data = data;
    q->arr[s].priority = priority;   
}
 
//---------------------------------------------------------------------------------------- 
/* adjust tree that lost its root */
void pq_adjusttree(pPriorityQueue q)
{    
    int p;  // index to parent
    int s1; // index for son 1
    int s2; // index for son 2
	int size;
    int current;
  
    p = 0;
    s1 = 1;
    s2 = 2;
     
    // readjusting tree since root (pq_array[0]) will be deleted 
    do
    {
           // if only one son, or son 1 lower priority
           if ((s2 > q->size-1) || (q->arr[s1].priority < q->arr[s2].priority)) 
           {
               q->arr[p]=q->arr[s1]; // son 1 takes place of parent
               p = s1;
           }
           else
           {
               q->arr[p]=q->arr[s2]; // son 2 takes place of parent
               p = s2;
           }
           s1 = 2*p + 1;
           s2 = 2*p + 2;
    } while (s1 <= q->size-1); // parent doesn't have any children. we are done    
    //printf("priority last parent: %d\n", p);
 
    // rellocate nodes in array after last parent that was moved up
    size = q->size;
    current = p+1;
    while (current != size) 
    {
        q->size = current - 1;
        Enqueue(q, q->arr[current].data, q->arr[current].priority);
        current++;
    }      
    q->size = size-1; // one fewer element in heap    
    if (q->size < q->alloc/2 && q->size >= 4)
    {
        q->arr = (pElement) realloc(q->arr, sizeof(Element) * (q->alloc/2));
        q->alloc = q->alloc/2;
    }
}                       
 
//---------------------------------------------------------------------------------------- 
TYPEDATA Dequeue(pPriorityQueue q)
{
	TYPEDATA out;
    if (q->size == 0)
    {
        printf("Underflow error: trying to pop empty queue\n");
        return 0;
    }
    out = q->arr[0].data;
    if (q->size == 1)
        q->size = 0;
    else
        pq_adjusttree(q);
    return out;
}
 
