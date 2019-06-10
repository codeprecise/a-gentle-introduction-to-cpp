#pragma once

#include <pthread.h>

typedef void* ElementPtr;

typedef struct Queue
{
    Queue(int maxElementCount, int elementSize);
    ~Queue();
    
    int GetSize();
    int Enqueue(ElementPtr element);
    int Dequeue(ElementPtr element);

    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;

    pthread_mutex_t _mutex;
} Queue;
