#include "Queue.h"

#include <stdlib.h>
#include <string.h>

Queue* CreateQueue(int maxElementCount, int elementSize)
{
    Queue* queue = (Queue*)malloc(sizeof(struct Queue));
    if (queue == 0) {
        return NULL;
    }

    int result = pthread_mutex_init(&queue->_mutex, 0);
    if (result != 0)
    {
        free(queue);
        return NULL;
    }

    queue->_elementSize = elementSize;
    queue->_maxElementCount = maxElementCount;
    queue->_elementCount = 0;
    queue->_writeIndex = 0;
    queue->_readIndex = 0;

    queue->_elements = (char*)malloc(maxElementCount * (size_t) elementSize);
    if (queue->_elements == NULL)
    {
        (void)pthread_mutex_destroy(&queue->_mutex);
        return NULL;
    }

    return queue;
}

void DeleteQueue(Queue* queue)
{
    (void)pthread_mutex_destroy(&queue->_mutex);

    free(queue->_elements);
    free(queue);
}

int GetSize(Queue* queue)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return -1;
    }

    int count = queue->_elementCount;

    (void)pthread_mutex_unlock(&queue->_mutex);

    return count;
}

int Enqueue(Queue* queue, ElementPtr element)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return 0;
    }

    if (queue->_elementCount == queue->_maxElementCount)
    {
        (void)pthread_mutex_unlock(&queue->_mutex);

        return 0;
    }

    memcpy(queue->_elements + queue->_writeIndex * (size_t) queue->_elementSize, element, queue->_elementSize);

    queue->_writeIndex = (queue->_writeIndex + 1) % queue->_maxElementCount;

    queue->_elementCount++;

    (void)pthread_mutex_unlock(&queue->_mutex);

    return 1;
}

int Dequeue(Queue * queue, ElementPtr element)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return 0;
    }

    if (queue->_elementCount == 0)
    {
        (void)pthread_mutex_unlock(&queue->_mutex);
        return 0;
    }

    memcpy(element, queue->_elements + queue->_readIndex * (size_t) queue->_elementSize, queue->_elementSize);
    queue->_readIndex = (queue->_readIndex + 1) % queue->_maxElementCount;

    queue->_elementCount--;

    (void)pthread_mutex_unlock(&queue->_mutex);
    return 1;
}