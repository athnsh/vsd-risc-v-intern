#include "io.h"

void delay(void)
{
    volatile unsigned int i;
    for(i = 0; i < 1000000; i++);
}

int main(void)
{
    while(1)
    {
        // All LEDs ON
        IO_OUT(IO_LEDS, 0x1F);
        delay();

        // All LEDs OFF
        IO_OUT(IO_LEDS, 0x00);
        delay();
    }

    return 0;
}