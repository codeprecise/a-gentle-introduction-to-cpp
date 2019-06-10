#pragma once

#include "Mutex.h"

typedef void* ElementPtr;

class Queue
{
public:

    Queue(int maxElementCount, int elementSize);
    ~Queue();
    
    int GetSize();
    bool Enqueue(ElementPtr element);
    bool Dequeue(ElementPtr element);

private:

    void IncrementIndex(int* index);

    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;

    Mutex _mutex;
};
