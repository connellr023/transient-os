#include "drivers/uart0.hpp"

int main()
{
    uart0::init();

    while (1)
    {
        uart0::send('a');

        for (int i = 0; i < 10000; i++)
            asm volatile("nop");

        uart0::send('v');

        for (int i = 0; i < 10000; i++)
            asm volatile("nop");
    }

    return 0;
}