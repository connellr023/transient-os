#ifndef IRQ_HPP
#define IRQ_HPP

#define REG_STACK_FRAME_SIZE 16 * 16

namespace kernel::interrupts {
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

void enable_interrupts();
void disable_interrupts();

extern "C" {
void init_interrupt_vector();
void handle_invalid_entry(EntryError error);
void interrupt_service_routine();
}
} // namespace kernel::interrupts

#endif // IRQ_HPP