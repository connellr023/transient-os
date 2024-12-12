#include "mem_utils.hpp"

void *memcpy(void *dest, const void *src, size_t n) {
  char *d = static_cast<char *>(dest);
  const char *s = static_cast<const char *>(src);

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

void memset(void *dest, uint8_t val, size_t n) {
  char *d = static_cast<char *>(dest);

  while (n--) {
    *d++ = val;
  }
}