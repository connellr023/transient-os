#include "mutex.hpp"

using namespace kernel::utils;

void Mutex::acquire() {
  while (true) {
    uint32_t lock_status;

    // Use LDAXR to load the lock status
    asm volatile("ldaxr %w0, [%1]"       // Load-Acquire Exclusive
                 : "=&r"(lock_status)    // Output: lock_status
                 : "r"(&this->is_locked) // Input: address of is_locked
                 : "memory");            // Memory barrier to prevent reordering

    // If not locked, attempt to acquire
    if (lock_status == 0) {
      uint32_t result;

      asm volatile("stlxr %w0, %w1, [%2]"  // Store-Release Exclusive
                   : "=&r"(result)         // Output: result of STLXR
                   : "r"(1),               // Input: value to store (1 = locked)
                     "r"(&this->is_locked) // Input: address of is_locked
                   : "memory");            // Memory barrier

      if (result == 0) {
        // Successfully acquired the lock
        break;
      }
    }

    // Optional: Small delay or yield to avoid aggressive spinning
  }
}

void Mutex::release() {
  // Store 0 to unlock
  asm volatile("stlr %w0, [%1]"        // Store-Release
               :                       // No output
               : "r"(0),               // Input: value to store (0 = unlocked)
                 "r"(&this->is_locked) // Input: address of is_locked
               : "memory");            // Memory barrier
}