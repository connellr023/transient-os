#include <kernel/memory/paging.hpp>
#include <kernel/memory/virtual_memory.hpp>
#include <stdint.h>

/**
 * @brief Value from linker script that defines the start of the data section
 */
extern volatile uint8_t _data;

/**
 * @brief Value from linker script that defines the end of all sections
 */
extern volatile uint8_t _end;

namespace kernel::memory {
void init_virtual_memory() {
  uint64_t data_page = reinterpret_cast<uint64_t>(&_data) / PAGE_SIZE;
  uint64_t *translation_table =
      reinterpret_cast<uint64_t *>(reinterpret_cast<uint64_t>(&_end));

  // Set up 1:1 translation table for all memory
}
} // namespace kernel::memory