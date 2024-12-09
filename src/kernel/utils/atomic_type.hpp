#ifndef ATOMIC_TYPE_HPP
#define ATOMIC_TYPE_HPP

#include "atomic_guard.hpp"

template <typename T> class Atomic {
public:
  Atomic() : value(0) {}
  Atomic(T value) : value(value) {}

  T get() {
    AtomicGuard guard;
    return value;
  }

  void set(T value) {
    AtomicGuard guard;
    this->value = value;
  }

  T operator++() {
    AtomicGuard guard;
    return ++value;
  }

  T operator++(int) {
    AtomicGuard guard;
    return value++;
  }

  T operator--() {
    AtomicGuard guard;
    return --value;
  }

  T operator--(int) {
    AtomicGuard guard;
    return value--;
  }

  T operator+=(T value) {
    AtomicGuard guard;
    return this->value += value;
  }

  T operator-=(T value) {
    AtomicGuard guard;
    return this->value -= value;
  }

  T operator=(T value) {
    AtomicGuard guard;
    return this->value = value;
  }
};

#endif // ATOMIC_TYPE_HPP