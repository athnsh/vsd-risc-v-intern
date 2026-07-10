#include <stdio.h>
#include "io.h"

int main(void)
{
    printf("=== Timer IP Validation ===\n");

    /* ---------- One-Shot Mode ---------- */

    printf("Starting One-Shot Timer...\n");

    IO_OUT(IO_TIMER_LOAD, 10);
    IO_OUT(IO_TIMER_CTRL, 0x01);   // EN = 1

    while(!(IO_IN(IO_TIMER_STATUS) & 0x1));

    printf("One-Shot Timeout!\n");

    IO_OUT(IO_TIMER_STATUS, 0x1);

    /* ---------- Periodic Mode ---------- */

    printf("Starting Periodic Timer...\n");

    IO_OUT(IO_TIMER_LOAD, 10);
    IO_OUT(IO_TIMER_CTRL, 0x03);   // EN = 1, MODE = 1

    for(int i = 0; i < 5; i++)
    {
        while(!(IO_IN(IO_TIMER_STATUS) & 0x1));

        printf("Periodic Timeout %d\n", i + 1);

        IO_OUT(IO_TIMER_STATUS, 0x1);
    }

    IO_OUT(IO_TIMER_CTRL, 0x00);

    printf("Validation Complete\n");

    return 0;
}