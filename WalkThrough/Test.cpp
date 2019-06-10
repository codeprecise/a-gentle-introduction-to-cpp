#include "Test.h"

#include <unistd.h>
#include <time.h>

Test::Test() :
    sourceQueue(size, sizeof(int)),
    evenQueue(size, sizeof(int) / 2),
    oddQueue(size, sizeof(int) / 2)
{
}

bool Test::IsOk()
{
    return true;
}

void Test::ProducerMethod()
{
    for (int i = 0; (i < size) && !shouldExit; i++)
    {
        int rand = clock() % 7;
        usleep(rand * 1000);
        sourceQueue.Enqueue(&i);
    }
}

void Test::ConsumerMethod()
{
    while (!shouldExit)
    {
        int rand = clock() % 7;
        usleep(rand * 1000);

        int value;
        int gotValue = sourceQueue.Dequeue(&value);
        if (gotValue)
        {
            if (value % 2 == 0)
            {
                evenQueue.Enqueue(&value);
            }
            else
            {
                oddQueue.Enqueue(&value);
            }
        }
        else if (evenQueue.GetSize() + oddQueue.GetSize() == size)
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
    int evenSize = evenQueue.GetSize();
    int oddSize = oddQueue.GetSize();

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
        evenQueue.Dequeue(&value);
        sum += value;
    }

    for (int i = 0; i < oddSize; i++)
    {
        oddQueue.Dequeue(&value);
        sum += value;
    }

    if (sum != (0 + size - 1) * size / 2) {
        return false;
    }

    return true;
}
