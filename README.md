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

The tasks cover the complete hardware-software flow, including RISC-V compilation, ISA-level debugging, FPGA environment setup, and designing a memory-mapped GPIO IP integrated into an existing RISC-V SoC.

**Key topics covered:**

- RISC-V toolchain and compilation
- Spike ISA simulation and debugging
- FPGA environment setup
- Verilog RTL design
- Memory-mapped peripheral integration
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
Design a simple memory-mapped peripheral, integrate it into the existing RISC-V SoC's address decoder and bus, and validate the integration through simulation. Includes 3 programs

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
└── resources/ (Reference materials currently in gitignore)
```

---

## Quick Links

- [Task 1 README](Task1/README.md) – RISC-V compilation workflow and toolchain setup with screenshots
- [Task 2 README](task2/README.md) – Spike simulation, debugging workflow and LFSR analysis
- [Task 3 README](task3/README.md) – FPGA environment setup and board bring-up guide
- [Task 4 README](task4/README.md) – GPIO IP design, SoC integration and simulation validation workflow

- [LFSR.c Source](task2/LFSR.c) – 32-bit pseudo-random generator implementation

- [gpio_ip.v](task4/task4-resources/gpio_ip.v) – 32-bit memory-mapped GPIO peripheral RTL
- [bench.v](task4/task4-resources/bench.v) – Simulation testbench and FPGA primitive stubs
- [gpio_test.c](task4/task4-resources/gpio_test.c) – Bare-metal firmware used to validate the GPIO IP
- [riscv.v](task4/task4-resources/riscv.v) – Modified RISC-V SoC with GPIO integration
- [io.h](task4/task4-resources/io.h) – Memory-mapped peripheral address definitions

- [Task 1 Notes](Task1/task1-resources/task1-notes.md) – Raw compilation notes and observations
- [Task 2 Notes](task2/task2-resources/task2-notes.md) – Spike debugging notes and observations
- [Task 3 Notes](task3/task3-resources/task3-notes.md) – FPGA setup notes and observations
- [Task 4 Notes](task4/task4-resources/task4-notes.md) – IP integration, simulation and debugging notes
