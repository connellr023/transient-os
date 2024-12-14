#include "mutex.hpp"
#include "../interrupts/interrupts.hpp"

using namespace kernel::utils;
using namespace kernel::interrupts;

void Mutex::acquire() {
  disable_interrupts();

  while (this->is_locked) {
    enable_interrupts();
    asm volatile("nop");
    disable_interrupts();
  }

  this->is_locked = true;
  enable_interrupts();
}

void Mutex::release() {
  disable_interrupts();
  this->is_locked = false;
  enable_interrupts();
}