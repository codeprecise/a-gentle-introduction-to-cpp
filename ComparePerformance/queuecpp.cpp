#include "queuecpp.h"

#include <stdlib.h>
#include <memory.h>

Queue::Queue(int maxElementCount, int elementSize)
{
    _elementSize = elementSize;
    _maxElementCount = maxElementCount;
    _elementCount = 0;
    _writeIndex = 0;
    _readIndex = 0;
    _elements = (char*)malloc(maxElementCount * elementSize);
}

Queue::~Queue()
{
    free(_elements);
}

int Queue::GetElementCount()
{
    return _elementCount;
}

int Queue::GetMaxElementCount()
{
    return _maxElementCount;
}

char* Queue::GetPtr( int index)
{
    return _elements + index * _elementSize;
}

void Queue::IncrementIndex( int* index)
{
    *index = (*index + 1) % _maxElementCount;
}

bool Queue::Enqueue( ElementPtr element)
{
    if (_elementCount == _maxElementCount)
    {
        return false;
    }

    memcpy(GetPtr(_writeIndex), element, _elementSize);
    IncrementIndex(&_writeIndex);
    
    _elementCount++;

    return true;
}

bool Queue::Dequeue( ElementPtr element)
{
    if (_elementCount == 0)
    {
        return false;
    }

    memcpy(element, GetPtr(_readIndex), _elementSize);
    IncrementIndex(&_readIndex);

    _elementCount--;

    return true;
}

ElementPtr Queue::Peek()
{
    if (_elementCount == 0)
    {
        return 0;
    }

    return GetPtr(_readIndex);
}

