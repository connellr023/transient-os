#include "interrupt.hpp"

inline void kernel_entry() __attribute__((always_inline))
{
    asm volatile("sub sp, sp, #16 * 16");
    asm volatile("stp x0, x1, [sp, #16 * 0]");
    asm volatile("stp x2, x3, [sp, #16 * 1]");
    asm volatile("stp x4, x5, [sp, #16 * 2]");
    asm volatile("stp x6, x7, [sp, #16 * 3]");
    asm volatile("stp x8, x9, [sp, #16 * 4]");
    asm volatile("stp x10, x11, [sp, #16 * 5]");
    asm volatile("stp x12, x13, [sp, #16 * 6]");
    asm volatile("stp x14, x15, [sp, #16 * 7]");
    asm volatile("stp x16, x17, [sp, #16 * 8]");
    asm volatile("stp x18, x19, [sp, #16 * 9]");
    asm volatile("stp x20, x21, [sp, #16 * 10]");
    asm volatile("stp x22, x23, [sp, #16 * 11]");
    asm volatile("stp x24, x25, [sp, #16 * 12]");
    asm volatile("stp x26, x27, [sp, #16 * 13]");
    asm volatile("stp x28, x29, [sp, #16 * 14]");
    asm volatile("str x30, [sp, #16 * 15]");
}

inline void kernel_exit() __attribute__((always_inline))
{
    asm volatile("ldp x0, x1, [sp, #16 * 0]");
    asm volatile("ldp x2, x3, [sp, #16 * 1]");
    asm volatile("ldp x4, x5, [sp, #16 * 2]");
    asm volatile("ldp x6, x7, [sp, #16 * 3]");
    asm volatile("ldp x8, x9, [sp, #16 * 4]");
    asm volatile("ldp x10, x11, [sp, #16 * 5]");
    asm volatile("ldp x12, x13, [sp, #16 * 6]");
    asm volatile("ldp x14, x15, [sp, #16 * 7]");
    asm volatile("ldp x16, x17, [sp, #16 * 8]");
    asm volatile("ldp x18, x19, [sp, #16 * 9]");
    asm volatile("ldp x20, x21, [sp, #16 * 10]");
    asm volatile("ldp x22, x23, [sp, #16 * 11]");
    asm volatile("ldp x24, x25, [sp, #16 * 12]");
    asm volatile("ldp x26, x27, [sp, #16 * 13]");
    asm volatile("ldp x28, x29, [sp, #16 * 14]");
    asm volatile("ldr x30, [sp, #16 * 15]");
    asm volatile("add sp, sp, #16 * 16");
}