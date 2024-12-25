#ifndef MUTEX_LOCK_GUARD_HPP
#define MUTEX_LOCK_GUARD_HPP

#include <api/concurrency/mutex.hpp>

/**
 * RAII class for locking and unlocking a mutex.
 */
class MutexLockGuard {
private:
  Mutex *mutex;

public:
  MutexLockGuard(Mutex *mutex) : mutex(mutex) { mutex->lock(); }

  ~MutexLockGuard() { mutex->unlock(); }
};

#endif // MUTEX_LOCK_GUARD_HPP