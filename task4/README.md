# FPGA Environment Setup & VSDFPGA Labs Execution

<summary><b>Task 4:</b> Design & Integrate Your First Memory-Mapped IP (GPIO Output IP)</summary>

This task moves from environment setup and reference bring-up (Task 3) into actual IP development. The objective is to design a simple memory-mapped peripheral, integrate it into the existing RISC-V SoC's address decoder and bus, and validate the integration through simulation. Hardware validation is optional and was not attempted in this task.

---

## Overview

Build a **Simple GPIO Output IP (Write-Only)** and attach it to the RISC-V CPU bus already present in the SoC — the same bus the existing LED and UART peripherals use.

**IP Specification (fixed, not open-ended):**

| | |
|---|---|
| **Functionality** | One 32-bit register. Writing to it updates an output signal (`gpio_out`). Reading it returns the last written value. |
| **Interface** | Memory-mapped, connected to the existing CPU bus, using the bus signals already present in the SoC (`mem_addr`, `mem_wdata`, `mem_wmask`/`mem_wstrb`, `mem_rdata`). |
| **Address Map (spec example)** | Base address assigned by mentor, e.g. `0x2000_0000`; offset `0x00` → GPIO output register. |

> **Note:** `0x2000_0000` in the spec is an illustrative example, not a literal requirement. The address this SoC actually assigns the GPIO register depends on its own existing decode scheme — see Step 3.

---

## Understanding Check

**Ques 1:** What address is used for the GPIO IP, and how was it derived?

***Answer:** Bit 22 of `mem_addr` (`isIO = mem_addr[22]`) decides RAM vs. IO space — when set, the address falls in the `0x40_0000` IO region, the same scheme already used by the LED and UART peripherals. Within IO space, the GPIO peripheral was assigned `IO_GPIO_bit = 3`. Converting that bit position into a byte-address offset gives `(1 << 3) << 2 = 32 = 0x20`. Adding this to the IO base gives the final GPIO register address: **`0x40_0020`**. This differs from the `0x2000_0000` example in the task spec — that was only illustrative; the real address is dictated by this SoC's existing decoder and however many peripherals are already wired in ahead of it.*

**Ques 2:** How does the CPU access the IP?

***Answer:** The CPU uses normal load/store instructions on the bus signals already wired into the SoC. A write such as `*(volatile uint32_t*)0x400020 = 0x55;` drives `mem_addr = 0x400020`, `mem_wdata = 0x55`, `mem_wmask = 1111`. The decoder ANDs `isIO`, `mem_wstrb`, and `mem_wordaddr[IO_GPIO_bit]` together to produce `gpio_write`, which enables the register inside `gpio_ip.v`. A read such as `uint32_t x = *(volatile uint32_t*)0x400020;` is routed back through the `IO_rdata` mux, which selects `gpio_rdata` whenever `mem_wordaddr[IO_GPIO_bit]` is set.*

**Ques 3:** What was validated in simulation?

***Answer:** A C test program wrote known values to the GPIO register and printed the readback over UART. Running this through the Icarus Verilog flow (`make gpio_test.bram.hex`, simulated with a custom `bench.v`) confirmed the printed values matched what was written. GTKWave traces of `gpio_sim.vcd` were used to visually confirm the write pulse, the register update, and the readback value appearing correctly on the bus.*

---

## Step 1: Understand the Existing SoC

Before writing any RTL, the existing address decode and bus structure was reviewed.

**Address decoder (`riscv.v`):**

The CPU drives a 32-bit address, and a single bit decides RAM vs. IO:

```verilog
wire isIO = mem_addr[22];
```

- Bit 22 = 0 → CPU talks to RAM
- Bit 22 = 1 → CPU talks to an IO peripheral

This is the same idea as the STM32 memory map (`0x0800_0000` → Flash, `0x2000_0000` → SRAM, `0x4000_0000` → Peripherals): a fixed address range per device class.

![Address decoder logic in riscv.v](task4-resources/Pasted%20image%2020260619123659.png)

**Bus signals available:** `mem_addr`, `mem_wdata`, `mem_wmask`, `mem_rdata`, `mem_wstrb`. For a write like:

```c
*(volatile uint32_t*)addr = 0x55;
```

the bus carries:

```
mem_addr  = addr
mem_wdata = 0x55
mem_wmask = 1111
```

![GPIO register read example and address concept](task4-resources/Pasted%20image%2020260619123133.png)

**Existing peripherals reviewed:** LED and UART are already memory-mapped onto the IO region the same way the new GPIO IP needs to be.

![LED peripheral](task4-resources/Pasted%20image%2020260619125045.png)

![UART peripheral](task4-resources/Pasted%20image%2020260619125249.png)

---

## Step 2: Write the IP RTL (`gpio_ip.v`)

The GPIO IP has three responsibilities:

1. **Store** a 32-bit value
2. **Write** — update the stored value when the CPU writes
3. **Read** — return the previously stored value when the CPU reads

```verilog
reg [31:0] gpio_reg;
```

Write and readback logic follow synchronous design principles: the register updates on the clock edge when the write-enable signal is asserted, and the readback path simply reflects the currently stored value back to the CPU — no side effects on read.

---

## Step 3: Integrate the IP into the SoC

**New address decoder entry:**

```verilog
localparam IO_GPIO_bit = 3;
```

![New address decoder entry for GPIO](task4-resources/Pasted%20image%2020260621000948.png)

**GPIO bus signals:**

```verilog
wire gpio_write;
wire [31:0] gpio_rdata;
wire [31:0] gpio_out;
assign gpio_write = isIO & mem_wstrb & mem_wordaddr[IO_GPIO_bit];
```

> IO selected **AND** CPU is writing **AND** GPIO address selected → enable GPIO write.

![GPIO bus signal wiring](task4-resources/Pasted%20image%2020260621001348.png)

**Instantiate the peripheral and update the readback mux:**

```verilog
gpio_ip GPIO(
    .clk(clk),
    .resetn(resetn),
    .write_en(gpio_write),
    .write_data(mem_wdata),
    .read_data(gpio_rdata),
    .gpio_out(gpio_out)
);
```

```verilog
wire [31:0] IO_rdata =
    mem_wordaddr[IO_UART_CNTL_bit] ? {22'b0, !uart_ready, 9'b0} :
    mem_wordaddr[IO_GPIO_bit]      ? gpio_rdata :
                                      32'b0;
```

![GPIO instantiation and readback mux update](task4-resources/Pasted%20image%2020260621001852.png)

This step is the RTL equivalent of adding a new peripheral to an STM32 memory map: the GPIO IP gets its own address region, is wired onto the CPU bus, and becomes accessible with normal `volatile` pointer reads/writes from C.

**Address calculation:** `IO_GPIO_bit = 3` is a *bit position* inside the decoder — it needs converting into a *byte-address offset* before it means anything to C:

```
(1 << 3) << 2
= 8 << 2
= 32   →   0x20
```

![Addressing scheme walkthrough](task4-resources/Pasted%20image%2020260621002728.png)

Combined with the IO base region (`isIO` → bit 22 set → base `0x40_0000`), the final GPIO register address is:

```
0x40_0000 + 0x20 = 0x40_0020
```

**Learning:** the `0x2000_0000` address quoted in the task spec is a teaching example only. The actual address is whatever this SoC's existing decoder produces once `IO_GPIO_bit` is slotted in — here, `0x40_0020`.

---

## Step 4: Validate Using Simulation

A small testbench and C test program were written to write values to the GPIO register and read them back via UART print.

![Testbench for GPIO validation](task4-resources/Pasted%20image%2020260621003359.png)

**Compile:**

```bash
make gpio_test.bram.hex
```

![Compilation of gpio_test.bram.hex](task4-resources/Pasted%20image%2020260621003452.png)

### Mistake 1 — Missing `bench.v`

First simulation attempt failed: no `bench.v` was present.

![No bench.v found](task4-resources/Pasted%20image%2020260621003741.png)

A second issue then surfaced: `SB_HFOSC` and `SB_PLL40_CORE` were unresolved. These are ICE40 FPGA hardware primitives (internal oscillator and PLL) referenced in `riscv.v` for on-chip clock generation — they don't exist in a software simulator, so Icarus Verilog can't elaborate the design without something standing in for them.

![SB_HFOSC / SB_PLL40_CORE missing](task4-resources/Pasted%20image%2020260621003734.png)

**Fix:** write a `bench.v` that provides software clock stubs in place of the missing FPGA hardware and drives the SoC directly:

```verilog
always #20 clk_gen = ~clk_gen;
```

The SoC (`uut`) is instantiated the same way the existing top-level program instantiates it, with the testbench generating `clk`/`resetn` and dumping a VCD.

![bench.v providing clock stubs and SoC instantiation](task4-resources/Pasted%20image%2020260621004750.png)

### Mistake 2 — `printf` formatting error

Compiling and running the C test produced incorrect output, traced back to a `printf` formatting issue in the test program rather than a hardware/RTL problem.

![printf error in compiled output](task4-resources/Pasted%20image%2020260621005414.png)

After correcting the format string, the simulated output matched the testbench's expected values exactly.

![Corrected output matching the testbench](task4-resources/Pasted%20image%2020260621010217.png)

**Waveform inspection:**

```bash
gtkwave gpio_sim.vcd
```

![GTKWave trace — GPIO write pulse and register update](task4-resources/Pasted%20image%2020260621010742.png)

![GTKWave trace — GPIO readback value on the bus](task4-resources/Pasted%20image%2020260621010844.png)

---

## Step 5 (Optional): Hardware Validation

Not attempted. The task spec marks this step optional, with no grading advantage, for participants who already have an FPGA board. Will complete this on a later date.

---

## Critical Lessons & Mistakes Made During Implementation

### Lesson 1: Simulators need an explicit testbench — synthesis targets don't give you one for free

`riscv.v` relies on FPGA-specific clock primitives (`SB_HFOSC`, `SB_PLL40_CORE`) that only exist once the design is synthesized onto real ICE40 silicon. Icarus Verilog has no model for them, so any simulation run needs a `bench.v` that stubs these out (e.g. `always #20 clk_gen = ~clk_gen;`) and instantiates the SoC directly as the unit under test.

### Lesson 2: Bit position ≠ byte address

`IO_GPIO_bit = 3` is a bit *index* used inside the decoder, not the address offset itself. It must be shifted into a word-aligned byte address (`(1 << bit) << 2`) before it's meaningful to C pointer arithmetic. Skipping this conversion is an easy way to end up writing to the wrong register.

### Lesson 3: Spec example addresses are illustrative, not literal

The task document's `0x2000_0000` mirrors an STM32-style memory map as a teaching example. The address actually assigned is whatever this SoC's existing decoder produces once the new `IO_GPIO_bit` is slotted in — here, `0x40_0020`, not the spec's example.

### Lesson 4: UART-printed verification is only as good as the `printf` format string

A formatting bug in the C test program produced output that initially looked like a hardware/RTL problem but turned out to be a bug in the verification harness itself. Worth ruling out the test program before assuming the RTL is at fault.

---

## Submission Checklist (per task spec)

- [x] GPIO IP RTL file — `gpio_ip.v`
- [x] SoC integration description — Step 3 (decoder entry, bus wiring, instantiation, readback mux)
- [x] Simulation log / waveform screenshot — Step 4 (`gpio_sim.vcd` in GTKWave)
- [x] Short explanation (address used / CPU access / what was validated) — Understanding Check above
- [ ] Hardware validation (optional — board photo or UART output) — Step 5, not attempted; simulation-only, per task instructions

---

## Summary: **Task 4**

By completing this task I have:

- Designed and implemented my first custom memory-mapped peripheral (**GPIO Output IP**) from scratch
- Understood and extended the SoC's existing address decoder (`isIO`, `mem_wordaddr`, the `IO_<peripheral>_bit` pattern)
- Connected a new IP onto the existing CPU bus (`mem_wdata`, `mem_wstrb`, `mem_rdata`) alongside the LED and UART peripherals
- Learned the difference between a decoder *bit index* and the resulting *byte address* (`(1 << bit) << 2`)
- Debugged a simulation environment missing FPGA-only clock primitives by writing a `bench.v` with software clock stubs
- Validated the design end-to-end with a C test program, UART printout, and GTKWave waveform inspection
