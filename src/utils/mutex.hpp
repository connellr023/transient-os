#ifndef MUTEX_HPP
#define MUTEX_HPP

class Mutex {
private:
  volatile bool is_locked;

public:
  Mutex() : is_locked(false) {}

  void acquire();
  void release();
};

#endif // MUTEX_HPP