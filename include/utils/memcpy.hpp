#ifndef MEMCPY_HPP
#define MEMCPY_HPP

#include <stddef.h>

extern "C" {
void *memcpy(void *dest, const void *src, size_t n);
}

#endif // MEMCPY_HPP