#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef void* ElementPtr;

typedef struct Queue
{
    int _elementSize;
    int _maxElementCount;
    int _elementCount;
    int _writeIndex;
    int _readIndex;
    char* _elements;

    pthread_mutex_t _mutex;
} Queue;

Queue* CreateQueue(int maxElementCount, int elementSize)
{
    Queue* queue = (Queue*)malloc(sizeof(struct Queue));

    int result = pthread_mutex_init(&queue->_mutex, 0);
    if (result != 0)
    {
        free(queue);
        return NULL;
    }

    queue->_elementSize = elementSize;
    queue->_maxElementCount = maxElementCount;
    queue->_elementCount = 0;
    queue->_writeIndex = 0;
    queue->_readIndex = 0;
    
    queue->_elements = (char*)malloc(maxElementCount * elementSize);
    if (queue->_elements == NULL)
    {
        (void)pthread_mutex_destroy(&queue->_mutex);
        return NULL;
    }

    return queue;
}

void DeleteQueue(Queue* queue)
{
    (void)pthread_mutex_destroy(&queue->_mutex);

    free(queue->_elements);
    free(queue);
}

int GetSize(Queue* queue)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return -1;
    }

    int count = queue->_elementCount;

    (void)pthread_mutex_unlock(&queue->_mutex);

    return count;
}

int Enqueue(Queue* queue, ElementPtr element)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return 0;
    }

    if (queue->_elementCount == queue->_maxElementCount)
    {
        (void)pthread_mutex_unlock(&queue->_mutex);

        return 0;
    }

    memcpy(queue->_elements + queue->_writeIndex * queue->_elementSize, element, queue->_elementSize);

    queue->_writeIndex = (queue->_writeIndex + 1) % queue->_maxElementCount;

    queue->_elementCount++;

    (void)pthread_mutex_unlock(&queue->_mutex);

    return 1;
}

int Dequeue(Queue * queue, ElementPtr element)
{
    int result = pthread_mutex_lock(&queue->_mutex);
    if (result != 0) {
        return 0;
    }

    if (queue->_elementCount == 0)
    {
        (void)pthread_mutex_unlock(&queue->_mutex);
        return 0;
    }

    memcpy(element, queue->_elements + queue->_readIndex * queue->_elementSize, queue->_elementSize);
    queue->_readIndex = (queue->_readIndex + 1) % queue->_maxElementCount;

    queue->_elementCount--;

    (void)pthread_mutex_unlock(&queue->_mutex);
    return 1;
}

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
    sourceQueue = CreateQueue(size, sizeof(int));
    if (!sourceQueue) {
        return 0;
    }
    evenQueue = CreateQueue(size / 2, sizeof(int));
    if (!evenQueue) {
        return 0;
    }
    oddQueue = CreateQueue(size / 2, sizeof(int));
    if (!oddQueue) {
        return 0;
    }
    return 1;
}

void Release()
{
    if (sourceQueue) {
        DeleteQueue(oddQueue);
    }
    if (evenQueue) {
        DeleteQueue(evenQueue);
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
        Enqueue(sourceQueue, &i);
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
        int gotValue = Dequeue(sourceQueue, &value);
        if (gotValue)
        {
            if (value % 2 == 0)
            {
                Enqueue(evenQueue, &value);
            }
            else
            {
                Enqueue(oddQueue, &value);
            }
        }
        else if (GetSize(evenQueue) + GetSize(oddQueue) == size)
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
        GetSize(evenQueue) == (size_t)(size / 2) &&
        GetSize(oddQueue) == (size_t)(size / 2);

    if (passed)
    {
        int evenSize = GetSize(evenQueue);
        for (int i = 0; i < evenSize; i++)
        {
            Dequeue(evenQueue, &value);
            sum += value;
        }

        int oddSize = GetSize(oddQueue);
        for (int i = 0; i < oddSize; i++)
        {
            Dequeue(oddQueue, &value);
            sum += value;
        }

        if (sum != (0 + size - 1) * size / 2) {
            passed = 0;
        }
    }

    printf("=========== %s\n", passed ? "PASSED" : "FAILED");

    return EXIT_SUCCESS;

Error:
    Release();
    return EXIT_FAILURE;
}
