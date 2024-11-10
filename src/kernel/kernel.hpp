/**
 * https://gist.github.com/petemoore/4a17f701d2ad57025a1f6ab90a66ce33
 */

#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

namespace kernel {
inline void interrupt_entry();
inline void interrupt_exit();

void default_panic_handler();

void (*const panic_handler)(void) = default_panic_handler;
} // namespace kernel

#endif // INTERRUPT_HPP