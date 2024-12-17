#include "../../../include/utils/concurrency/mutex.hpp"
#include "../../../include/kernel/interrupts/interrupts.hpp"

using namespace kernel::interrupts;

void Mutex::lock() {
  disable_interrupts();

  while (this->is_locked) {
    enable_interrupts();
    asm volatile("wfi");
    disable_interrupts();
  }

  this->is_locked = true;
  enable_interrupts();
}

void Mutex::unlock() {
  disable_interrupts();
  this->is_locked = false;
  enable_interrupts();
}