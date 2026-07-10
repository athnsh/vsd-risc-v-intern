# Example Usage

## Software Programming Model

A typical sequence for using the Timer IP is:

1. Write the desired countdown value to the **LOAD** register.
2. Configure the **CTRL** register and set the **EN** bit.
3. Poll the **STATUS** register until the **TIMEOUT** flag becomes `1`.
4. Clear the timeout flag by writing `1` to the **STATUS** register.
5. For periodic mode, continue polling and clearing the timeout flag while the timer automatically reloads.

---

# Example Program

The following example demonstrates both one-shot and periodic operation of the Timer IP.

```c
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
```

---

# Expected UART Output

```
=== Timer IP Validation ===
Starting One-Shot Timer...
One-Shot Timeout!
Starting Periodic Timer...
Periodic Timeout 1
Periodic Timeout 2
Periodic Timeout 3
Periodic Timeout 4
Periodic Timeout 5
Validation Complete
```

---

# Expected Hardware Behavior

During hardware validation, the `timeout` output of the Timer IP is connected to the SoC LED control logic.

- In **One-Shot Mode**, the timeout output is asserted once when the countdown reaches zero.
- In **Periodic Mode**, the timeout output is asserted after every countdown cycle as the timer automatically reloads.
- The reference implementation toggles **LED[4]** on every timeout event, providing a visual indication of timer operation.

---

# Validation Performed

The Timer IP was validated by:

- Writing the LOAD register.
- Configuring the CTRL register.
- Reading the VALUE register during countdown.
- Polling the STATUS register.
- Clearing the timeout flag using Write-1-to-Clear.
- Verifying one-shot mode.
- Verifying periodic auto-reload mode.
- Demonstrating timeout indication on the FPGA board.

---

# Common Issues

| Issue | Possible Cause |
|-------|----------------|
| Timer does not start | EN bit not set in CTRL |
| Timeout never occurs | LOAD value not written or timer not enabled |
| Timeout flag never clears | STATUS register not written with `1` |
| Timer reloads unexpectedly | MODE bit configured for periodic mode |
| Countdown speed incorrect | Incorrect LOAD value or prescaler configuration |