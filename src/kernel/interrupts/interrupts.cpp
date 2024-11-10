#include "interrupts.hpp"
#include "../../drivers/uart0.hpp"
#include "../kernel.hpp"

void kernel::interrupts::handle_invalid_entry(EntryError error) {
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

void kernel::interrupts::interrupt_service_routine() { uart0::puts("IRQ\n"); }

void kernel::interrupts::enable_interrupts() {
  asm volatile("msr daifclr, #2");
}

void kernel::interrupts::disable_interrupts() {
  asm volatile("msr daifset, #2");
}