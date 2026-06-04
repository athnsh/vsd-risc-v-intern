# RISC-V FPGA IP Design – VSD Internship

**Submitted by:** Atharv Priyansh  
**Date:** 2026-06-02  
**Program:** VSD FPGA Internship

[![Compiler](https://img.shields.io/badge/Compiler-RISC--V%20GCC-blue)](https://github.com/riscv-collab/riscv-gnu-toolchain)
[![Simulator](https://img.shields.io/badge/Simulator-Spike%20ISA-brightgreen)](https://github.com/riscv-software-src/riscv-isa-sim)
[![Language|0](https://img.shields.io/badge/Language-C%20%7C%20Assembly-red)](https://www.gnu.org/software/gcc/)

---

## Overview

This internship project focuses on **RISC-V processor design and verification** using a systematic multi-stage validation flow (O0→O1→O2→O3→O4). The goal is to understand how high-level C programs are compiled, optimized, and executed on RISC-V architecture, and how this foundation supports subsequent hardware design stages.

### Design & Verification Flow

- **O0**: x86 GCC compilation (reference baseline)
- **O1**: RISC-V C model execution and ISA verification
- **O2**: RTL design verification (Verilog)
- **O3**: Physical design (GDSII)
- **O4**: Silicon tape-out and final verification

---

## Tasks Overview

### [Task 1: RISC-V Compilation](vsd-intern/task1/README.md)
Understanding C-to-RISC-V compilation pipeline with different optimization levels (-O1 vs -Ofast). Demonstrates how the same C program generates different assembly depending on compiler flags.

**Key Skills:** Cross-compilation, Disassembly, Compiler Optimization, ISA Analysis

### [Task 2: Spike Simulation & Debugging](vsd-intern/task2/README.md)
Executing and debugging RISC-V binaries using the Spike ISA simulator. Includes two programs:
- **sum1ton.c**: Mathematical computation with optimization comparison
- **[LFSR.c](vsd-intern/task2/LFSR.c)**: 32-bit pseudo-random generator (hardware-oriented algorithm)

**Key Skills:** RISC-V Simulation, Register Inspection, Instruction Encoding, Functional Verification, Hardware Algorithm Implementation

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
└── resources/ (Reference materials currently in gitignore)
```

---

## Quick Links

- [Task 1 README](vsd-intern/task1/README.md) – Full compilation guide with screenshots
- [Task 2 README](vsd-intern/task2/README.md) – Spike debugging workflow and LFSR analysis
- [LFSR.c Source](vsd-intern/task2/LFSR.c) – 32-bit pseudo-random generator implementation
- [Task 1 Notes](vsd-intern/task1/task1-resources/task1-notes.md) [Task 2 Notes](vsd-intern/task2/task2-resources/task2-notes.md) – Raw debugging notes and observations