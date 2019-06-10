#pragma once
#include <stdbool.h>

typedef void* ElementPtr;

typedef struct Queue
{
    Queue(int maxElementCount, int elementSize);
    ~Queue();

    int GetElementCount();
    int GetMaxElementCount();
    bool Enqueue(ElementPtr element);
    bool Dequeue(ElementPtr element);
    ElementPtr Peek();

private:

    char* GetPtr(int index);
    void IncrementIndex(int* index);

    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;

} Queue;



