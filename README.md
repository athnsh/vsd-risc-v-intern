# RISC-V FPGA IP Design – VSD Internship

**Submitted by:** Atharv Priyansh  
**Date:** 2026-06-02  
**Program:** VSD FPGA Internship

[![Compiler](https://img.shields.io/badge/Compiler-RISC--V%20GCC-blue)](https://github.com/riscv-collab/riscv-gnu-toolchain)
[![Simulator](https://img.shields.io/badge/Simulator-Spike%20ISA-brightgreen)](https://github.com/riscv-software-src/riscv-isa-sim)
[![Language|0](https://img.shields.io/badge/Language-C%20%7C%20Assembly-red)](https://www.gnu.org/software/gcc/)

---

## Overview

This repository documents my work during the VSD FPGA Internship, progressing from basic RISC-V software execution to FPGA development workflows and custom hardware design.

The tasks cover the complete hardware-software flow, including RISC-V compilation, ISA-level debugging, FPGA environment setup, and designing memory-mapped peripheral IPs — starting with a GPIO output peripheral, extending it into a full multi-register bidirectional GPIO controller, and then adding a real timer peripheral integrated into the existing RISC-V SoC.

**Key topics covered:**

- RISC-V toolchain and compilation
- Spike ISA simulation and debugging
- FPGA environment setup
- Verilog RTL design
- Memory-mapped peripheral integration
- Address-offset decoding for multi-register IPs
- Timer peripheral design and validation
- Hardware-software co-design and simulation

---

## Tasks Overview

### [Task 1: RISC-V Compilation](Task1/README.md)
Understanding C-to-RISC-V compilation pipeline with different optimization levels (-O1 vs -Ofast). Demonstrates how the same C program generates different assembly depending on compiler flags.

### [Task 2: Spike Simulation & Debugging](task2/README.md)
Executing and debugging RISC-V binaries using the Spike ISA simulator. Includes two programs:
- **sum1ton.c**: Mathematical computation with optimization comparison
- **[LFSR.c](task2/LFSR.c)**: 32-bit pseudo-random generator (hardware-oriented algorithm)

### [Task 3: Environment Setup & RISC-V Reference Bring-Up](task3/README.md)
Building and validating the complete RISC-V software-to-FPGA workflow, validating software execution using Spike, and preparing the FPGA build flow.

### [Task 4: Design & Integrate a Memory-Mapped IP](task4/README.md)
Design a simple memory-mapped peripheral, integrate it into the existing RISC-V SoC's address decoder and bus, and validate the integration through simulation. Includes 3 programs.

### [Task 5: Extend the GPIO IP into a Full GPIO Control Peripheral](task5/README.md)
Extend the write-only, single-register GPIO IP from Task 4 into a three-register, bidirectional GPIO controller (`GPIO_DATA`, `GPIO_DIR`, `GPIO_READ`) using address-offset decoding behind one base address. Validates that the direction register correctly gates the readback path.

### [Task 6: Timer IP - Core Contributor Task](task6/README.md)
Design and integrate a memory-mapped timer peripheral with one-shot and periodic modes, an optional prescaler, a sticky timeout flag, and an LED-facing timeout output. Includes RTL, software validation, simulation evidence, and hardware flashing notes.

---

## Repository Structure

```
vsd-intern/
├── README.md (this file)
├── task1/
│   ├── README.md (Compilation guide)
│   └── task1-resources/
│       └── (screenshots and notes)
├── task2/
│   ├── README.md (Spike debugging guide)
│   ├── LFSR.c (pseudo-random generator)
│   └── task2-resources/
│       └── (screenshots and notes)
├── task3/
│   ├── README.md (Environment setup and FPGA bring-up guide)
│   └── task3-resources/
│       └── (Docker file, screenshots and notes)
├── task4/
│   ├── README.md (GPIO IP design and simulation validation guide)
│   └── task4-resources/
│       ├── gpio_ip.v (32-bit memory-mapped GPIO peripheral RTL)
│       ├── bench.v (Simulation testbench and FPGA primitive stubs)
│       ├── riscv.v (Modified RISC-V SoC with GPIO integration)
│       ├── gpio_test.c (Firmware used to validate the GPIO IP)
│       ├── io.h (Memory-mapped peripheral address definitions)
│       └── (Waveform screenshots, simulation logs and implementation notes)
├── task5/
│   ├── README.md (GPIO Control IP extension and simulation validation guide)
│   └── task5-resources/
│       ├── gpio_ip.v (Extended 3-register GPIO peripheral RTL — DATA/DIR/READ)
│       ├── riscv.v (Modified RISC-V SoC with offset-decoded GPIO integration)
│       ├── gpio_test.c (Firmware exercising all three GPIO registers)
│       ├── io.h (Updated peripheral address definitions for DATA/DIR/READ)
│       └── (Waveform screenshots, simulation logs and implementation notes)
├── task6/
│   ├── README.md (Timer IP core contributor write-up and validation guide)
│   ├── ip/
│   │   └── ap_timer_ip/
│   │       ├── README.md (Timer IP documentation)
│   │       ├── rtl/
│   │       │   ├── bench.v (Simulation testbench and FPGA primitive stubs)
│   │       │   ├── io.h (Memory-mapped timer address definitions)
│   │       │   ├── riscv.v (Modified RISC-V SoC with timer integration)
│   │       │   └── timer_ip.v (32-bit memory-mapped timer peripheral RTL)
│   │       └── test/
│   │           ├── led_test.c (Firmware used to validate the timer IP)
│   │           ├── LED_Test01.mp4 (Hardware demo video)
│   │           └── (Waveform screenshots, simulation logs and implementation notes)
│   └── task6-notes.md (Raw timer implementation notes and screenshots)
└── resources/ (Reference materials currently in gitignore)
```

---

## Quick Links

- [Task 1 README](Task1/README.md) – RISC-V compilation workflow and toolchain setup with screenshots
- [Task 2 README](task2/README.md) – Spike simulation, debugging workflow and LFSR analysis
- [Task 3 README](task3/README.md) – FPGA environment setup and board bring-up guide
- [Task 4 README](task4/README.md) – GPIO IP design, SoC integration and simulation validation workflow
- [Task 5 README](task5/README.md) – GPIO Control IP extension, address-offset decoding and direction-register validation
- [Task 6 README](task6/README.md) – Timer IP design, SoC integration, simulation and hardware validation

- [LFSR.c Source](task2/LFSR.c) – 32-bit pseudo-random generator implementation

- [gpio_ip.v (Task 4)](task4/task4-resources/gpio_ip.v) – 32-bit memory-mapped GPIO peripheral RTL
- [bench.v](task4/task4-resources/bench.v) – Simulation testbench and FPGA primitive stubs
- [gpio_test.c (Task 4)](task4/task4-resources/gpio_test.c) – Bare-metal firmware used to validate the GPIO IP
- [riscv.v (Task 4)](task4/task4-resources/riscv.v) – Modified RISC-V SoC with GPIO integration
- [io.h (Task 4)](task4/task4-resources/io.h) – Memory-mapped peripheral address definitions

- [gpio_ip.v (Task 5)](task5/task5-resources/gpio_ip.v) – Extended 3-register GPIO peripheral RTL (DATA/DIR/READ)
- [gpio_test.c (Task 5)](task5/task5-resources/gpio_test.c) – Firmware exercising all three GPIO registers
- [riscv.v (Task 5)](task5/task5-resources/riscv.v) – SoC with offset-decoded GPIO integration
- [io.h (Task 5)](task5/task5-resources/io.h) – Updated peripheral address definitions for DATA/DIR/READ

- [Timer IP README](task6/ip/ap_timer_ip/README.md) – Timer IP overview, register map and functional description
- [timer_ip.v](task6/ip/ap_timer_ip/rtl/timer_ip.v) – 32-bit memory-mapped timer peripheral RTL
- [led_test.c](task6/ip/ap_timer_ip/test/led_test.c) – Bare-metal firmware used to validate the timer IP
- [riscv.v](task6/ip/ap_timer_ip/rtl/riscv.v) – Modified RISC-V SoC with timer integration
- [io.h](task6/ip/ap_timer_ip/rtl/io.h) – Memory-mapped timer register definitions

- [Task 1 Notes](Task1/task1-resources/task1-notes.md) – Raw compilation notes and observations
- [Task 2 Notes](task2/task2-resources/task2-notes.md) – Spike debugging notes and observations
- [Task 3 Notes](task3/task3-resources/task3-notes.md) – FPGA setup notes and observations
- [Task 4 Notes](task4/task4-resources/task4-notes.md) – IP integration, simulation and debugging notes
- [Task 5 Notes](task5/task5-resources/task5-notes.md) – GPIO Control IP extension and validation notes
- [Task 6 Notes](task6/task6-notes.md) – Timer IP implementation notes and validation screenshots