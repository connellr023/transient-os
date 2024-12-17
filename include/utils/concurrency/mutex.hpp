#ifndef MUTEX_HPP
#define MUTEX_HPP

class Mutex {
private:
  volatile bool is_locked;

public:
  Mutex() : is_locked(false) {}

  ~Mutex() {
    if (this->is_locked) {
      this->unlock();
    }
  }

  void lock();
  void unlock();
};

#endif // MUTEX_HPP