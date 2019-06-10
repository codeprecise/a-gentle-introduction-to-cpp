#include "Queue.h"
#include "Thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int size = 1000;

Queue* sourceQueue = NULL;
Queue* evenQueue = NULL;
Queue* oddQueue = NULL;

volatile int shouldExit = 0;

int Initialize()
{
    sourceQueue = new Queue(size, sizeof(int));
    if (!sourceQueue) {
        return 0;
    }
    evenQueue = new Queue(size / 2, sizeof(int));
    if (!evenQueue) {
        return 0;
    }
    oddQueue = new Queue(size / 2, sizeof(int));
    if (!oddQueue) {
        return 0;
    }
    return 1;
}

void Release()
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

void* ProducerMethod(void* arg)
{
    (void) arg;
    for (int i = 0; (i < size) && !shouldExit; i++)
    {
        int rand = clock() % 7;
        usleep(rand * 1000);
        sourceQueue->Enqueue(&i);
    }

    return NULL;
}

void* ConsumerMethod(void* arg)
{
    (void)arg;
    while (! shouldExit)
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

    return NULL;
}

int main()
{
    int sum = 0;
    int value = 0;
    int passed = 0;
    int result = 0;

    int ok = Initialize();
    if (! ok) {
        goto Error;
    }

    {
        Thread producerThread(ProducerMethod, NULL);
        Thread consumer1Thread(ConsumerMethod, NULL);
        Thread consumer2Thread(ConsumerMethod, NULL);

        producerThread.Join();
        consumer1Thread.Join();
        consumer2Thread.Join();
    }

    passed =
        evenQueue->GetSize() == (size_t)(size / 2) &&
        oddQueue->GetSize() == (size_t)(size / 2);

    if (passed)
    {
        int evenSize = evenQueue->GetSize();
        for (int i = 0; i < evenSize; i++)
        {
            evenQueue->Dequeue(&value);
            sum += value;
        }

        int oddSize = oddQueue->GetSize();
        for (int i = 0; i < oddSize; i++)
        {
            oddQueue->Dequeue(&value);
            sum += value;
        }

        if (sum != (0 + size - 1) * size / 2) {
            passed = 0;
        }
    }

    printf("=========== %s\n", passed ? "PASSED" : "FAILED");

    Release();

    return EXIT_SUCCESS;

Error:
    Release();
    return EXIT_FAILURE;
}
