# Integration Guide

## Overview

This guide explains how to integrate the Timer IP into a VSDSquadron RISC-V SoC. The Timer IP is implemented as a memory-mapped peripheral and communicates with the processor through the existing I/O bus.

---

# Required Files

The following RTL file is required for hardware integration:

```
timer_ip.v
```

The following software files are provided for reference and validation:

```
io.h
timer_test.c
```

---

# Memory Map

The Timer IP exposes four 32-bit memory-mapped registers. In the reference SoC implementation, the peripheral is mapped to the base address **0x400040**.

| Register | Offset | Address |
|----------|--------|----------|
| CTRL | 0x00 | 0x400040 |
| LOAD | 0x04 | 0x400044 |
| VALUE | 0x08 | 0x400048 |
| STATUS | 0x0C | 0x40004C |

---

# Include the IP

Include the Timer IP module in the SoC source.

```verilog
`include "timer_ip.v"
```

---

# Address Decoding

Reserve an I/O bit for the Timer peripheral.

```verilog
localparam IO_TIMER_bit = 4;
```

Generate the Timer select, write enable, and register offset signals.

```verilog
wire        timer_sel;
wire        timer_write;
wire [1:0]  timer_offset;

assign timer_sel    = isIO & mem_wordaddr[IO_TIMER_bit];
assign timer_write  = timer_sel & mem_wstrb;
assign timer_offset = mem_addr[3:2];
```

The lower address bits (`mem_addr[3:2]`) are forwarded to the Timer IP as the `offset` signal. The Timer IP internally decodes this 2-bit offset to select one of its four registers.

---

# Instantiating the Timer IP

Instantiate the Timer IP alongside the other memory-mapped peripherals in the SoC.

```verilog
wire [31:0] timer_rdata;
wire        timer_timeout;

timer_ip TIMER(
    .clk(clk),
    .resetn(resetn),

    .sel(timer_sel),
    .write_en(timer_write),
    .offset(timer_offset),

    .write_data(mem_wdata),
    .read_data(timer_rdata),
    .timeout(timer_timeout)
);
```

---

# Connecting the Read Bus

Connect the Timer read data to the SoC I/O read multiplexer.

```verilog
wire [31:0] IO_rdata =
        mem_wordaddr[IO_UART_CNTL_bit] ? {22'b0,!uart_ready,9'b0} :
        mem_wordaddr[IO_GPIO_bit]      ? gpio_rdata :
        mem_wordaddr[IO_TIMER_bit]     ? timer_rdata :
                                         32'b0;
```

This allows software to access the Timer registers through standard memory-mapped reads.

---

# Timeout Output

The Timer IP provides a dedicated hardware output:

```verilog
output timeout;
```

The `timeout` output directly reflects the internal `timeout_flag`. It becomes asserted when the countdown reaches zero and remains asserted until software clears the STATUS register by writing a `1` to bit 0.

The signal can be connected to:

- LEDs
- GPIO
- Interrupt controller
- Custom hardware logic

In the reference implementation, the timeout signal is connected to hardware logic that toggles **LED[4]** whenever a timeout event occurs.

```verilog
reg timer_timeout_d;
reg led_timer_toggle;

always @(posedge clk) begin
    timer_timeout_d <= timer_timeout;

    if(timer_timeout && !timer_timeout_d)
        led_timer_toggle <= ~led_timer_toggle;
end

always @(posedge clk)
    LEDS[4] <= led_timer_toggle;
```

---

# Software Integration

Add the Timer register definitions to `io.h`.

```c
#define IO_TIMER_CTRL   64
#define IO_TIMER_LOAD   68
#define IO_TIMER_VALUE  72
#define IO_TIMER_STATUS 76
```

Software can access the Timer using the existing memory-mapped I/O macros.

```c
IO_OUT(IO_TIMER_LOAD, 1000);
IO_OUT(IO_TIMER_CTRL, 0x01);   // Enable timer

while(!(IO_IN(IO_TIMER_STATUS) & 0x1));

IO_OUT(IO_TIMER_STATUS, 0x1);  // Clear timeout flag
```

---

# Integration Checklist

- [ ] Add `timer_ip.v` to the RTL project
- [ ] Reserve an I/O address for the Timer peripheral
- [ ] Generate `timer_sel`, `timer_write`, and `timer_offset`
- [ ] Instantiate the Timer IP
- [ ] Connect `timer_rdata` to the SoC read-data multiplexer
- [ ] Route the `timeout` output to the required hardware
- [ ] Add the Timer register definitions to `io.h`
- [ ] Validate functionality using the supplied software example

---

# Notes

- The Timer IP uses a synchronous system clock.
- Reset is active-low and synchronous.
- Register accesses are 32-bit and word-aligned.
- Undefined register offsets return `0x00000000`.
- The timeout flag is cleared by writing a `1` to bit 0 of the STATUS register.
- The Timer IP can be integrated into any memory-mapped SoC that provides compatible select, write-enable, address-offset, and data signals.