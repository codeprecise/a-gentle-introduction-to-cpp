#include "Queue.h"

#include <stdlib.h>
#include <string.h>

Queue::Queue(int maxElementCount, int elementSize)
{
    int result = pthread_mutex_init(&_mutex, 0);
    if (result != 0)
    {
        return;
    }

    _elementSize = elementSize;
    _maxElementCount = maxElementCount;
    _elementCount = 0;
    _writeIndex = 0;
    _readIndex = 0;

    _elements = (char*)malloc(maxElementCount * (size_t) elementSize);
    if (_elements == NULL)
    {
        (void)pthread_mutex_destroy(&_mutex);
    }
}

Queue::~Queue()
{
    (void)pthread_mutex_destroy(&_mutex);

    free(_elements);
}

int Queue::GetSize()
{
    int result = pthread_mutex_lock(&_mutex);
    if (result != 0) {
        return -1;
    }

    int count = _elementCount;

    (void)pthread_mutex_unlock(&_mutex);

    return count;
}

int Queue::Enqueue(ElementPtr element)
{
    int result = pthread_mutex_lock(&_mutex);
    if (result != 0) {
        return 0;
    }

    if (_elementCount == _maxElementCount)
    {
        (void)pthread_mutex_unlock(&_mutex);

        return 0;
    }

    memcpy(_elements + _writeIndex * (size_t) _elementSize, element, _elementSize);
    IncrementIndex(&_writeIndex);

    _elementCount++;

    (void)pthread_mutex_unlock(&_mutex);

    return 1;
}

int Queue::Dequeue(ElementPtr element)
{
    int result = pthread_mutex_lock(&_mutex);
    if (result != 0) {
        return 0;
    }

    if (_elementCount == 0)
    {
        (void)pthread_mutex_unlock(&_mutex);
        return 0;
    }

    memcpy(element, _elements + _readIndex * (size_t) _elementSize, _elementSize);
    IncrementIndex(&_readIndex);

    _elementCount--;

    (void)pthread_mutex_unlock(&_mutex);
    return 1;
}

void Queue::IncrementIndex(int* index)
{
    *index = (*index + 1) % _maxElementCount;
}