#pragma once

#include <cstdlib>
#include <cstdint>
#include <ctime>

struct Rand
{
    static uint32_t state;
    static uint32_t Get();
};