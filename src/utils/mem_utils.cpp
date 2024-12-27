#include "../../include/utils/mem_utils.hpp"

namespace utils {
uint32_t atomic_swap(volatile uint32_t *addr, uint32_t new_value) {
  uint32_t old_value;
  uint32_t temp;

  asm volatile(
      "1: ldxr    %w0, [%2]       \n" // Load the current value (exclusive)
      "   stxr    %w1, %w3, [%2] \n"  // Try to store the new value (exclusive)
      "   cbnz    %w1, 1b        \n"  // If store failed, retry
      : "=&r"(old_value), "=&r"(temp) // Output operands
      : "r"(addr), "r"(new_value)     // Input operands
      : "memory"                      // Clobbered registers
  );

  return old_value; // Return the original value
}

void *memcpy(void *dest, const void *src, size_t n) {
  char *csrc = reinterpret_cast<char *>(const_cast<void *>(src));
  char *cdest = reinterpret_cast<char *>(dest);

  for (size_t i = 0; i < n; i++) {
    cdest[i] = csrc[i];
  }

  return dest;
}
} // namespace utils