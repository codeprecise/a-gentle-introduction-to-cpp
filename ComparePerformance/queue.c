#include "queue.h"

#include <stdlib.h>
#include <memory.h>

struct Queue
{
    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;
};

Queue* QU_CreateQueue(int maxElementCount, int elementSize)
{
    Queue* queue = (Queue*)malloc(sizeof(struct Queue));
    queue->_elementSize = elementSize;
    queue->_maxElementCount = maxElementCount;
    queue->_elementCount = 0;
    queue->_writeIndex = 0;
    queue->_readIndex = 0;
    queue->_elements = (char*)malloc(maxElementCount * elementSize);
    return queue;
}

void QU_DeleteQueue(Queue* queue)
{
    free(queue->_elements);
    free(queue);
}

int QU_GetElementCount(Queue* queue)
{
    return queue->_elementCount;
}

int QU_GetMaxElementCount(Queue* queue)
{
    return queue->_maxElementCount;
}

char* GetPtr(Queue* queue, int index)
{
    return queue->_elements + index * queue->_elementSize;
}

void IncrementIndex(Queue* queue, int* index)
{
    *index = (*index + 1) % queue->_maxElementCount;
}

bool QU_Enqueue(Queue* queue, ElementPtr element)
{
    if (queue->_elementCount == queue->_maxElementCount)
    {
        return false;
    }

    memcpy(GetPtr(queue, queue->_writeIndex), element, queue->_elementSize);
    IncrementIndex(queue, &queue->_writeIndex);
    
    queue->_elementCount++;

    return true;
}

bool QU_Dequeue(Queue* queue, ElementPtr element)
{
    if (queue->_elementCount == 0)
    {
        return false;
    }

    memcpy(element, GetPtr(queue, queue->_readIndex), queue->_elementSize);
    IncrementIndex(queue, &queue->_readIndex);

    queue->_elementCount--;

    return true;
}

ElementPtr QU_Peek(Queue* queue)
{
    if (queue->_elementCount == 0)
    {
        return 0;
    }

    return GetPtr(queue, queue->_readIndex);
}

