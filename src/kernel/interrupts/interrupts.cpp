#include "interrupts.hpp"
#include "../../drivers/framebuffer.hpp"
#include "../../drivers/interrupt_requests.hpp"
#include "../../drivers/timer.hpp"
#include "../../drivers/uart0.hpp"
#include "../kernel.hpp"
#include <stdint.h>

uint64_t kernel::interrupts::isr(uint64_t sp) {
  safe_put("ISR\n");

  // Save context of interrupted thread
  scheduler.peek().save_ctx(sp);

  safe_put("Interrupted thread SP: 0x");
  safe_hex(scheduler.peek().get_sp());
  safe_put("\n");

  safe_put("Interrupted thread PC: 0x");
  safe_hex(scheduler.peek().get_pc());
  safe_put("\n");

  // Goto next thread
  scheduler.next();
  safe_put("Next thread\n");

  safe_put("Next thread SP: 0x");
  safe_hex(scheduler.peek().get_sp());
  safe_put("\n");

  safe_put("Next thread PC: 0x");
  safe_hex(scheduler.peek().get_pc());
  safe_put("\n");

  // Restore context of next thread and return its stack pointer
  return scheduler.peek().restore_ctx();
}

uint32_t current_us = 0;

void kernel::interrupts::init_timer() {
  current_us = *TIMER_COUNTER_LOW;
  current_us += timer_interval_us;

  // Generate an interrupt when the timer reaches the current value
  *TIMER_CMP_1 = current_us;
}

void kernel::interrupts::post_isr() {
  current_us = *TIMER_COUNTER_LOW + timer_interval_us;

  // Generate an interrupt when the timer reaches the current value
  *TIMER_CMP_1 = current_us;

  // Clear the interrupt
  *TIMER_CS = TIMER_CS_M1;
}

void kernel::interrupts::enable_interrupt_controller() {
  // Enable the system timer IRQ
  *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}

void kernel::interrupts::enable_interrupts() {
  asm volatile("msr daifclr, #2");
}

void kernel::interrupts::disable_interrupts() {
  asm volatile("msr daifset, #2");
}

void kernel::interrupts::init_interrupt_vector() {
  asm volatile("adrp x2, _vectors \n\t\
                add x2, x2, :lo12:_vectors \n\t\
                msr vbar_el1, x2");
}

void kernel::interrupts::synch_exception_handler() {
  disable_interrupts();

  uint64_t esr, elr, far;

  // Read the ESR_EL1 (Exception Syndrome Register)
  asm volatile("mrs %0, esr_el1" : "=r"(esr));
  const uint64_t ec = esr >> 26;       // Exception Class (EC)
  const uint64_t iss = esr & 0xFFFFFF; // Instruction Specific Syndrome (ISS)

  // Read the ELR_EL1 (Exception Link Register)
  asm volatile("mrs %0, elr_el1" : "=r"(elr));

  // Read the FAR_EL1 (Fault Address Register)
  asm volatile("mrs %0, far_el1" : "=r"(far));

  // Log ESR_EL1 (Exception Class and ISS)
  safe_put("Synchronous exception cause (EC): 0x");
  safe_hex(ec);
  safe_put("\n");

  safe_put("Instruction Specific Syndrome (ISS): 0x");
  safe_hex(iss);
  safe_put("\n");

  // Log ELR_EL1 (Faulting instruction address)
  safe_put("Exception Link Register (ELR_EL1): 0x");
  safe_hex(elr);
  safe_put("\n");

  // Log FAR_EL1 (Fault Address Register, if applicable)
  safe_put("Fault Address Register (FAR_EL1): 0x");
  safe_hex(far);
  safe_put("\n");

  // Halt execution for debugging
  safe_put("System halted due to synchronous exception.\n");

  while (true) {
    asm volatile("wfe");
  }
}

void kernel::interrupts::fiq_exception_handler() {
  disable_interrupts();
  safe_put("FIQ\n");

  while (true) {
    asm volatile("wfe");
  }
}

void kernel::interrupts::serror_exception_handler() {
  disable_interrupts();
  safe_put("SError\n");

  while (true) {
    asm volatile("wfe");
  }
}

uint64_t _kernel_isr(uint64_t sp) { return kernel::interrupts::isr(sp); }

void _kernel_post_isr() { kernel::interrupts::post_isr(); }

void _kernel_synch_exception_handler() {
  kernel::interrupts::synch_exception_handler();
}

void _kernel_fiq_exception_handler() {
  kernel::interrupts::fiq_exception_handler();
}

void _kernel_serror_exception_handler() {
  kernel::interrupts::serror_exception_handler();
}