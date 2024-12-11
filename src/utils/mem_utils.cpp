#include "mem_utils.hpp"

void *memcpy(void *dest, const void *src, size_t n) {
  char *d = static_cast<char *>(dest);
  const char *s = static_cast<const char *>(src);

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

// #include "../drivers/uart0.hpp"

// extern "C" void _test_thing(uint64_t *sp) {
//   const uint64_t x0 = sp[0];
//   const uint64_t x1 = sp[1];
//   const uint64_t lr = sp[30];

//   uart0::puts("Test thing\n");
//   uart0::puts("sp: ");
//   uart0::hex(reinterpret_cast<uint64_t>(sp));
//   uart0::puts("\n");

//   // Print x0 to x30
//   for (int i = 0; i < 31; ++i) {
//     uart0::puts("x");
//     uart0::hex(i);
//     uart0::puts(": ");
//     uart0::hex(sp[i]);
//     uart0::puts("\n");
//   }
// }