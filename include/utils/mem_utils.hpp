#ifndef MEM_UTILS_HPP
#define MEM_UTILS_HPP

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Aligns a value to the next multiple of the alignment.
 */
constexpr uint64_t align_up(uint64_t value) {
  return (value + alignof(max_align_t) - 1) & ~(alignof(max_align_t) - 1);
}

extern "C" {
/**
 * @brief Copies n bytes from src to dest.
 */
void *memcpy(void *dest, const void *src, size_t n);
}

#endif // MEM_UTILS_HPP