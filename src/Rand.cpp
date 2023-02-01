#include "Rand.h"
#include <limits>

uint32_t Rand::state = time(NULL);

uint32_t Rand::Get()
{
    uint64_t product = (uint64_t)state * 48271;
	uint32_t x = (product & 0x7fffffff) + (product >> 31);

	x = (x & 0x7fffffff) + (x >> 31);
	return state = x;
}

float Rand::Get_r()
{
	return ((double)Rand::Get()) / ((double)std::numeric_limits<int>::max());;
}