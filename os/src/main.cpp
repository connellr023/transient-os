#include <api/include/sys/sys_calls.hpp>
#include <rpi3-drivers/include/uart0.hpp>

int main() {
  uart0::init();
  api::sys::set_output_handler(&uart0::puts);

  for (int i = 0; i < 10; i++) {
    uart0::puts("Hello, World!\n");
  }

  return 0;
}