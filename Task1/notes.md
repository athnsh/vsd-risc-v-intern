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