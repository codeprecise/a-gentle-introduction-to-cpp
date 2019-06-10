#pragma once

#include <pthread.h>

typedef void *(*StartRoutine) (void *);

class Thread 
{
public:
    Thread(StartRoutine startRoutine, void* arg)
    {
        int result = pthread_create(&_threadId, nullptr, startRoutine, arg);
        (void)result;
        // TODO: error handling with exceptions
    }

    void* Join() 
    {
        void* routineResult = nullptr;
        (void)pthread_join(_threadId, &routineResult);
        return routineResult;
    }

private:

    pthread_t _threadId;
};
