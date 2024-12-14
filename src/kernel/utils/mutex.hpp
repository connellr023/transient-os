#ifndef MUTEX_HPP
#define MUTEX_HPP

namespace kernel::utils {
class Mutex {
private:
  volatile bool is_locked;

public:
  Mutex() : is_locked(false) {}

  void acquire();
  void release();
};

} // namespace kernel::utils

#endif // MUTEX_HPP