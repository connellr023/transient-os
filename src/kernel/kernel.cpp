#include "kernel.hpp"
#include "../drivers/uart0.hpp"
#include <stdint.h>

void kernel::init() { uart0::init(); }

void kernel::irq::handle_invalid_entry(EntryError error) {
  uint64_t exception_syndrome_reg;
  asm volatile("mrs %0, esr_el1" : "=r"(exception_syndrome_reg));

  uint64_t exception_link_reg;
  asm volatile("mrs %0, elr_el1" : "=r"(exception_link_reg));

  uart0::puts("Error type: ");
  uart0::hex(static_cast<int>(error));
  uart0::puts("\n");

  uart0::puts("Exception syndrome register: ");
  uart0::hex(exception_syndrome_reg);
  uart0::puts("\n");

  uart0::puts("Exception link register: ");
  uart0::hex(exception_link_reg);
  uart0::puts("\n");

  kernel::panic_handler();
}

void kernel::irq::handle_interrupt() {
  interrupt_entry();

  uart0::puts("IRQ\n");

  interrupt_exit();
}

void kernel::irq::init_interrupt_vector() {
  asm volatile("adr x0, vectors\n\t"
               "msr vbar_el1, x0");
}

void kernel::irq::enable_interrupts() { asm volatile("msr daifclr, #2"); }

void kernel::irq::disable_interrupts() { asm volatile("msr daifset, #2"); }

void kernel::irq::interrupt_entry() {
  asm volatile("sub sp, sp, %0\n\t"
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
               "str x30, [sp, #16 * 15]\n\t" ::"i"(register_stack_frame_size));
}

void kernel::irq::interrupt_exit() {
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
               "ldr x30, [sp, #16 * 15]\n\t"
               "add sp, sp, %0\n\t" ::"i"(register_stack_frame_size));
}

void kernel::default_panic_handler() {
  while (true)
    ;
}