#include <stdio.h>
extern "C" {
#include "queue.h"
}
#include "queuecpp.h"
#include <time.h>
#include <math.h>

double ProfileTest(bool (*f)())
{
    clock_t latest = 0;
    double average = 0;
    double sum = 0;
    int count = 0;

    int countOfLowDeviation = 0;

    while(true)
    {
        clock_t start = clock();

        f();

        clock_t end = clock();
        clock_t diff = end - start;
        sum += diff;
        count++;
        double currAverage = sum / (double)count;
        if (average != 0)
        {
            double deviation = fabs((currAverage - average) / average);
            if (deviation < 0.001) {
                countOfLowDeviation++;
            }
            else {
                countOfLowDeviation = 0;
            }
            if (countOfLowDeviation > 3) {
                break;
            }
        }
        average = currAverage;
    }

    return average / CLOCKS_PER_SEC * 1000;
}

bool Test()
{
    const int size = 1000000;

    Queue* queue = QU_CreateQueue(size, 4);

    for (int i = 0; i < size; i++)
    {
        QU_Enqueue(queue, &i);
    }

    for (int i = 0; i < size; i++)
    {
        int result;
        QU_Dequeue(queue, &result);
        if (i != result) {
            return false;
        }
    }

    QU_DeleteQueue(queue);

    return true;
}

bool TestCpp()
{
    const int size = 1000000;

    Queue queue(size, 4);

    for (int i = 0; i < size; i++)
    {
        queue.Enqueue(&i);
    }

    for (int i = 0; i < size; i++)
    {
        int result;
        queue.Dequeue(&result);
        if (i != result) {
            return false;
        }
    }

    return true;
}

int main()
{
    double timeC = ProfileTest(Test);
    double timeCpp = ProfileTest(TestCpp);

    //bool ok = Test();
    printf("========== C %f msec Cpp %f\n", timeC, timeCpp); 
}
