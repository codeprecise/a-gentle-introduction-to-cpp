#pragma once

#include <pthread.h>

typedef void* ElementPtr;

typedef struct Queue
{
    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;

    pthread_mutex_t _mutex;
} Queue;

Queue* CreateQueue(int maxElementCount, int elementSize);
void DeleteQueue(Queue* queue);
int GetSize(Queue* queue);
int Enqueue(Queue* queue, ElementPtr element);
int Dequeue(Queue* queue, ElementPtr element);
