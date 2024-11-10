#include "utility.hpp"

int8_t utility::get_exception_level() {
  int8_t el;

  asm volatile("mrs %x0, CurrentEL" : "=r"(el));
  return el >> 2;
}