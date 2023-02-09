#pragma once

#include <cstdlib>
#include <cstdint>
#include <ctime>

struct Rand
{
    static uint32_t state;
    static uint32_t Get();
    static float Get_r();
    static float FloatRange(float,float);
    static int IntRange(int,int);
};