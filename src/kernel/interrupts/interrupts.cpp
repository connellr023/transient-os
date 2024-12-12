#include "interrupts.hpp"
#include "../../drivers/framebuffer.hpp"
#include "../../drivers/interrupt_requests.hpp"
#include "../kernel.hpp"
#include "../threads/thread_control_block.hpp"
#include <stdint.h>

void *kernel::interrupts::isr(void *interrupted_sp) {
  using namespace threads;
  static bool is_first_isr = true;

  safe_put("Entering interrupt service routine\n");

  if (scheduler.get_size() == 0) {
    safe_put("No threads to switch to\n");

    // Halt kernel execution
    while (true) {
      asm volatile("wfe");
    }
  }

  // Avoid corrupting the first thread's context when switching from main
  if (!is_first_isr) {
    is_first_isr = false;

    // Save context of interrupted thread
    scheduler.peek()->set_sp(interrupted_sp);
    scheduler.peek()->set_state(ThreadState::Ready);
  }

  // Goto next thread
  scheduler.next();
  scheduler.peek()->set_state(ThreadState::Running);

  // Restore context of next thread and return its stack pointer
  return scheduler.peek()->get_sp();
}

uint32_t current_us = 0;

void kernel::interrupts::trigger_isr() {
  uint32_t current_us = *TIMER_COUNTER_LOW;
  *TIMER_CMP_1 = current_us + 1000;

  asm volatile("wfi");
}

void kernel::interrupts::post_isr() {
  current_us = *TIMER_COUNTER_LOW + scheduler.peek()->get_burst_time();

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