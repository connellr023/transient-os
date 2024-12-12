#include "memory.hpp"

using namespace kernel::memory;

uint16_t memory_map[PAGE_COUNT] = {0};

uint64_t kernel::memory::palloc() {
  for (uint64_t i = 0; i < PAGE_COUNT; i++) {
    if (memory_map[i] == 0) {
      memory_map[i] = 1;

      return LOW_MEMORY + (i * PAGE_SIZE);
    }
  }

  return 0;
}

void kernel::memory::pfree(uint64_t page) {
  memory_map[(page - LOW_MEMORY) / PAGE_SIZE] = 0;
}