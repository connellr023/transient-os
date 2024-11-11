#define SYS_DEBUG
#ifdef SYS_DEBUG

#include "../drivers/uart0.hpp"
#include <stdbool.h>

extern "C" void print_current_el() {
  uart0::init();

  uint64_t current_el;
  asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
  current_el >>= 2;

  for (int i = 0; i < 150; i++) {
    uart0::puts("CurrentEL: 0x");

    if (current_el == 0) {
      uart0::puts("0");
    } else if (current_el == 1) {
      uart0::puts("1");
    } else if (current_el == 2) {
      uart0::puts("2");
    } else if (current_el == 3) {
      uart0::puts("3");
    } else {
      uart0::puts("?");
    }

    uart0::send('|');
    uart0::send('\n');
  }
}

extern "C" void print_as_hex(uint64_t v1, uint64_t v2, uint64_t v3,
                             uint64_t v4) {
  uart0::init();

  for (int i = 0; i < 100; i++) {
    uart0::puts("sctlr_el1: 0x");
    uart0::hex(v1);

    uart0::puts(" hcr_el2: 0x");
    uart0::hex(v2);

    uart0::puts(" scr_el3: 0x");
    uart0::hex(v3);

    uart0::puts(" spsr_el3: 0x");
    uart0::hex(v4);
    uart0::send('|');
    uart0::send('\n');
  }
}

extern "C" void say_hello() {
  uart0::init();

  for (int i = 0; i < 100; i++) {
    uart0::puts("Hello, World!\n");
  }
}

extern "C" void cmp_address(void *a, void *b) {
  uart0::init();

  for (int i = 0; i < 100; i++) {
    if (a == b) {
      uart0::hex(reinterpret_cast<uint64_t>(a));
      uart0::puts(" == ");
      uart0::hex(reinterpret_cast<uint64_t>(b));
    } else {
      uart0::hex(reinterpret_cast<uint64_t>(a));
      uart0::puts(" != ");
      uart0::hex(reinterpret_cast<uint64_t>(b));
    }

    uart0::send('\n');
  }
}

#endif // SYS_DEBUG