#include "Queue.h"
#include "MutexLockGuard.h"

#include <stdlib.h>
#include <string.h>

Queue::Queue(int maxElementCount, int elementSize) :
    _elementSize (elementSize),
    _maxElementCount (maxElementCount),
    _elementCount (0),
    _writeIndex (0),
    _readIndex (0),
    _elements ((char*)malloc(maxElementCount * (size_t)elementSize))
{
}

Queue::~Queue()
{
    free(_elements);
}

int Queue::GetSize()
{
    MutexLockGuard guard(&_mutex);

    int count = _elementCount;

    return count;
}

bool Queue::Enqueue(ElementPtr element)
{
    MutexLockGuard guard(&_mutex);

    if (_elementCount == _maxElementCount)
    {
        return false;
    }

    memcpy(_elements + _writeIndex * (size_t)_elementSize, element, _elementSize);
    IncrementIndex(&_writeIndex);

    _elementCount++;

    return true;
}

bool Queue::Dequeue(ElementPtr element)
{
    MutexLockGuard guard(&_mutex);

    if (_elementCount == 0)
    {
        return false;
    }

    memcpy(element, _elements + _readIndex * (size_t)_elementSize, _elementSize);
    IncrementIndex(&_readIndex);

    _elementCount--;

    return true;
}

void Queue::IncrementIndex(int* index)
{
    *index = (*index + 1) % _maxElementCount;
}