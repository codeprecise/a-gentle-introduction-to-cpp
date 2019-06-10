#include "Queue.h"
#include "Thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

class Test
{
public:

    const int size = 1000;

    Queue* sourceQueue = NULL;
    Queue* evenQueue = NULL;
    Queue* oddQueue = NULL;

    volatile int shouldExit = 0;

    Test()
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

    ~Test()
    {
        if (sourceQueue) {
            delete sourceQueue;
        }
     
        if (evenQueue) {
            delete evenQueue;
        }

        if (sourceQueue) {
            delete oddQueue;
        }
    }

    bool IsOk()
    {
        return sourceQueue != NULL;
    }

    void ProducerMethod()
    {
        for (int i = 0; (i < size) && !shouldExit; i++)
        {
            int rand = clock() % 7;
            usleep(rand * 1000);
            sourceQueue->Enqueue(&i);
        }
    }

    void ConsumerMethod()
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

    static void* ProducerMethodStatic(void* arg)
    {
        ((Test*)arg)->ProducerMethod();
        return NULL;
    }

    static void* ConsumerMethodStatic(void* arg)
    {
        ((Test*)arg)->ConsumerMethod();
        return NULL;
    }

    bool RunThreads()
    {
        Thread producerThread(ProducerMethodStatic, this);
        Thread consumer1Thread(ConsumerMethodStatic, this);
        Thread consumer2Thread(ConsumerMethodStatic, this);

        producerThread.Join();
        consumer1Thread.Join();
        consumer2Thread.Join();

        return IsPassed();
    }

    bool IsPassed()
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
};

int main()
{
    Test test;

    if (!test.IsOk()) {
        return EXIT_FAILURE;
    }

    bool passed = test.RunThreads();

    printf("=========== %s\n", passed ? "PASSED" : "FAILED");

    return EXIT_SUCCESS;
}
