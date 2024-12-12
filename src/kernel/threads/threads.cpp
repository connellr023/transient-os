#include "threads.hpp"
#include "../interrupts/interrupts.hpp"
#include "../kernel.hpp"
#include "../memory/memory.hpp"

void kernel::threads::thread_return_handler() {
  interrupts::disable_interrupts();

  const ThreadControlBlock *current_tcb = kernel::scheduler.peek();

  safe_put("Thread ID: ");
  safe_hex(current_tcb->get_thread_id());
  safe_put("completed!\n");

  // Free thread page
  memory::pfree(current_tcb->get_page());

  // Remove thread from scheduler
  kernel::scheduler.dequeue();

  // Trigger context switch
  interrupts::enable_interrupts();
  interrupts::trigger_isr();
}