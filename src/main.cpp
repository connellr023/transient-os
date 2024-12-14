#include "drivers/framebuffer.hpp"
#include "drivers/uart0.hpp"
#include "kernel/interrupts/interrupts.hpp"
#include "kernel/kernel.hpp"
#include "kernel/threads/thread_control_block.hpp"
#include "kernel/utils/mutex.hpp"
#include <stdint.h>

using namespace kernel::utils;
using namespace kernel::threads;

struct SharedTestStruct {
public:
  uint64_t a;
  uint64_t b;
  uint64_t c;
};

Mutex uart_mutex = Mutex();

constexpr uint64_t uart_buffer_size = 100;
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
  while (true) {
    produce_string("thread 1 - ");

    for (int i = 0; i < 100000; i++)
      asm volatile("nop");
  }
}

void test_task_2(void *arg) {
  while (true) {
    produce_string("thread 2 - ");

    for (int i = 0; i < 10000; i++)
      asm volatile("nop");
  }
}

int main() {
  uart0::init();
  kernel::init_dbg_out(&uart0::puts, &uart0::hex);

  uart0::puts("Hello, world!\n");

  ThreadControlBlock tcb_0(&uart_consumer_task, 1000);
  ThreadControlBlock tcb_1(&test_task_1, 1800);
  ThreadControlBlock tcb_2(&test_task_2, 1200);

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