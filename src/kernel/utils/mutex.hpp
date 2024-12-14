#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <stdint.h>

namespace kernel::utils {
class Mutex {
private:
  alignas(8) volatile uint32_t is_locked;

public:
  Mutex() : is_locked(false) {}

  bool get_is_locked() const { return static_cast<bool>(is_locked); }

  void acquire();
  void release();
};
} // namespace kernel::utils

#endif // MUTEX_HPP