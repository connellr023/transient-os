#include "kernel.hpp"
#include <stdint.h>

__attribute__((always_inline)) inline void kernel::interrupt_entry() {
  asm volatile("\n\t"
               "\n\t"
               "stp x6, x7, [sp, #16 * 3]\n\t"
               "stp x8, x9, [sp, #16 * 4]\n\t"
               "stp x10, x11, [sp, #16 * 5]\n\t"
               "stp x12, x13, [sp, #16 * 6]\n\t"
               "stp x14, x15, [sp, #16 * 7]\n\t"
               "stp x16, x17, [sp, #16 * 8]\n\t"
               "stp x18, x19, [sp, #16 * 9]\n\t"
               "stp x20, x21, [sp, #16 * 10]\n\t"
               "stp x22, x23, [sp, #16 * 11]\n\t"
               "stp x24, x25, [sp, #16 * 12]\n\t"
               "stp x26, x27, [sp, #16 * 13]\n\t"
               "stp x28, x29, [sp, #16 * 14]\n\t"
               "str x30, [sp, #16 * 15]\n\t");
}

__attribute__((always_inline)) inline void kernel::interrupt_exit() {
  asm volatile("ldp x0, x1, [sp, #16 * 0]\n\t"
               "ldp x2, x3, [sp, #16 * 1]\n\t"
               "ldp x4, x5, [sp, #16 * 2]\n\t"
               "ldp x6, x7, [sp, #16 * 3]\n\t"
               "ldp x8, x9, [sp, #16 * 4]\n\t"
               "ldp x10, x11, [sp, #16 * 5]\n\t"
               "ldp x12, x13, [sp, #16 * 6]\n\t"
               "ldp x14, x15, [sp, #16 * 7]\n\t"
               "ldp x16, x17, [sp, #16 * 8]\n\t"
               "ldp x18, x19, [sp, #16 * 9]\n\t"
               "ldp x20, x21, [sp, #16 * 10]\n\t"
               "ldp x22, x23, [sp, #16 * 11]\n\t"
               "ldp x24, x25, [sp, #16 * 12]\n\t"
               "ldp x26, x27, [sp, #16 * 13]\n\t"
               "ldp x28, x29, [sp, #16 * 14]\n\t"
               "ldr x30, [sp, #16 * 15]\n\t");
}

void kernel::default_panic_handler() {
  while (true)
    ;
}