# AP Timer IP

A lightweight 32-bit memory-mapped countdown timer designed for integration with the **VSDSquadron RISC-V SoC**. The Timer IP supports programmable countdowns, one-shot and periodic operation, an optional prescaler, and a software-clearable timeout flag.

---

## Features

- 32-bit programmable countdown timer
- Memory-mapped register interface
- One-shot and periodic operating modes
- Optional 8-bit programmable prescaler
- Write-1-to-clear timeout flag
- Read-only countdown value register
- Dedicated `timeout` output for hardware integration

---

## Register Summary

| Offset | Register | Access | Description |
|:------:|:--------:|:------:|-------------|
| `0x00` | CTRL | R/W | Timer configuration |
| `0x04` | LOAD | R/W | Countdown start value |
| `0x08` | VALUE | R | Current countdown value |
| `0x0C` | STATUS | R/W | Timeout status (Write-1-to-Clear) |

For complete register definitions, see **[docs/Register_Map.md](docs/Register_Map.md)**.

---

## Quick Integration

Integrating the Timer IP requires only a few steps:

1. Add `rtl/timer_ip.v` to your RTL project.
2. Instantiate the Timer IP in the SoC.
3. Connect the Timer registers to the memory-mapped I/O bus.
4. Add the Timer register definitions to `io.h`.

Complete integration instructions are available in **[docs/Integration_Guide.md](docs/Integration_Guide.md)**.

---

## Quick Software Example

```c
IO_OUT(IO_TIMER_LOAD, 1000);
IO_OUT(IO_TIMER_CTRL, 0x01);      // Enable timer

while(!(IO_IN(IO_TIMER_STATUS) & 0x1));

IO_OUT(IO_TIMER_STATUS, 0x1);     // Clear timeout flag
```

---

## Testing

A reference software application is provided in **`software/timer_test.c`**.

The test demonstrates:

- One-shot mode
- Periodic mode
- Countdown operation
- Timeout generation
- STATUS flag clearing
- Hardware LED demonstration

Complete software examples and expected behavior can be found in **[docs/Example_Usage.md](docs/Example_Usage.md)**.

---

## Documentation

| Document | Description |
|----------|-------------|
| **[IP_User_Guide.md](docs/IP_User_Guide.md)** | Overview, architecture, operating modes, applications, and limitations |
| **[Register_Map.md](docs/Register_Map.md)** | Register definitions, bit fields, reset values, and register behavior |
| **[Integration_Guide.md](docs/Integration_Guide.md)** | SoC integration, address decoding, instantiation, and software interface |
| **[Example_Usage.md](docs/Example_Usage.md)** | Programming examples, expected output, and hardware validation |

---

## Validation

The Timer IP was verified through both simulation and FPGA hardware testing.

Validation included:

- Register read/write verification
- Countdown functionality
- One-shot mode
- Periodic mode
- Prescaler operation
- Timeout generation
- Write-1-to-clear functionality
- Hardware LED demonstration

---

## Project Structure

```text
ap_timer_ip/
├── README.md
├── docs/
│   ├── Example_Usage.md
│   ├── IP_User_Guide.md
│   ├── Integration_Guide.md
│   └── Register_Map.md
├── rtl/
│   ├── riscv.v
|   ├── io.h
|   ├── bench.v
│   └── timer_ip.v
├── software/
│   ├── led_test.c
│   └── timer_test.c
└── validation/
    ├── LED_Test01.mp4
    ├── waveform_1.png
    ├── waveform_2.png
    ├── waveform_3.png
    └── ...
```

---

## Limitations

- Polling-based operation (no interrupt support)
- Single timer instance
- Single timeout output
- 8-bit programmable prescaler
- Assumes a synchronous system clock