#ifndef INTERNAL_INTERRUPTS_HPP
#define INTERNAL_INTERRUPTS_HPP

#include "../sys/sys_calls.hpp"

#define SVC_EC 0x15

namespace kernel::interrupts {
/**
 * ### (INTERNAL) IRQ Exception Handler
 * @brief Handles an interrupt request exception.
 * @param interrupted_sp The stack pointer at the time of the interrupt.
 * @return The new stack pointer.
 */
void *internal_irq_exception_handler(void *interrupted_sp) asm(
    "_internal_irq_exception_handler");

/**
 * ### (INTERNAL) Synchronous Exception Handler
 * @brief Handles a synchronous exception.
 * @param call_code The system call code. (SVC only)
 * @param arg The argument to the system call. (SVC only)
 * @return Stack pointer of the next thread.
 */
void *internal_synch_exception_handler(
    SystemCall call_code, void *arg,
    void *interrupted_sp) asm("_internal_synch_exception_handler");
} // namespace kernel::interrupts

#endif // INTERNAL_INTERRUPTS_HPP