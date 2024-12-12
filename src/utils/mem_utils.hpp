#ifndef MEM_UTILS_HPP
#define MEM_UTILS_HPP

#include <stddef.h>
#include <stdint.h>

extern "C" {
void *memcpy(void *dest, const void *src, size_t n);
void memset(void *dest, uint8_t val, size_t n);
}

#endif // MEM_UTILS_HPP