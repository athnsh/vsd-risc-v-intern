# TASK 6

## Objective

### Timer IP - Core Contributor Task

## Overview

This task implements a real memory-mapped **Timer peripheral IP** and integrates it into the existing RISC-V SoC used in the project. The timer supports a programmable countdown, a sticky timeout flag, one-shot and periodic modes, and an optional prescaler. It was validated through software, simulation, and FPGA hardware.

- **Owner:** Timer IP
- **SoC integration base address:** `TIMER_BASE = 0x2000_1000`
- **Bus interface:** memory-mapped, 32-bit registers, word-aligned accesses

---

## What the IP Does

The timer counts down from a software-programmed load value and exposes the current count and status through a simple register interface.

When the counter reaches zero:

- the `TIMEOUT` status bit is set
- in **one-shot mode**, the timer stops at zero
- in **periodic mode**, the timer automatically reloads from `LOAD` and continues counting

Software polls the `STATUS.TIMEOUT` bit and clears it using a **write-1-to-clear** operation. For the hardware demo, the timer timeout output was also connected to an LED toggle path so that every timeout event produces a visible heartbeat.

**Implemented RTL:** `ip/ap_timer_ip/rtl/timer_ip.v`

```verilog
module timer_ip (
	input            clk,
	input            resetn,
	input            sel,
	input            wr_en,
	input            rd_en,
	input      [1:0] addr,
	input      [31:0] wdata,
	output reg [31:0] rdata,
	output           timeout_o
);
```

---

## Register Map

Base address: `TIMER_BASE = 0x2000_1000`

| Offset | Name   | R/W | Description |
|--------|--------|-----|-------------|
| 0x00   | CTRL   | R/W | Control bits |
| 0x04   | LOAD   | R/W | Countdown start value |
| 0x08   | VALUE  | R   | Current countdown value |
| 0x0C   | STATUS | R/W | Timeout status / clear |

![Timer register map and register interface plan](ip/ap_timer_ip/test/Pasted%20image%2020260704185043.png)

### CTRL (0x00)

| Bits | Field | Description |
|------|-------|-------------|
| 0 | EN | 1 = enable counting, 0 = stop |
| 1 | MODE | 0 = one-shot, 1 = periodic / auto-reload |
| 2 | PRESC_EN | 0 = no prescale, 1 = prescaler enabled |
| 15:8 | PRESC_DIV | Prescaler divide value; effective divisor = `PRESC_DIV + 1` |
| others | - | Reserved, read as 0 |

### LOAD (0x04)

This register stores the initial countdown value. The timer loads this value when it is enabled and reloads from it in periodic mode.

### VALUE (0x08)

This register shows the current countdown value. It is read-only.

### STATUS (0x0C)

| Bit | Field | Description |
|-----|-------|-------------|
| 0 | TIMEOUT | Set to 1 when the countdown reaches 0. Cleared by writing 1. |

---

## Functional Behavior

- While `EN = 1`, the timer decrements `VALUE` once per tick.
- If the prescaler is disabled, the decrement occurs every clock cycle.
- If the prescaler is enabled, the decrement occurs every `PRESC_DIV + 1` cycles.
- When `VALUE` reaches zero, `TIMEOUT` is asserted.
- In one-shot mode, the timer holds at zero.
- In periodic mode, the timer reloads from `LOAD` and continues running.
- On the `EN` rising edge, `VALUE` is loaded from `LOAD` before the first decrement so that the timer does not immediately timeout after reset.

The timeout output used for the board demo is driven directly from the sticky status flag.

---

## SoC Integration

The timer was integrated into the SoC by decoding the assigned address window and routing the CPU bus signals to the peripheral.

- Address decoding uses the high address region for the timer window.
- The lower address bits select the internal register.
- Reads from undefined offsets return 0 and writes are ignored.
- The timer timeout output is routed into the SoC LED demo path.

The implementation follows the same memory-mapped peripheral pattern used for the other tasks in the project.

### Address Offset Decoding

The timer uses a 4-register window with offset-based decoding:

| Offset | Register |
|--------|----------|
| `2'b00` | CTRL |
| `2'b01` | LOAD |
| `2'b10` | VALUE |
| `2'b11` | STATUS |

---

## RTL Implementation

The timer RTL was implemented in a clean synchronous style with separate control, prescaler, and countdown blocks.

### Internal State

```verilog
reg        en, mode, presc_en;
reg [7:0]  presc_div;
reg [31:0] load_reg;
reg [31:0] value_reg;
reg        timeout_flag;
reg        en_prev;
reg [7:0]  presc_cnt;
```

### Key Logic Blocks

- **CTRL/LOAD write path:** updates the timer configuration and load value.
- **Prescaler block:** generates a tick based on the configured divider.
- **Countdown core:** handles load-on-enable, countdown, sticky timeout, and periodic reload.
- **Read mux:** returns CTRL, LOAD, VALUE, or STATUS based on the selected offset.

The timer uses a sticky `TIMEOUT` flag so software can poll it reliably, even if the timeout event occurs faster than the software polling loop.

![Timer register interface implementation](ip/ap_timer_ip/test/Pasted%20image%2020260704190522.png)

![Timer registers fully implemented](ip/ap_timer_ip/test/Pasted%20image%2020260704195947.png)

![Timer integrated into the SoC](ip/ap_timer_ip/test/Pasted%20image%2020260704200008.png)

---

## Software Control

The test software is located at `ip/ap_timer_ip/test/led_test.c` and exercises the timer through UART-visible logging.

### Control Flow Used by the Test

1. Program `LOAD`
2. Enable the timer in one-shot mode
3. Poll `STATUS.TIMEOUT`
4. Clear the timeout flag using write-1-to-clear
5. Repeat the test in periodic mode
6. Print progress through UART for visibility in simulation and hardware

### Example Usage

```c
#define TIMER_BASE   0x20001000
#define TIMER_CTRL   (*(volatile unsigned int *)(TIMER_BASE + 0x00))
#define TIMER_LOAD   (*(volatile unsigned int *)(TIMER_BASE + 0x04))
#define TIMER_VALUE  (*(volatile unsigned int *)(TIMER_BASE + 0x08))
#define TIMER_STAT   (*(volatile unsigned int *)(TIMER_BASE + 0x0C))

#define CTRL_EN        (1 << 0)
#define CTRL_MODE      (1 << 1)

TIMER_LOAD = TICK_COUNT;
TIMER_CTRL = CTRL_EN;
while ((TIMER_STAT & 1) == 0)
	;
TIMER_STAT = 1;
```

### Software Files

- `ip/ap_timer_ip/test/led_test.c` - timer validation program
- `ip/ap_timer_ip/rtl/io.h` - memory-mapped register definitions

![Updated io.h with timer register macros](ip/ap_timer_ip/test/Pasted%20image%2020260704200129.png)

![Timer validation test program](ip/ap_timer_ip/test/Pasted%20image%2020260704230331.png)

![Timer test program UART output and second run](ip/ap_timer_ip/test/Pasted%20image%2020260704230457.png)

---

## Validation

### I. Simulation

The timer IP was validated in simulation using the RISC-V test program and a Verilog bench.

Validation covered:

- programming `LOAD`
- enabling the timer
- polling `STATUS.TIMEOUT`
- clearing the sticky status bit
- one-shot timeout behavior
- periodic auto-reload behavior

The simulation setup used:

- `ip/ap_timer_ip/rtl/bench.v`
- `ip/ap_timer_ip/rtl/riscv.v`
- `ip/ap_timer_ip/rtl/timer_ip.v`
- `ip/ap_timer_ip/test/led_test.c`

### II. Hardware

The design was synthesized and flashed to the VSDSquadron FPGA board for board-level validation.

To make the timeout event visible on hardware, an extra timer output was added in the RTL:

```verilog
output timeout
assign timeout = timeout_flag;
```

That timeout output was then used to toggle an LED on every timeout event.

Hardware validation was completed with the standard flow:

```bash
make clean
make
sudo make flash
```

This confirmed that the timer behaved correctly on real hardware as well as in simulation.

![Timer hardware output and flash run](ip/ap_timer_ip/test/Pasted%20image%2020260706002947.png)

---

## Directory Structure

```text
task6/
├── ip/
│   └── ap_timer_ip/
│       ├── README.md
│       ├── rtl/
│       │   ├── bench.v
│       │   ├── io.h
│       │   ├── riscv.v
│       │   └── timer_ip.v
│       └── test/
│           └── led_test.c
├── task6-notes.md
└── README.md
```

---

## Design Decisions

- **Sticky timeout flag:** `TIMEOUT` is held until software clears it so the event is easy to poll and easy to observe on hardware.
- **Load-on-enable:** `VALUE` is loaded from `LOAD` when `EN` rises, which avoids an immediate false timeout after reset.
- **Periodic auto-reload:** periodic mode reloads from `LOAD` automatically, allowing repeated timeouts without software intervention.
- **Separate prescaler counter:** the prescaler is implemented as its own counter so the countdown logic stays simple.
- **Word-aligned register map:** the timer exposes a small 4-register map using `mem_addr[3:2]`, matching the common integration rules in the task spec.

---

## Summary

By completing this task, I:

- designed and implemented a memory-mapped timer IP
- integrated it into the existing RISC-V SoC
- added one-shot and periodic countdown behavior
- implemented a sticky timeout flag with write-1-to-clear semantics
- validated the design in simulation and on FPGA hardware
- added an LED-facing timeout output for visible board demonstration

The final result is a usable peripheral IP that follows the task specification and fits the same integration style used across the other SoC peripherals.
