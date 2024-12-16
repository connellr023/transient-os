#include "interrupts.hpp"
#include "../../drivers/framebuffer.hpp"
#include "../../drivers/interrupt_requests.hpp"
#include "../kernel.hpp"
#include "../threads/thread_control_block.hpp"
#include <stdint.h>

void kernel::interrupts::prepare_timer_interrupt(uint64_t interval) {
  static uint32_t current_us = 0;

  current_us = *TIMER_COUNTER_LOW;
  *TIMER_CMP_1 = current_us + interval;
}

void kernel::interrupts::yield() {
  prepare_timer_interrupt(10);
  asm volatile("wfi");
}

void kernel::interrupts::clear_timer_interrupt() {
  // Clear the M1 bit in the control/status register
  *TIMER_CS = TIMER_CS_M1;
}

void kernel::interrupts::enable_interrupt_controller() {
  // Enable the system timer IRQ
  *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}

void *kernel::interrupts::irq_exception_handler(void *interrupted_sp) {
  const ThreadControlBlock *next_tcb = context_switch(interrupted_sp);

  // Prepare timer for next context switch
  prepare_timer_interrupt(next_tcb->get_burst_time());
  clear_timer_interrupt();

  // Return the stack pointer of the next thread
  return next_tcb->get_sp();
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