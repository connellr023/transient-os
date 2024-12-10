#include "mem_utils.hpp"

void *memcpy(void *dest, const void *src, size_t n) {
  char *d = static_cast<char *>(dest);
  const char *s = static_cast<const char *>(src);

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

#include "../drivers/uart0.hpp"

extern "C" void _test_thing(uint64_t *sp) {
  const uint64_t x0 = sp[0];
  const uint64_t x1 = sp[1];
  const uint64_t x2 = sp[2];

  uart0::puts("x0: ");
  uart0::hex(x0);
  uart0::puts("\n");

  uart0::puts("x1: ");
  uart0::hex(x1);
  uart0::puts("\n");

  uart0::puts("x2: ");
  uart0::hex(x2);
  uart0::puts("\n");
}