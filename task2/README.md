# RISC-V Program Execution and Debugging with Spike

<summary><b>Task 2:</b> Running and Debugging RISC-V Programs with Spike Simulator</summary>

This task demonstrates how to execute RISC-V compiled programs using the Spike simulator and debug them using the Spike debugger. The objective is to understand instruction execution, register state changes, and instruction encoding at different optimization levels.

---

## Overview: Spike RISC-V Simulator

**Spike** is a functional RISC-V ISA simulator that allows you to:
- Execute RISC-V binaries without physical hardware
- Inspect register and memory state during execution
- Debug programs step-by-step using breakpoints and program counter (PC) manipulation
- Analyze how compiler optimizations affect instruction generation

This task builds on **Task 1** by taking the compiled RISC-V ELF binary (`sum1ton.o`) and executing it through the Spike simulator.

---

## Program: Sum from 1 to N (Continuation from Task 1)

We continue with the same `sum1ton.c` program compiled with different optimization levels. The compilation produces:
- `sum1ton.o` with `-O1` optimization: 15 instructions in main
- `sum1ton.o` with `-Ofast` optimization: 12 instructions in main

The key difference: **-Ofast produces more aggressive optimizations**, which we can observe and debug.

---

## Step 1: Run the RISC-V Program with Spike

Execute the compiled RISC-V binary using the Spike simulator:

```bash
spike pk sum1ton.o
```

### Command Explanation

- **`spike`**: RISC-V functional ISA simulator
- **`pk`**: Proxy kernel - a minimal OS environment that provides basic system call support for bare-metal RISC-V binaries
- **`sum1ton.o`**: The RISC-V ELF binary compiled in Task 1

**Result:** The program executes and displays the output. For `n=100`, the expected output is:
```
5050
```

This confirms that your RISC-V program computes the sum correctly and produces the same result as the x86 GCC compilation (O0 = O1 verification).

![Spike execution of sum1ton program](task2-resources/Pasted%20image%2020260604181157.png)

---

## Step 2: Open the Spike Debugger

To debug the program and inspect individual instruction execution, launch Spike with debugging enabled:

```bash
spike -d pk sum1ton.o
```

### Command Explanation

- **`-d`**: Debug mode flag that opens the interactive Spike debugger
- This allows step-by-step execution and inspection of registers and memory

**Result:** The Spike debugger prompt appears, ready for interactive commands.

![Spike debugger opened](task2-resources/Pasted%20image%2020260604182417.png)

---

## Step 3: Navigate to the Main Function

The `main` function starts at a specific address in the program counter (PC). To jump directly to main and begin inspection:

```
until pc 0 100b0
```

### Command Explanation

- **`until pc 0 100b0`**: Continues execution until the PC reaches address `0x100b0` (where main is located)
- This skips the startup code and takes you directly to the main function

**Result:** The debugger halts at the entry to the main function, ready for instruction-by-instruction debugging.

![Breakpoint set at main function](task2-resources/Pasted%20image%2020260604182843.png)

---

## Step 4: Inspect Register State - LUI Instruction

At main, the first instruction is `lui a2, 0x1`, which loads the upper immediate value into register `a2`.

Check the before state:

```
reg 0 a2
```

### Understanding LUI (Load Upper Immediate)

- **LUI** loads a 20-bit immediate value into bits [31:12] of the destination register
- **Instruction Encoding:**
  - Bits [0:6]: Opcode
  - Bits [7:11]: Destination register (rd)
  - Bits [12:31]: 20-bit immediate value
  - Bits [32:63]: Sign-extended zeros (for 64-bit)

**Before LUI execution:**

![Register a2 before LUI](task2-resources/Pasted%20image%2020260604182946.png)

```
a2 = 0x0000000000000000
```

Press `Enter` to execute the next instruction.

**After LUI execution:**

![Register a2 after LUI](task2-resources/Pasted%20image%2020260604183050.png)

```
a2 = 0x0000000000001000
```

The value `0x1` was shifted left 12 bits to become `0x1000`.

---

## Step 5: Inspect Multiple Register Changes - ADDI and AUI

Continue through the next instructions. The next instructions load values into registers:

```
lui a0, 0x21    → a0 becomes 0x0000000000021000
```

![Register a0 after loading upper immediate](task2-resources/Pasted%20image%2020260604183121.png)

Then the stack pointer (sp) is adjusted for the function prologue:

```
addi sp, sp, -16
```

**Before ADDI:**
- `sp = 0x000000007f7e9b50` (stack top)

**After ADDI:**
- `sp = 0x000000007f7e9b40` (sp decreased by 16, which is 0x10 in hex)

![Stack pointer adjustment](task2-resources/Pasted%20image%2020260604183211.png)

### ADDI (Add Immediate) Explanation

- **ADDI** adds a signed 12-bit immediate to a register
- Format: `addi rd, rs1, imm`
- Here: `sp` register receives `sp + (-16)`
- `-16` in binary (12-bit two's complement) = `0b111111110000` = `-0x10`

---

## Step 6: Instruction Encoding Details

Each RISC-V instruction is encoded as a 32-bit value. The encoding varies by instruction type (I-type, R-type, S-type, etc.).

**Example: LUI Instruction Analysis**

For `lui a2, 0x1`:
- Opcode (bits 0-6): `0110111` (binary) = LUI opcode
- Destination register a2 (bits 7-11): `01010` (binary) = 10 (a2 is register 12, but encoded differently)
- Immediate value (bits 12-31): `0x1` shifted into position

![Detailed instruction format](task2-resources/Pasted%20image%2020260604183552.png)

---

## Step 7: Optimization Comparison - -O1 vs -Ofast

### -O1 Optimization

At `-O1`, the compiler uses `lui` and `addi` to load immediate values into registers.

```
lui a2, 0x1
addi a2, a2, 0x400  (for example)
```

This is a two-instruction sequence to load a 32-bit immediate.

![Instructions with -O1 optimization](task2-resources/Pasted%20image%2020260604183858.png)

### -Ofast Optimization  

At `-Ofast`, the compiler uses a single `li` (load immediate) pseudo-instruction, which is expanded by the assembler into the minimal instruction sequence.

```
li a2, 0x1400  →  lui a2, 0x1
                   addi a2, a2, 0x400
```

The **-Ofast** optimization produces more compact code by eliminating redundant instructions and using more aggressive register allocation.

![Instructions with -Ofast optimization](task2-resources/Pasted%20image%2020260604184243.png)

The difference is visible when comparing the final instruction counts:
- **-O1**: ~15 instructions in main
- **-Ofast**: ~12 instructions in main

This demonstrates the practical impact of compiler optimization levels on generated code size and execution speed.

---

## Step 8: Continue Debugging

To continue step-by-step debugging, use these Spike debugger commands:

| Command | Purpose |
|---------|---------|
| `Enter` (blank line) | Execute the next instruction |
| `reg 0 <reg_name>` | Display register value |
| `mem 0 <address>` | Display memory content |
| `until pc 0 <address>` | Continue until PC reaches address |
| `step` | Step one instruction (alternative) |
| `quit` | Exit the debugger |

By repeating `Enter` and checking register states with `reg 0`, you can trace the entire program execution and understand how each instruction modifies the processor state.

![Continuing execution in debugger](task2-resources/Pasted%20image%2020260604185125.png)

---

## Summary: O1 Functional Equivalence Verification

This task completes the **O1 verification** in the design flow:

| Stage  | Task                           | Output                    | Verification                    |
| ------ | ------------------------------ | ------------------------- | ------------------------------- |
| **O0** | Task 1: GCC Compilation        | x86 reference output      | Baseline (n=1 to 5 → output=15) |
| **O1** | Task 2: RISC-V Spike Execution | Spike simulator output    | **O0 = O1** (output=5050)       |
| **O2** | Task 3: RTL Verification       | Verilog simulation output | O1 = O2 verification            |
| **O3** | Task 4: Physical Design        | GDSII verification        | O2 = O3 verification            |
| **O4** | Task 5: Silicon                | Tape-out results          | O3 = O4 verification            |

**Key Achievement:** By running the RISC-V binary through Spike and observing the correct output, we confirm that:
1. The RISC-V compilation is correct (C code → RISC-V ISA)
2. The Spike simulator correctly implements the RISC-V ISA
3. O0 = O1 (functional equivalence established)

This foundation ensures that any subsequent hardware implementation (RTL, physical design) only needs to verify against the RISC-V ISA specification, not re-verify the entire software compilation process.

---

# LFSR Program - 32-bit Pseudo-Random Generator

My program ([LFSR.c](LFSR.c)) implements a Linear Feedback Shift Register for pseudo-random sequence generation with:
- **Seed:** `0x00007D61` (initial state - must never be 0)
- **Polynomial:** `0xB4BCD35C` (feedback taps)
- **Algorithm:** Extract LSB → Shift right → XOR with polynomial if LSB=1 → Repeat

Compile with RISC-V GCC:

```bash
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o LFSR.o LFSR.c
```

### Running LFSR with Spike

Execute the LFSR program:

```bash
spike pk LFSR.o
```

**Output:**
```
Step  Output
----  ----------
1     0x00007D61
2     0xB4BCEDEC
3     0x5A5E76F6
...
16    0xED605A44
```

The LFSR generates a deterministic pseudo-random sequence. Output with **RISC-V GCC** is **identical** to native GCC, confirming correct compilation and execution.

![LFSR execution with Spike](task2-resources/Pasted%20image%2020260604191746.png)

### LFSR Instruction Count Analysis

**-Ofast Compilation:**

```bash
riscv64-unknown-elf-objdump -d LFSR.o | less
```

Result: **29 instructions in main function**

![Objdump for LFSR -Ofast](task2-resources/Pasted%20image%2020260604192203.png)

**-O1 Compilation:**

```bash
riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o LFSR.o LFSR.c
riscv64-unknown-elf-objdump -d LFSR.o | less
```

Result: **31 instructions in main function**

![Objdump for LFSR -O1](task2-resources/Pasted%20image%2020260604192606.png)

**Comparison Summary:**
- **-Ofast**: 29 instructions (more aggressive optimization)
- **-O1**: 31 instructions (fewer optimizations)
- **Reduction**: 2 instructions (~6.5% code size reduction with -Ofast)

The maximum possible sequence length is **2^32 - 1** states before the sequence repeats (XOR-based feedback ensures this period).