#include "Queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int size = 1000;

Queue* sourceQueue = NULL;
Queue* evenQueue = NULL;
Queue* oddQueue = NULL;

volatile int shouldExit = 0;
pthread_t producerThread = 0;
pthread_t consumer1Thread = 0;
pthread_t consumer2Thread = 0;

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
    
    if (producerThread) {
        pthread_join(producerThread, NULL);
    }
    
    if (consumer1Thread) {
        pthread_join(consumer1Thread, NULL);
    }

    if (consumer2Thread) {
        pthread_join(consumer2Thread, NULL);
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

    result = pthread_create(&producerThread, NULL, ProducerMethod, NULL);
    if (result != 0) {
        goto Error;
    }

    result = pthread_create(&consumer1Thread, NULL, ConsumerMethod, NULL);
    if (result != 0) {
        goto Error;
    }

    result = pthread_create(&consumer2Thread, NULL, ConsumerMethod, NULL);
    if (result != 0) {
        goto Error;
    }

    result = pthread_join(producerThread, NULL);
    if (result != 0) {
        goto Error;
    }
    producerThread = 0;

    result = pthread_join(consumer1Thread, NULL);
    if (result != 0) {
        goto Error;
    }
    consumer1Thread = 0;

    result = pthread_join(consumer2Thread, NULL);
    if (result != 0) {
        goto Error;
    }
    consumer2Thread = 0;

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
