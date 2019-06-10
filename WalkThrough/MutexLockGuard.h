#pragma once

#include "Mutex.h"

class MutexLockGuard
{
public:
    MutexLockGuard(Mutex* mutex) :
        _mutex(mutex)
    {
        _mutex->Lock();
    }

    ~MutexLockGuard()
    {
        _mutex->Unlock();
    }

private:
    Mutex* _mutex;
};
