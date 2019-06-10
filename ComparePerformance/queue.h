#pragma once
#include <stdbool.h>

typedef struct Queue Queue;

typedef void* ElementPtr;

Queue* QU_CreateQueue(int maxElementCount, int elementSize);
void QU_DeleteQueue(Queue* queue);
int QU_GetElementCount(Queue* queue);
int QU_GetMaxElementCount(Queue* queue);
bool QU_Enqueue(Queue* queue, ElementPtr element);
bool QU_Dequeue(Queue* queue, ElementPtr element);
ElementPtr QU_Peek(Queue* queue);


