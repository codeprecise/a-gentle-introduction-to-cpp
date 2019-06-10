#pragma once

#include <Windows.h>

typedef void* pthread_mutexattr_t;
typedef CRITICAL_SECTION pthread_mutex_t;

inline int pthread_mutex_lock(pthread_mutex_t* m)
{
    EnterCriticalSection(m);
    return 0;
}

inline int pthread_mutex_unlock(pthread_mutex_t* m)
{
    LeaveCriticalSection(m);
    return 0;
}

inline int pthread_mutex_trylock(pthread_mutex_t* m)
{
    return TryEnterCriticalSection(m) ? 0 : EBUSY;
}

inline int pthread_mutex_init(pthread_mutex_t* m, pthread_mutexattr_t* a)
{
    (void)a;
    InitializeCriticalSection(m);

    return 0;
}

inline int pthread_mutex_destroy(pthread_mutex_t* m)
{
    DeleteCriticalSection(m);
    return 0;
}

typedef HANDLE pthread_t;
typedef int pthread_attr_t;

inline int pthread_create(pthread_t* th, pthread_attr_t* attr, void* (*func)(void*), void* arg)
{
    DWORD threadId;
    *th = CreateThread(
        NULL,                           // default security attributes
        0,                              // use default stack size  
        (LPTHREAD_START_ROUTINE)func,   // thread function name
        arg,                            // argument to thread function 
        0,                              // use default creation flags 
        &threadId);                     // returns the thread iden
    return *th == NULL ? -1 : 0;
}

inline int pthread_join(pthread_t th, void** ppresult)
{
    WaitForSingleObject(th, INFINITE);
    return 0;
}
