#include "Test.h"

#include <unistd.h>
#include <time.h>

Test::Test()
{
    sourceQueue = new Queue(size, sizeof(int));
    if (!sourceQueue) {
        return;
    }

    evenQueue = new Queue(size / 2, sizeof(int));
    if (!evenQueue) {
        return;
    }
    
    oddQueue = new Queue(size / 2, sizeof(int));
    if (!oddQueue) {
        return;
    }
}

Test::~Test()
{
    if (sourceQueue) {
        delete sourceQueue;
    }
     
    if (evenQueue) {
        delete evenQueue;
    }

    if (oddQueue) {
        delete oddQueue;
    }
}

bool Test::IsOk()
{
    return sourceQueue != NULL;
}

void Test::ProducerMethod()
{
    for (int i = 0; (i < size) && !shouldExit; i++)
    {
        int rand = clock() % 7;
        usleep(rand * 1000);
        sourceQueue->Enqueue(&i);
    }
}

void Test::ConsumerMethod()
{
    while (!shouldExit)
    {
        int rand = clock() % 7;
        usleep(rand * 1000);

        int value;
        int gotValue = sourceQueue->Dequeue(&value);
        if (gotValue)
        {
            if (value % 2 == 0)
            {
                evenQueue->Enqueue(&value);
            }
            else
            {
                oddQueue->Enqueue(&value);
            }
        }
        else if (evenQueue->GetSize() + oddQueue->GetSize() == size)
        {
            break;
        }
    }
}

void* Test::ProducerMethodStatic(void* arg)
{
    ((Test*)arg)->ProducerMethod();
    return NULL;
}

void* Test::ConsumerMethodStatic(void* arg)
{
    ((Test*)arg)->ConsumerMethod();
    return NULL;
}

bool Test::RunThreads()
{
    Thread producerThread(ProducerMethodStatic, this);
    Thread consumer1Thread(ConsumerMethodStatic, this);
    Thread consumer2Thread(ConsumerMethodStatic, this);

    producerThread.Join();
    consumer1Thread.Join();
    consumer2Thread.Join();

    return IsPassed();
}

bool Test::IsPassed()
{
    int evenSize = evenQueue->GetSize();
    int oddSize = oddQueue->GetSize();

    bool passed =
        evenSize == (size_t)(size / 2) &&
        oddSize == (size_t)(size / 2);

    if (!passed)
    {
        return false;
    }

    int sum = 0;
    int value = 0;

    for (int i = 0; i < evenSize; i++)
    {
        evenQueue->Dequeue(&value);
        sum += value;
    }

    for (int i = 0; i < oddSize; i++)
    {
        oddQueue->Dequeue(&value);
        sum += value;
    }

    if (sum != (0 + size - 1) * size / 2) {
        return false;
    }

    return true;
}
