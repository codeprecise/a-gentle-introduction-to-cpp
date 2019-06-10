#include "Queue.h"
#include "Thread.h"

class Test
{
public:

    Test();
    ~Test();

    bool IsOk();
    bool RunThreads();
    bool IsPassed();

private:

    void ProducerMethod();
    void ConsumerMethod();
    static void* ProducerMethodStatic(void* arg);
    static void* ConsumerMethodStatic(void* arg);

    const int size = 1000;

    Queue* sourceQueue = NULL;
    Queue* evenQueue = NULL;
    Queue* oddQueue = NULL;

    volatile int shouldExit = 0;
};
