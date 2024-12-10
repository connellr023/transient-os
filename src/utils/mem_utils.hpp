#ifndef MEM_UTILS_HPP
#define MEM_UTILS_HPP

#include <stddef.h>
#include <stdint.h>

constexpr uint64_t mem_align_16(uint64_t value) { return (value + 0xf) & ~0xf; }

extern "C" {
void *memcpy(void *dest, const void *src, size_t n);
}

#endif // MEM_UTILS_HPP