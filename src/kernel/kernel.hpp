/**
 * https://gist.github.com/petemoore/4a17f701d2ad57025a1f6ab90a66ce33
 */

#ifndef INTERRUPT_HPP
#define INTERRUPT_HPP

namespace kernel {
constexpr int register_stack_frame_size = 16 * 16;

void init();

void default_panic_handler();
void (*const panic_handler)(void) = default_panic_handler;

namespace irq {
enum class EntryError {
  SyncInvalidEL1t = 0,
  IRQInvalidEL1t = 1,
  FIQInvalidEL1t = 2,
  ErrorInvalidEL1t = 3,
  SyncInvalidEL1h = 4,
  IRQInvalidEL1h = 5,
  FIQInvalidEL1h = 6,
  ErrorInvalidEL1h = 7,
  SyncInvalidEL0_64 = 8,
  IRQInvalidEL0_64 = 9,
  FIQInvalidEL0_64 = 10,
  ErrorInvalidEL0_64 = 11,
  SyncInvalidEL0_32 = 12,
  IRQInvalidEL0_32 = 13,
  FIQInvalidEL0_32 = 14,
  ErrorInvalidEL0_32 = 15
};

void handle_invalid_entry(EntryError error);

__attribute__((always_inline)) inline void entry();
__attribute__((always_inline)) inline void exit();

void handler();
} // namespace irq
} // namespace kernel

#endif // INTERRUPT_HPP