basically in this task we have to build a peripheral(ip) and attach it to the risc-v cpu

to build: GPIO Output IP
specs: 
- 1 x 32-bit register
- Writing updates an output signal
- Reading returns the previously written value
Memory-mapped, connected to the existing CPU bus 
Uses the same bus signals already present in the SoC Address Map 
(example) 
Base address assigned by mentor (e.g. 0x2000_0000)
Offset 0x00 → GPIO output register

`uint32_t x = *(volatile uint32_t *)0x20000000;`
reading this returns the value in the gpio register 

![[Pasted image 20260619123133.png]]

![[Pasted image 20260619123659.png]]
this is address decoder in riscv.v

CPU generates an address.
Example:
```
0x00000020
```
The SoC then asks:
```
Is this RAM?orIs this an IO peripheral?
```
This line does that:
```
wire isIO = mem_addr[22];
```
If bit 22 = 0
```
CPU ---> RAM
```
If bit 22 = 1
```
CPU ---> IO peripheral
```
Exactly the same idea as STM32:
0x08000000 -> Flash
0x20000000 -> SRAM
0x40000000 -> Peripherals

cpu has bus wires: `mem_addr`, `mem_wdata`, `mem_wmask`, `mem_rdata`, and `mem_wstrb`
`*(volatile uint32_t*)addr = 0x55;`
mem_addr  = addr
mem_wdata = 0x55
mem_wmask = 1111

existing peripherals:
![[Pasted image 20260619125045.png]]
led peripheral
![[Pasted image 20260619125249.png]]
uart peripheral

now moving onto step 2
writing gpio_ip.v
so gpio has basically 3 responsibilities writing to register

`reg [31:0] gpio_reg;`

then the cpu must write to this register and finally
when the cpu must read from the gpio stored register value is returned


Now Step 3:
so basically integrate the gpio ip into the riscv main program
so first we make a new address decoder entry
![[Pasted image 20260621000948.png]]
`localparam IO_GPIO_bit = 3;`

now we will make gpio bus signals
![[Pasted image 20260621001348.png]]

```
wire gpio_write;
wire [31:0] gpio_rdata;
wire [31:0] gpio_out;
assign gpio_write = isIO & mem_wstrb & mem_wordaddr[IO_GPIO_bit];
```

IO selected AND CPU is writing AND GPIO address selected
      ↓
enable GPIO write

now we instantiate this peripheral
& update readback mux 
![[Pasted image 20260621001852.png]]
```
gpio_ip GPIO(
    .clk(clk),
    .resetn(resetn),
    .write_en(gpio_write),
    .write_data(mem_wdata),
    .read_data(gpio_rdata),
    .gpio_out(gpio_out)
);
```

```
wire [31:0] IO_rdata =
mem_wordaddr[IO_UART_CNTL_bit]
? {22'b0,!uart_ready,9'b0}
: mem_wordaddr[IO_GPIO_bit]
? gpio_rdata
: 32'b0;
```

This step is similar to adding a new peripheral to the STM32 memory map. The GPIO IP is assigned an address region, connected to the CPU bus, and can now be accessed using normal memory read and write operations.

step 4:
![[Pasted image 20260621002728.png]]
This tells us exactly how the addressing works
so we need to 
```
IO_GPIO_bit = 3
```
So:
```
(1<<3)<<2
8<<2
32
```
so we must define IO_GPIO to 32 then our GPIO registers address becomes 0x400020


now we will write a testbench to verify the output
![[Pasted image 20260621003359.png]]


now doing compiling
`make gpio_test.bram.hex`
![[Pasted image 20260621003452.png]]


trying to simulate first there shows no bench.v
![[Pasted image 20260621003741.png]]
next there was an issue in including the file and issue with SB_HFOSC and SB_PLL40_CORE which i think are the clocks so bench.v is required which will create fake stubs to provide for the missing hardwares
![[Pasted image 20260621003734.png]]

which is present in the riscv.v file so we will use those by adding the definitions for the clock sources.
we write a bench.v
![[Pasted image 20260621004750.png]]
so we created software replacement for the clocks 
`always #20 clk_gen = ~clk_gen;`

SOC uut used similar to main and will call when SOC is called in the main riscv program
we also started the cpu and created dumpfiles

ok printing the output of the compiliation we get this error, which is an error in the printf
![[Pasted image 20260621005414.png]]

after fixing
![[Pasted image 20260621010217.png]]
which perfectly matches the testbench


now doing sim on gtkwave
`gtkwave gpio_sim.vcd`
![[Pasted image 20260621010742.png]]
![[Pasted image 20260621010844.png]]