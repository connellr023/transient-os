#include "interrupts.hpp"
#include "../../drivers/framebuffer.hpp"
#include "../../drivers/interrupt_requests.hpp"
#include "../../drivers/timer.hpp"
#include "../../drivers/uart0.hpp"
#include "../kernel.hpp"
#include <stdint.h>

void kernel::interrupts::interrupt_service_routine(void *sp) {
  // uint64_t *stack = static_cast<uint64_t *>(sp);

  safe_string_output("ISR\n");
  // framebuffer::fill_screen(0x002BFF);

  // for (int i = 0; i < 15; i++) {
  //   uart0::puts("ISR\n");
  // }

  // // Save context of interrupted thread
  // thread_queue.peek().get_gp_registers().save(stack);
  // thread_queue.peek().get_sp_registers().save();

  // // Goto next thread
  // thread_queue.next();
  // uart0::puts("Next\n");

  // // Restore context of next thread
  // thread_queue.peek().get_gp_registers().restore(stack);
  // thread_queue.peek().get_sp_registers().restore();
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
  safe_string_output("Synchronous exception cause (EC): 0x");
  safe_hex_output(ec);
  safe_string_output("\n");

  safe_string_output("Instruction Specific Syndrome (ISS): 0x");
  safe_hex_output(iss);
  safe_string_output("\n");

  // Log ELR_EL1 (Faulting instruction address)
  safe_string_output("Exception Link Register (ELR_EL1): 0x");
  safe_hex_output(elr);
  safe_string_output("\n");

  // Log FAR_EL1 (Fault Address Register, if applicable)
  safe_string_output("Fault Address Register (FAR_EL1): 0x");
  safe_hex_output(far);
  safe_string_output("\n");

  // Halt execution for debugging
  safe_string_output("System halted due to synchronous exception.\n");

  while (true) {
    asm volatile("wfe");
  }
}

void kernel::interrupts::fiq_exception_handler() {
  safe_string_output("FIQ\n");
}

void kernel::interrupts::serror_exception_handler() {
  safe_string_output("SError\n");
}