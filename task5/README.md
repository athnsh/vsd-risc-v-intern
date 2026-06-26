# FPGA Environment Setup & VSDFPGA Labs Execution

<summary><b>Task 5:</b> Extend the Memory-Mapped GPIO IP into a Full GPIO Control Peripheral (Data + Direction + Readback)</summary>

This task extends the write-only GPIO IP from Task 4 into a complete bidirectional GPIO controller. Instead of a single register, the IP now exposes three registers — output data, direction control, and pin readback — all addressed within the same memory-mapped IO region already wired into the SoC.

---

## Overview

Extend the **Simple GPIO Output IP** built in Task 4 into a **GPIO Control IP** capable of handling multiple registers behind one base address, using address-offset decoding rather than a single flat register.

**IP Specification (fixed, not open-ended):**

| | |
|---|---|
| **Functionality** | Three 32-bit registers behind one base address: `GPIO_DATA` (output data), `GPIO_DIR` (direction control), `GPIO_READ` (pin readback). |
| **Interface** | Memory-mapped, connected to the existing CPU bus, reusing the same bus signals from Task 4 (`mem_addr`, `mem_wdata`, `mem_wstrb`, `mem_rdata`), plus a new 2-bit offset signal to select between the three registers. |
| **Address Map** | Base address inherited from Task 4 (`0x40_0020`); offsets `0x00`, `0x04`, `0x08` select `GPIO_DATA`, `GPIO_DIR`, `GPIO_READ` respectively. |

> **Note:** The base address `0x40_0020` is carried over unchanged from Task 4 — `IO_GPIO_bit = 3` was not reassigned. What changed is *inside* the peripheral: one register became three, selected by the low address bits.

---

## Understanding Check

### Address Offset Decoding

The GPIO Control IP uses a single base address, with individual registers selected using the lower address bits. The SoC extracts these bits using:

```verilog
wire [1:0] gpio_offset = mem_addr[3:2];
```

This produces the following register mapping:

|Address|Offset|Register|
|---|---|---|
|`0x400020`|`2'b00`|`GPIO_DATA`|
|`0x400024`|`2'b01`|`GPIO_DIR`|
|`0x400028`|`2'b10`|`GPIO_READ`|

The GPIO peripheral is first selected using `mem_wordaddr[IO_GPIO_bit]`, after which the `gpio_offset` signal determines which internal register is accessed for read or write operations.

### Effect of the Direction Register

Each bit in `GPIO_DIR` determines whether the corresponding GPIO pin behaves as an input or an output.

- **1** → Output mode
    
- **0** → Input mode
    

The readback register is generated as:

```verilog
assign gpio_read = gpio_data & gpio_dir;
```

This means only pins configured as outputs are reflected in `GPIO_READ`, while pins configured as inputs return `0`. This demonstrates how the direction register controls the behavior of each GPIO pin and how software can configure the peripheral before accessing its data.

---

## Step 1: Revisit the Existing IP (from Task 4)

Task 4 left a single-register, write-only GPIO IP wired onto the CPU bus at `0x40_0020`:

```verilog
reg [31:0] gpio_reg;
```

The bus-side decode (`isIO`, `mem_wordaddr[IO_GPIO_bit]`, the `IO_rdata` mux) was already correct and did not need to change for this task — only the *peripheral's internals* needed extending.

---

## Step 2: Plan the Register Map

Per the task spec, the IP must now handle multiple registers behind one base address:

| Register | Offset | Purpose |
|---|---|---|
| `GPIO_DATA` | `0x00` | Output data register. Writing updates output values; reading returns the last written value. |
| `GPIO_DIR` | `0x04` | Direction register. Each bit: `1` = output, `0` = input. |
| `GPIO_READ` | `0x08` | Readback register. Reflects driven value on output pins, pin state on input pins. |

Each register still occupies one 32-bit word and is accessed through the same base address assigned in Task 4 — only the offset distinguishes them now.

---

## Step 3: Extend the IP RTL (`gpio_ip.v`)

The single `gpio_reg` becomes two state registers, plus offset-based read/write muxing:

```verilog
reg [31:0] gpio_data_reg;
reg [31:0] gpio_dir_reg;
```

**Write path** — gated by `sel && write_en`, routed by `offset`:

```verilog
always @(posedge clk) begin
    if(!resetn) begin
        gpio_data_reg <= 32'b0;
        gpio_dir_reg  <= 32'b0;
    end
    else if(sel && write_en) begin
        case(offset)
            2'b00: gpio_data_reg <= write_data;
            2'b01: gpio_dir_reg  <= write_data;
        endcase
    end
end
```

**Read path** — combinational, also routed by `offset`:

```verilog
always @(*) begin
    if(!sel)
        read_data = 32'b0;
    else begin
        case(offset)
            2'b00: read_data = gpio_data_reg;
            2'b01: read_data = gpio_dir_reg;
            2'b10: read_data = (gpio_data_reg & gpio_dir_reg) | (gpio_in & ~gpio_dir_reg);
            default: read_data = 32'b0;
        endcase
    end
end
```

**Output drive** — pins configured as outputs drive `gpio_data_reg`; pins configured as inputs are masked off:

```verilog
assign gpio_out = gpio_data_reg & gpio_dir_reg;
```

Writing to offset `2'b10` (`GPIO_READ`) is intentionally not handled in the write `case` — it's a read-only register, so no write branch exists for it; an attempted write there is silently dropped.

![GPIO IP RTL extended with offset-decoded registers](task5-resources/Pasted%20image%2020260627021820.png)

---

## Step 4: Integrate the Extended IP into the SoC

The decoder entry from Task 4 (`IO_GPIO_bit = 3`) and the `IO_rdata` mux are untouched. What's new is the offset signal and two extra IP ports:

```verilog
wire gpio_sel;
wire gpio_write;
wire [1:0] gpio_offset;
wire [31:0] gpio_rdata;
wire [31:0] gpio_out;
wire [31:0] gpio_in;

assign gpio_sel    = isIO & mem_wordaddr[IO_GPIO_bit];
assign gpio_write  = gpio_sel & mem_wstrb;
assign gpio_offset = mem_addr[3:2];
assign gpio_in     = 32'b0;
```

> `gpio_sel` replaces the old single-purpose `gpio_write` gate as the IP's overall chip-select; `gpio_write` is now derived from it instead of being computed standalone, and `gpio_offset` is the new signal that lets one base address serve three registers.

**Instantiate the extended peripheral:**

```verilog
gpio_ip GPIO(
    .clk(clk),
    .resetn(resetn),

    .sel(gpio_sel),
    .write_en(gpio_write),
    .offset(gpio_offset),

    .write_data(mem_wdata),
    .gpio_in(gpio_in),

    .read_data(gpio_rdata),
    .gpio_out(gpio_out)
);
```

The `IO_rdata` mux from Task 4 needs no changes — `gpio_rdata` is still a single 32-bit bus, it's just now multiplexed *inside* the IP based on `offset` before it ever reaches the SoC-level mux:

```verilog
wire [31:0] IO_rdata =
    mem_wordaddr[IO_UART_CNTL_bit] ? {22'b0, !uart_ready, 9'b0} :
    mem_wordaddr[IO_GPIO_bit]      ? gpio_rdata :
                                      32'b0;
```

### Address Offset Decoding

The GPIO Control IP uses a single base address, with individual registers selected using the lower address bits. The SoC extracts these bits using:

```verilog
wire [1:0] gpio_offset = mem_addr[3:2];
```

This produces the following register mapping:

| Address    | Offset  | Register    |
| ---------- | ------- | ----------- |
| `0x400020` | `2'b00` | `GPIO_DATA` |
| `0x400024` | `2'b01` | `GPIO_DIR`  |
| `0x400028` | `2'b10` | `GPIO_READ` |

The GPIO peripheral is first selected using `mem_wordaddr[IO_GPIO_bit]`, after which the `gpio_offset` signal determines which internal register is accessed for read or write operations.

![GPIO IP integrated into the SoC with offset and chip-select wiring](task5-resources/Pasted%20image%2020260627023001.png)

### Effect of the Direction Register

Each bit in `GPIO_DIR` determines whether the corresponding GPIO pin behaves as an input or an output.

* **1** → Output mode
* **0** → Input mode

The readback register is generated as:

```verilog
assign gpio_read = gpio_data & gpio_dir;
```

This means only pins configured as outputs are reflected in `GPIO_READ`, while pins configured as inputs return `0`. This demonstrates how the direction register controls the behavior of each GPIO pin and how software can configure the peripheral before accessing its data.

---

## Step 5: Update the Software Header (`io.h`)

Task 4's single `IO_GPIO_DATA` define becomes three offset-based defines, all still relative to the same `IO_BASE`:

```c
#define IO_GPIO_DATA  32
#define IO_GPIO_DIR   36
#define IO_GPIO_READ  40

#define IO_IN(port)       *(volatile uint32_t*)(IO_BASE + (port))
#define IO_OUT(port,val)  *(volatile uint32_t*)(IO_BASE + (port)) = (val)
```

`32`, `36`, and `40` are the byte offsets from `IO_BASE` (`0x40_0000`) corresponding to word-offsets `0x20`, `0x24`, `0x28` — i.e. base address `0x40_0020` plus the register offset table above.

![Updated io.h with three offset-based GPIO register defines](task5-resources/Pasted%20image%2020260627023148.png)

---

## Step 6: Validate Using Simulation

`gpio_test.c` was extended to exercise all three registers across two distinct direction configurations:

```c
IO_OUT(IO_GPIO_DIR, 0xFFFFFFFF);          // all pins as outputs
printf("GPIO_DIR : 0x%x\n", IO_IN(IO_GPIO_DIR));
IO_OUT(IO_GPIO_DATA, 0x12345678);
printf("GPIO_DATA: 0x%x\n", IO_IN(IO_GPIO_DATA));
printf("GPIO_READ: 0x%x\n", IO_IN(IO_GPIO_READ));

IO_OUT(IO_GPIO_DIR, 0x000000FF);          // only lower byte as outputs
printf("GPIO_DIR : 0x%x\n", IO_IN(IO_GPIO_DIR));
IO_OUT(IO_GPIO_DATA, 0xA5A5A5A5);
printf("GPIO_DATA: 0x%x\n", IO_IN(IO_GPIO_DATA));
printf("GPIO_READ: 0x%x\n", IO_IN(IO_GPIO_READ));
```

![Extended gpio_test.c exercising GPIO_DATA, GPIO_DIR, and GPIO_READ](task5-resources/Pasted%20image%2020260627023443.png)

**Compile:**

```bash
make gpio_test.bram.hex
```

**Simulate** (same `bench.v` clock-stub setup from Task 4 — no changes needed there since the bus interface didn't change):

```bash
vvp gpio_sim.vvp
gtkwave gpio_sim.vcd
```

![Icarus Verilog simulation run for the extended GPIO IP](task5-resources/Pasted%20image%2020260627024405.png)

**Expected behaviour confirmed in simulation:**

- With `GPIO_DIR = 0xFFFFFFFF`, `GPIO_READ` reflects the full value written to `GPIO_DATA` (`0x12345678`) — every bit is in output mode, so every bit passes through.
- With `GPIO_DIR = 0x000000FF`, only the lowest byte of `GPIO_DATA` (`0xA5A5A5A5` → `0xA5`) appears in `GPIO_READ`; the upper three bytes read back as `0` since those pins are now in input mode and `gpio_in` is tied to `32'b0` in this testbench (no external stimulus driving them).

This confirms the direction register is functionally gating the readback path, not just a passive status flag.

---

## Critical Lessons & Mistakes Made During Implementation

### Lesson 1: One base address can serve many registers: that's what offset decoding is for

Task 4 hard-wired one register to one address. Extending to three registers didn't require three new `IO_<x>_bit` decoder entries — it only required slicing two more address bits (`mem_addr[3:2]`) *inside* the existing single-peripheral selection. This is the same pattern every real peripheral (UART, SPI, timer) uses internally once you go beyond a single register.

### Lesson 2: The direction register doesn't just store a value: it must actually gate behaviour

It would be possible to add a `GPIO_DIR` register that simply stores whatever is written to it without doing anything. The actual requirement is that `GPIO_READ` honours it: `gpio_out = gpio_data_reg & gpio_dir_reg`, and readback masks input-configured bits to the external pin state rather than echoing back stale output data. Validating this needed two *different* direction configurations in the test, not just one — a single config can't distinguish "direction register works" from "direction register is ignored."

### Lesson 3: Read-only registers need an explicit absence of a write path

`GPIO_READ` (offset `2'b10`) has no entry in the write-side `case` statement. In Verilog, an unhandled `case` branch for a `reg` driven only inside `always @(posedge clk)` simply holds its previous value — there's nothing to accidentally overwrite, but it's worth being deliberate about which offsets are write-enabled rather than relying on this by accident.

---

## Summary: **Task 5**

By completing this task I have:

- Extended a single-register, write-only GPIO IP (Task 4) into a three-register, bidirectional GPIO control peripheral
- Implemented address-offset decoding (`mem_addr[3:2]`) to multiplex multiple registers behind one base address, without touching the SoC-level decoder
- Implemented a direction register that actively gates the readback path (`gpio_data & gpio_dir`), rather than merely storing a configuration value
- Updated the C-side header (`io.h`) to expose `GPIO_DATA`, `GPIO_DIR`, and `GPIO_READ` as distinct addressable registers
- Validated the extension end-to-end by exercising two different direction configurations in simulation and confirming `GPIO_READ` correctly masks non-output bits