#include "Queue.h"
#include "Thread.h"

class Test
{
public:

    Test();

    bool IsOk();
    bool RunThreads();
    bool IsPassed();

private:

    void ProducerMethod();
    void ConsumerMethod();
    static void* ProducerMethodStatic(void* arg);
    static void* ConsumerMethodStatic(void* arg);

    const int size = 1000;

    Queue sourceQueue;
    Queue evenQueue;
    Queue oddQueue;

    volatile int shouldExit = 0;
};
