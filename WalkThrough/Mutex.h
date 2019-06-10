#pragma once

#include <pthread.h>

class Mutex
{
public:

    Mutex()
    {
        int result = pthread_mutex_init(&_mutex, nullptr);
        if (result != 0) {
            // Handle errors in constructors is often done using exceptions
            // This is an advanced topic.
            // throw std::runtime_error("pthread_mutex_init");
        }
    }

    ~Mutex()
    {
        (void)pthread_mutex_destroy(&_mutex);
    }

    void Lock()
    {
        int result = pthread_mutex_lock(&_mutex);
        if (result != 0) {
            // throw std::runtime_error("pthread_mutex_lock");
        }
    }

    void Unlock()
    {
        (void)pthread_mutex_unlock(&_mutex);
    }

private:
    pthread_mutex_t _mutex;
};
