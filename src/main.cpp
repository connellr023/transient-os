#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/interrupts/interrupts.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include <stdint.h>

using namespace kernel::threads;

struct SharedTestStruct {
public:
  uint64_t a;
  uint64_t b;
  uint64_t c;
};

constexpr uint64_t uart_buffer_size = 1000;
char uart_buffer[uart_buffer_size] = {0};
uint64_t uart_read_index = 0;
uint64_t uart_write_index = 0;

void produce_char(char c) {
  // kernel::interrupts::disable_interrupts();
  //  uart_mutex.acquire();

  uart_buffer[uart_write_index] = c;
  uart_write_index = (uart_write_index + 1) % uart_buffer_size;

  // uart_mutex.release();
  // kernel::interrupts::enable_interrupts();
}

void produce_string(const char *s) {
  kernel::interrupts::disable_interrupts();

  while (*s) {
    produce_char(*s);
    s++;
  }

  kernel::interrupts::enable_interrupts();
}

void produce_hex(uint64_t value) {
  kernel::interrupts::disable_interrupts();

  for (int i = 0; i < 16; i++) {
    uint64_t nibble = (value >> (60 - i * 4)) & 0xF;

    if (nibble < 10)
      produce_char('0' + nibble);
    else
      produce_char('A' + nibble - 10);
  }

  produce_char('\n');

  kernel::interrupts::enable_interrupts();
}

void uart_consumer_task(void *arg) {
  while (true) {
    kernel::interrupts::disable_interrupts();
    //  uart_mutex.acquire();

    if (uart_buffer[uart_read_index] != 0) {
      uart0::send(uart_buffer[uart_read_index]);
      uart_buffer[uart_read_index] = 0;
    }

    uart_read_index = (uart_read_index + 1) % uart_buffer_size;

    // uart_mutex.release();
    kernel::interrupts::enable_interrupts();
  }
}

void test_task_1(void *arg) {
  uint64_t shared = reinterpret_cast<uint64_t>(arg);

  while (true) {
    produce_hex(0x69);
    asm volatile("wfi");
  }
}

void test_task_2(void *arg) {
  // uint64_t shared = *(reinterpret_cast<uint64_t *>(arg));

  while (true) {
    // Get sp
    uint64_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));

    produce_hex(sp);

    for (int i = 0; i < 1000; i++)
      asm volatile("nop");

    asm volatile("wfi");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  uint64_t test_shared = 0x69;

  ThreadControlBlock tcb_0(&uart_consumer_task, 1000);
  ThreadControlBlock tcb_1(&test_task_1, 1800, &test_shared);
  ThreadControlBlock tcb_2(&test_task_2, 1200, &test_shared);

  kernel::spawn_thread(&tcb_0);
  kernel::spawn_thread(&tcb_1);
  kernel::spawn_thread(&tcb_2);

  uart0::puts("Starting kernel\n");
  kernel::start();

  while (true) {
    uart0::puts("Waiting for kernel to start\n");
    asm volatile("wfi");
  }

  return 0;
}