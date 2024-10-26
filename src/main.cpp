#include "drivers/uart.hpp"

int main()
{
    uart_init();

    while (1)
    {
        uart_send('a');

        for (int i = 0; i < 10000; i++)
        {
            asm volatile("nop");
        }

        uart_send('b');

        for (int i = 0; i < 10000; i++)
        {
            asm volatile("nop");
        }
    }

    return 0;
}