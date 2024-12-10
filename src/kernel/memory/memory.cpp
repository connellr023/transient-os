#include "memory.hpp"

using namespace kernel::memory;

uint16_t memory_map[page_count] = {0};

uint64_t kernel::memory::palloc() {
  for (uint64_t i = 0; i < page_count; i++) {
    if (memory_map[i] == 0) {
      memory_map[i] = 1;

      return low_memory + (i * page_size);
    }
  }

  return 0;
}

void kernel::memory::pfree(uint64_t page) {
  memory_map[(page - low_memory) / page_size] = 0;
}