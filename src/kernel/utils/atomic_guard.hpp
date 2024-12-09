#ifndef ATOMIC_GUARD_HPP
#define ATOMIC_GUARD_HPP

#include "../interrupts/interrupts.hpp"

namespace kernel::utils {
// RAII class to perform operations atomically
class AtomicGuard {
public:
  AtomicGuard() { interrupts::disable_interrupts(); }

  ~AtomicGuard() { interrupts::enable_interrupts(); }
};
} // namespace kernel::utils

#endif // ATOMIC_GUARD_HPP