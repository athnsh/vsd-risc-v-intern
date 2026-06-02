![[Pasted image 20260602131032.png]]
during initial setup forgot to change the directory to sample programs.

![[Pasted image 20260602131135.png]]
fixed the directory.

![[Pasted image 20260602132109.png]]
opening the sum1ton.c file in the sample programs and checking the code, the tutorial showed open on leafedit but gedit is installed so opened on that
![[Pasted image 20260602132454.png]]
edited it to run the n = 5 code and verified.

now for the riscv and spike compiler
![[Pasted image 20260602133030.png]]
we display and then we use the compiler to 
Execute the following command:

```shell
riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o sum1ton.o sum1ton.c
```

This command cross-compiles the C source file for the 64-bit RISC-V architecture using basic `-O1` optimization, generating an ELF object file (`sum_1ton.o`) that contains RISC-V machine instructions instead of native x86 instructions.

then we 

Run the following command to disassemble the object file and inspect all sections:

```shell
riscv64-unknown-elf-objdump -d sum1ton.o
```

`objdump -d` reverse-translates the compiled binary back into human-readable RISC-V assembly, allowing you to examine exactly which instructions the compiler generated for each function in your program.
![[Pasted image 20260602155130.png]]
then we View Assembly in `less` and Search for `main`
![[Pasted image 20260602155220.png]]
search for main by doing /. main
![[Pasted image 20260602155308.png]]

then we will run the same command but without -O1 `-Ofast` Optimization — 12 Instructions in `main`
![[Pasted image 20260602161044.png]]
does faster execution 

---

Digital VLSI SoC: Design & Planning

designing or testing of processor which supports all specifications (not talking about iot/embedded)

first thing is architecture
![[Pasted image 20260602183907.png]]
test app (testbench) is written in c language then we port to GCC to simulate and check the functionality of the app, the output that is measured is called O0 (gcc is for x86), now we use a model for RISC V processor (c model) which gives an output O1. Whole purpose of this is to see if O1 = O0.
![[Pasted image 20260602192506.png]]
so now we model it in Hardware description language RTL (verilog) \[in industry we use bluespac and chisel\] the output gives O2. and then check if equality stands O1 = O2.
power performance and area.
![[Pasted image 20260602200411.png]]
for analog IPs we use synthesizable verilog code for verifying only we build it on MOSFETs
![[Pasted image 20260602200915.png]]
![[Pasted image 20260602201420.png]]
physical design
GDSII: Graphic Data System II its basically a file which has entire flat description of the verilog, metal layers cmos polysilicon. no relation between verilog and gdsII. verification here by c program is very heavy task so we use LVS/DRC checks electrical connections.

tape out: sending a file from our location to foundry. tape in is opposite.
![[Pasted image 20260602201620.png]]
then we build the pcb. O1=O2=O3=O4
