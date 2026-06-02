# Compilation of C Program using GCC and RISC-V GCC Compiler

<summary><b>Task 1:</b> Compilation of C Program using GCC and RISC-V GCC Compiler</summary>
This task shows how to compile a simple C program using the RISC-V GCC cross-compiler. The objective is to understand the compilation flow and observe the generated RISC-V assembly code. We will also compare the assembly output between different optimization levels (`-O1` and `-Ofast`).

---

## Context: VLSI SoC Design & Planning

This task is part of a broader **Digital VLSI SoC (System-on-Chip) design and verification flow**. The verification process involves multiple abstraction levels, each producing an output (O0-O4) that must be validated for functional equivalence:

### The Design & Verification Flow

| Stage | Description | Output | Purpose |
|-------|-------------|--------|---------|
| **O0** | **GCC Compilation** - Test application compiled with x86 GCC compiler | Reference output | Functional verification baseline on x86 architecture |
| **O1** | **C Model (RISC-V)** - C language model of the RISC-V processor architecture | C model output | Verify RISC-V ISA implementation matches O0 |
| **O2** | **RTL Design (Verilog/Bluespec/Chisel)** | RTL simulation output | Verify hardware description matches C model (O1 = O2) |
| **O3** | **Physical Design (GDSII)** - Layout with metal layers, CMOS, polysilicon | Post-silicon verification | Verify physical implementation (LVS/DRC checks for electrical correctness) |
| **O4** | **PCB & Tape-out** | Silicon output | Final verification: O1 = O2 = O3 = O4 |

**Key Principle:** At each stage, the output must be functionally equivalent to the previous stage to ensure correct processor design: **O0 = O1 = O2 = O3 = O4**

This task focuses on producing **O0** (x86 GCC output) and **O1** (RISC-V C model output), which form the foundation for all subsequent hardware verification stages.

---

## Program: Sum from 1 to N

We use a simple C program that calculates the sum of numbers from 1 to n. This straightforward program is ideal for learning how C code translates to RISC-V assembly.

### Quick Reference: Commands and Terminology

| Term/Command                    | Meaning                                 | Purpose                                                                                       |
| ------------------------------- | --------------------------------------- | --------------------------------------------------------------------------------------------- |
| **riscv64-unknown-elf-gcc**     | RISC-V 64-bit GCC Cross-Compiler        | Compiles C code into RISC-V machine code for bare-metal environments (no OS)                  |
| **-O1**                         | Basic Optimization Level                | Enables simple optimizations: faster code with minimal compile-time overhead                  |
| **-Ofast**                      | Aggressive Optimization Level           | Enables maximum speed optimizations; may ignore strict standards; produces fewer instructions |
| **-mabi=lp64**                  | ABI (Application Binary Interface) Flag | Specifies 64-bit RISC-V ABI with 64-bit longs and pointers                                    |
| **-march=rv64i**                | Architecture Flag                       | Targets the RV64I base integer instruction set (no floating-point extensions)                 |
| **-o**                          | Output File Flag                        | Specifies the output file name for compilation                                                |
| **riscv64-unknown-elf-objdump** | RISC-V Disassembler Utility             | Converts compiled binaries into human-readable assembly language                              |
| **-d**                          | Disassemble Flag                        | Disassembles all executable sections of a binary file                                         |
| **ELF**                         | Executable and Linkable Format          | Standard binary file format for executables and object files                                  |
| **gedit**                       | Text Editor                             | GUI text editor for viewing and editing source code files                                     |
| **less**                        | Paging Utility                          | Terminal utility for scrolling and searching through long text output                         |
| **RV64I**                       | RISC-V 64-bit Base Integer ISA          | Base instruction set for 64-bit RISC-V architecture (I = Integer only)                        |
| **ISA**                         | Instruction Set Architecture            | Set of machine instructions a processor can execute                                           |
| **Cross-Compilation**           | Compilation Process                     | Compiling code on one architecture (x86) to run on a different architecture (RISC-V)          |
| **Disassembly**                 | Reverse Engineering Process             | Converting machine code back into human-readable assembly language                            |

---

## Step 1: Navigate to the Sample Programs Directory

First navigate to the directory containing the sample C programs:

```bash
cd /workspaces/vsd-riscv2/samples
```

This ensures all commands below assume `sum1ton.c` is in the current working directory.

**Initial mistake - wrong directory:**

![Wrong directory initially](resources/Screenshot%202026-06-02%20131029.png)

**Fixed - correct sample programs directory:**

![Moved to correct directory](resources/Screenshot%202026-06-02%20131133.png)

---

## Step 2: Open and Inspect the Source File

Open the file using a text editor. In this case, `gedit` is used since `leafedit` isnt installed:

```bash
gedit sum1ton.c
```

This opens the text editor where you can view and verify the C program structure. Confirm the loop that computes the sum from 1 to n.

![Opened sum1ton.c in editor](resources/Screenshot%202026-06-02%20131934.png)

---

## Step 3: Edit the Program (Set n = 5)

Edit the source file to set `n = 5` so the output is easy to verify. This makes testing and understanding the output straightforward.

Example modification in the code:

```c
int n = 5;
```

After making the changes, save and close the file.

![Edited n to 5 and verified](resources/Screenshot%202026-06-02%20132449.png)

**This produces the O0 output** - the functional reference output from the x86 GCC compiler. This is our baseline for verifying that the RISC-V implementation produces the same result.

---

## Step 4: Compile for RISC-V (With -O1 Optimization)

Use the RISC-V cross-compiler to build an ELF object file with basic optimization:

```bash
riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o sum1ton.o sum1ton.c
```

### Explanation

- **`riscv64-unknown-elf-gcc`**: GCC cross-compiler for the 64-bit RISC-V architecture for bare-metal (no os) environment.
- **`-O1`**: Enables basic optimization that improve code performance while keeping compile time reasonable.
- **`-mabi=lp64`**: Selects the 64-bit RISC-V Application Binary Interface (ABI) with 64-bit longs and pointers.
- **`-march=rv64i`**: Targets the RV64I base integer instruction set (no floating-point or other extensions).
- **`-o sum1ton.o`**: Output file name which generates an ELF object for RISC-V architecture.
- **`sum1ton.c`**: Input C source file to be compiled.

**Result:** You now have `sum1ton.o` which is an ELF binary containing RISC-V machine code instead of native x86 instructions.

![Compiled with -O1 optimization](resources/Screenshot%202026-06-02%20133027.png)

---

## Step 5: Disassemble the Binary

Convert the RISC-V binary into human-readable assembly code to inspect the compiler output:

```bash
riscv64-unknown-elf-objdump -d sum1ton.o
```

### Command Explanation

- **`riscv64-unknown-elf-objdump`**: Disassembler utility for RISC-V binaries.
- **`-d`**: Disassemble all sections that contain executable code, convert binary instructions back to assembly language.

**Result:** You can now see the generated RISC-V assembly instructions for each function in the program. This reveals exactly how the C compiler translated your high-level code into machine instructions.

![Objdump disassembly output](resources/Screenshot%202026-06-02%20155126.png)

---

## Step 6: View Assembly with Paging and Search for `main`

Since the disassembly output can be lengthy, use `less` to navigate and search through it:

```bash
riscv64-unknown-elf-objdump -d sum1ton.o | less
```

### Using `less` to Find the `main` Function

Inside `less`, you can search for specific functions:

- Press `/` to open the search prompt.
- Type `main` to search for the main function.
- Press `Enter` to jump to the first match.
- Use `n` to jump to the next match.
- Use `Shift+G` to go to the end of the file.
- Press `q` to quit `less`.

**Step-by-step search:**

![Viewing disassembly in less](resources/Screenshot%202026-06-02%20155217.png)

![Searching for main in less](resources/Screenshot%202026-06-02%20155258.png)

---

## Step 7: Compare Optimization Levels - Compile with -Ofast

Recompile the program using aggressive optimization to observe how the assembly changes:

```bash
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o sum1ton.o sum1ton.c
```

### What is `-Ofast` Optimization?

- **`-Ofast`**: Enables aggressive optimizations aimed at maximum execution speed.
- May ignore strict adherence to standards for the sake of performance.
- Reduces the number of instructions, leading to faster execution.
- May produce different behavior for cases not standard compared to lower optimization levels.

### Instruction Count Comparison

**With `-O1` optimization:** The `main` function contains more instructions (15).

**With `-Ofast` optimization:** The `main` function is reduced to just **12 instructions**.

(Other options are `O0, O2, O3 and Os`)

This reduction demonstrates how compiler optimizations directly impact code size and execution efficiency. More aggressive means deeper and complex set of transformations applied to the code or the code that occupies less memory.

![Comparison - Ofast main instructions](resources/Screenshot%202026-06-02%20161035.png)

You can re-run the `objdump` and `less` commands to verify the reduced instruction count in `main` with `-Ofast`.

**This produces the O1 output** - the functional output from the C model of the RISC-V processor. By comparing O0 (x86 reference) with O1 (RISC-V), we verify that the RISC-V architecture correctly executes the same program with the same results. This equivalence (O0 = O1) confirms that the instruction set and architecture specification are correctly implemented before proceeding to RTL design (O2).

---

## Verification Methodology

The compilation and assembly analysis in this task demonstrates the first critical step in formal verification:

1. **Test Application Development**: Create a simple, deterministic C program with known output.
2. **O0 Baseline (x86 GCC)**: Compile and run on x86 to establish the functional baseline.
3. **O1 Implementation (RISC-V C Model)**: Compile and analyze the RISC-V cross-compiler output to verify architectural correctness.
4. **Functional Equivalence Check**: Verify O0 = O1 by comparing program behavior and outputs.

This methodology ensures that before designing complex hardware (RTL, physical layout, etc.), the fundamental processor behavior is correct. Any mismatch between O0 and O1 would indicate an issue in:
- The RISC-V ISA specification
- The C-level model of the architecture
- The compiler's RISC-V code generation

### SoC Design Flow Diagram

![VLSI SoC Design Architecture Overview](resources/Pasted%20image%2020260602183907.png)

### O0 → O1 → O2 → O3 → O4 Verification Flow

The complete design flow involves multiple verification stages, each with output that must match the previous stage:

![O0-O1 Test Application to C Model Verification](resources/Pasted%20image%2020260602192506.png)

![Hardware RTL Design Stage (O2)](resources/Pasted%20image%2020260602200411.png)

![Analog IP Synthesis and Power/Performance/Area Analysis](resources/Pasted%20image%2020260602200915.png)

![Physical Design and GDSII Generation (O3)](resources/Pasted%20image%2020260602201420.png)

![Final PCB and Tape-out Stage (O4)](resources/Pasted%20image%2020260602201620.png)

---

## Key Learnings

Through this task, the following concepts were explored:

1. **RISC-V Cross-Compilation**: Using specialized compilers to generate code for different architectures.
2. **Compiler Flags**: Understanding what `-O1`, `-Ofast`, `-mabi`, and `-march` do.
3. **Binary Disassembly**: Converting compiled binaries back into human-readable assembly language.
4. **Optimization Impact**: Observing how different compiler optimization levels affect instruction count and code efficiency.
5. **Assembly Code Analysis**: Examining generated assembly to understand compiler behavior and architecture-specific instructions.
6. **SoC Verification Flow**: Understanding how O0 and O1 outputs form the foundation for processor design verification across multiple abstraction levels.

---

## Conclusion

This task provided hands-on experience with the RISC-V compilation pipeline and introduced the systematic verification methodology used in Digital VLSI SoC design. By compiling a simple C program using the RISC-V GCC toolchain and analyzing the generated assembly with different optimization levels, we established:

- **O0 (x86 Reference)**: The functional baseline output from native x86 GCC compilation
- **O1 (RISC-V C Model)**: The output from the C-level RISC-V processor model
- How high-level C code is translated into RISC-V machine instructions
- The relationship between compiler optimizations and instruction count
- The role of flags like `-O1` and `-Ofast` in code generation

The comparison between `-O1` (15 instructions in `main`) and `-Ofast` (12 instructions in `main`) clearly demonstrates how compiler optimizations directly impact program efficiency at the instruction level.

By verifying that **O0 = O1**, we confirm that the RISC-V architecture is correctly implementing the ISA specification before proceeding to more complex design stages: RTL (O2), Physical Design (O3), and PCB/Tape-out (O4).
