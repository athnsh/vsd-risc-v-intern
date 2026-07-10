# Timer IP User Guide

## 1. Introduction

The Timer IP is a programmable 32-bit memory-mapped countdown timer designed for integration with the VSDSquadron RISC-V SoC. It provides software-controlled timing by generating a timeout event after a configurable countdown period.

The IP supports both one-shot and periodic operation, making it suitable for delays, periodic tasks, timeout detection, and other embedded timing applications.

---

## 2. Features

- 32-bit programmable countdown timer
- Memory-mapped register interface
- One-shot and periodic operating modes
- Optional 8-bit programmable prescaler
- Write-1-to-clear timeout flag
- Read-only countdown value register
- Dedicated hardware timeout output
- Synchronous design with active-low reset

---

## 3. Applications

Typical applications include:

- Software delay generation
- LED blinking
- Periodic task scheduling
- Timeout detection
- General-purpose timing
- Simple embedded control applications

---

## 4. Functional Overview

The Timer IP operates by loading a user-defined value into an internal countdown register.

When the timer is enabled:

1. The VALUE register is loaded from the LOAD register.
2. VALUE decrements every clock cycle.
3. If the prescaler is enabled, the timer decrements once every `(PRESC_DIV + 1)` clock cycles.
4. When VALUE reaches zero:
   - The TIMEOUT flag is asserted.
   - In **One-shot Mode**, the counter stops at zero.
   - In **Periodic Mode**, the LOAD value is automatically reloaded and counting continues.

The timeout flag remains asserted until software clears it by writing a `1` to the STATUS register.

---

## 5. Internal Architecture

```
                CPU Bus
                   │
           Address Decoder
                   │
          Register Interface
                   │
      ┌────────────┴────────────┐
      │                         │
  CTRL / LOAD              Prescaler
      │                         │
      └────────────┬────────────┘
                   │
          Countdown Counter
                   │
             Timeout Logic
                   │
      STATUS Register / timeout
```

---

## 6. Operating Modes

### One-shot Mode

The timer counts down once and stops when the countdown reaches zero. The TIMEOUT flag remains asserted until cleared by software.

### Periodic Mode

The timer automatically reloads the LOAD value after reaching zero and continues counting, allowing continuous periodic operation.

---

## 7. Prescaler

The Timer IP includes an optional programmable prescaler.

When enabled, the countdown occurs once every:

```
PRESC_DIV + 1
```

clock cycles instead of every clock cycle. This allows longer timing intervals without increasing the LOAD value.

---

## 8. Software Programming Model

A typical software sequence is:

1. Write the desired countdown value to **LOAD**.
2. Configure the **CTRL** register.
3. Set the **EN** bit to start the timer.
4. Poll the **STATUS** register until the TIMEOUT flag is asserted.
5. Clear the TIMEOUT flag by writing `1` to the STATUS register.

---

## 9. Hardware Demonstration

For hardware validation on the VSDSquadron FPGA board, the Timer IP timeout output was connected to the SoC LED control logic. Each timeout event toggles **LED[4]**, providing a visual indication that the timer is operating correctly.

---

## 10. Validation

The Timer IP was validated through both simulation and hardware testing by:

- Configuring timer registers
- Reading the VALUE register during countdown
- Verifying timeout generation
- Testing one-shot and periodic modes
- Clearing the timeout flag through software
- Demonstrating timeout indication using the onboard LED

---

## 11. Limitations

- Polling-based operation (no interrupt support)
- Single timer instance
- Single timeout output
- 8-bit programmable prescaler
- Assumes a synchronous system clock
- Internal prescaler counter is not software-readable