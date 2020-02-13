#include "of1275.h"

uint32_t ___le32_to_cpu(uint32_t x)
{
	return (((x & 0x000000ffU) << 24) |
		((x & 0x0000ff00U) <<  8) |
		((x & 0x00ff0000U) >>  8) |
		((x & 0xff000000U) >> 24));
}

uint64_t ____le64_to_cpu(uint64_t x)
{
	return (((x & 0x000000ffU) << 24) |
		((x & 0x0000ff00U) <<  8) |
		((x & 0x00ff0000U) >>  8) |
		((x & 0xff000000U) >> 24));
}

