# Timer IP

## Overview

The Timer IP is a 32-bit memory-mapped countdown timer designed for integration with the RISC-V SoC. It supports one-shot and periodic operating modes, an optional prescaler, and a timeout flag that can be cleared through software.

---

## Features

- 32-bit programmable countdown timer
- One-shot and periodic modes
- Optional programmable prescaler
- Memory-mapped register interface
- Write-1-to-clear timeout flag
- Read-only current timer value
- Timeout output signal for external hardware

---

## Register Map

| Offset | Register | Access | Description |
|--------|----------|--------|-------------|
| `0x00` | CTRL | R/W | Timer control register |
| `0x04` | LOAD | R/W | Initial countdown value |
| `0x08` | VALUE | R | Current countdown value |
| `0x0C` | STATUS | R/W | Timeout status register |

---

## Register Description

### CTRL (0x00)

| Bits | Name | Description |
|------|------|-------------|
| 0 | EN | Enable timer |
| 1 | MODE | `0` = One-shot, `1` = Periodic |
| 2 | PRESC_EN | Enable prescaler |
| 15:8 | PRESC_DIV | Prescaler divide value |
| Others | Reserved | Read as 0 |

---

### LOAD (0x04)

Stores the initial countdown value. Whenever the timer is started or reloaded in periodic mode, this value is copied into the VALUE register.

---

### VALUE (0x08)

Read-only register containing the current countdown value.

---

### STATUS (0x0C)

| Bit | Name | Description |
|-----|------|-------------|
| 0 | TIMEOUT | Set when countdown reaches zero. Cleared by writing `1`. |

---

## Functional Description

When the timer is enabled (`EN = 1`), the VALUE register is loaded from LOAD and begins counting down.

If the prescaler is enabled, the counter decrements once every `(PRESC_DIV + 1)` clock cycles. Otherwise, it decrements every clock cycle.

When the countdown reaches zero:

- The TIMEOUT flag is asserted.
- In **One-shot mode**, the timer stops at zero.
- In **Periodic mode**, the timer automatically reloads the LOAD value and continues counting.

The TIMEOUT flag remains asserted until software clears it by writing a `1` to the STATUS register.

The `timeout` output directly reflects the timeout flag and can be connected to external hardware or other SoC peripherals.

---

## Address Decoding

The SoC selects the Timer IP using its assigned memory region. The lower address bits (`offset`) select the internal register.

| Offset | Register |
|--------|----------|
| `2'b00` | CTRL |
| `2'b01` | LOAD |
| `2'b10` | VALUE |
| `2'b11` | STATUS |

---

## Software Example

```c
IO_OUT(IO_TIMER_LOAD, 1000);
IO_OUT(IO_TIMER_CTRL, 0x03);   // Enable + Periodic Mode

while(!(IO_IN(IO_TIMER_STATUS) & 0x1));

IO_OUT(IO_TIMER_STATUS, 0x1);  // Clear timeout flag
```

---

## Validation

The Timer IP was validated by:

- Programming the LOAD register
- Enabling the timer
- Reading the VALUE register
- Polling the TIMEOUT flag
- Clearing the TIMEOUT flag through STATUS
- Verifying both one-shot and periodic modes
- Integrating the timeout output with the SoC for hardware demonstration