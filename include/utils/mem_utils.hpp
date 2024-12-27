/*
 * Copyright (C) 2024/2025 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef MEM_UTILS_HPP
#define MEM_UTILS_HPP

#include <stddef.h>
#include <stdint.h>

namespace utils {
/**
 * @brief Aligns a value to the next multiple of the alignment.
 */
constexpr uint64_t align_up(uint64_t value) {
  return (value + alignof(max_align_t) - 1) & ~(alignof(max_align_t) - 1);
}

uint32_t atomic_swap(volatile uint32_t *addr, uint32_t new_value);

extern "C" {
/**
 * @brief Copies n bytes from src to dest.
 */
void *memcpy(void *dest, const void *src, size_t n);
}
} // namespace utils

#endif // MEM_UTILS_HPP