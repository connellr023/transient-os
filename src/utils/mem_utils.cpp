#include "../../include/utils/mem_utils.hpp"

void *memcpy(void *dest, const void *src, size_t n) {
  char *csrc = reinterpret_cast<char *>(const_cast<void *>(src));
  char *cdest = reinterpret_cast<char *>(dest);

  for (size_t i = 0; i < n; i++) {
    cdest[i] = csrc[i];
  }

  return dest;
}