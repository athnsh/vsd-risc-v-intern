write your own applications and do:
GCC
RISCV-GCC
SPIKE


IOT
AIML
Digital Design

this is for -Ofast
so `spike pk sum1ton.o` is used to run the code after compiling with riscv.
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604181157.png]]

for debugging:
we will open the obj dump of the file to check the addresses
`spike -d pk sum1ton.o`
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604182417.png]]
which opens the spike debugger
now we write
`until pc 0 100b0`
main is at 100b0
which takes the program counter to main.
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604182843.png]]
since here a2 is being changed we will check the before and after values.
`reg 0 a2`
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604182946.png]]
press `enter` for the next instruction
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604183050.png]]
now checking content again ![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604183121.png]]
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604183211.png]]
so basically `lui` is an instruction which loads the upper immediate of the data it starts from the 12th bit and whatever offset (31 to 12) "0x1" is written is stored after the 12th bit.
destination register takes up bits 11 to 7 and opcode takes 0 to 6
these are in bits bit displayed is hex. each 0 takes up bits 0 to 3. and then the result is appended by 0s to extend to 64 bit

similarly for the next inst.
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604183552.png]]

the next instruction `addi` means add immediate and is adding the stack pointer 
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604183858.png]]![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604184243.png]]
register source and register destination
-16 got updated in the stack pointer which is -10 hex 

"|Command|Instruction|Register|Before|After|
|---|---|---|---|---|
|`reg 0 a2`|`lui a2, 0x1`|`a2`|`0x0000000000000000`|`0x0000000000001000`|
|`reg 0 a0`|`lui a0, 0x21`|`a0`|—|`0x0000000000021000`|
|`reg 0 sp`|`addi sp, sp, -16`|`sp`|`0x000000007f7e9b50`|`0x000000007f7e9b40`|"

in the -O1 optimization Load immediate is used `li` more commonly in O1
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604185125.png]]

for the sample c program i designed a pseudo random 32 bit sequence generator using LFSR 
with a seed value of 0x00007D61 ie the initial value which can be anything except 0

the c program is in [[LFSR.c]]

and the feedback polynomial as 0xB4BCD35C
Initialize LFSR with seed  
  
LOOP:  
Extract LSB  
Shift register right  
If LSB == 1  
XOR with polynomial  
Print state  
END LOOP

possible states are 2^32 -1 
therefore this is the period after which values will repeat since its xor

![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604191746.png]]
running the output with both RISCV and GCC works the same 
Output is **identical** to native GCC  which confirms that the RISC-V binary works correctly on SPIKE.

![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604192203.png]]
objdump
so calculating the total instructions in main. there are 29 instructions in main using Ofast optimization.
so now we check for comparison in O1
![[01. projects/summer2k26/VSD/vsd-intern/task2/task2-resources/Pasted image 20260604192606.png]]
main in O1.
so in here there are 31 instructions in main using O1 optimization.

after which we can do step by step execution using the spike debug mode.
