#pragma once

#include <Windows.h>

inline void usleep(int usecs)
{
    Sleep(usecs / 1000);
}