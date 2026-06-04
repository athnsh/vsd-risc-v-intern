write your own applications and do:
GCC
RISCV-GCC
SPIKE


IOT
AIML
Digital Design

this is for -Ofast
so `spike pk sum1ton.o` is used to run the code after compiling with riscv.
![[Pasted image 20260604181157.png]]

for debugging:
we will open the obj dump of the file to check the addresses
`spike -d pk sum1ton.o`
![[Pasted image 20260604182417.png]]
which opens the spike debugger
now we write
`until pc 0 100b0`
main is at 100b0
which takes the program counter to main.
![[Pasted image 20260604182843.png]]
since here a2 is being changed we will check the before and after values.
`reg 0 a2`
![[Pasted image 20260604182946.png]]
press `enter` for the next instruction
![[Pasted image 20260604183050.png]]
now checking content again ![[Pasted image 20260604183121.png]]
![[Pasted image 20260604183211.png]]
so basically `lui` is an instruction which loads the upper immediate of the data it starts from the 12th bit and whatever offset (31 to 12) "0x1" is written is stored after the 12th bit.
destination register takes up bits 11 to 7 and opcode takes 0 to 6
these are in bits bit displayed is hex. each 0 takes up bits 0 to 3. and then the result is appended by 0s to extend to 64 bit

similarly for the next inst.
![[Pasted image 20260604183552.png]]

the next instruction `addi` means add immediate and is adding the stack pointer 
![[Pasted image 20260604183858.png]]![[Pasted image 20260604184243.png]]
register source and register destination
-16 got updated in the stack pointer which is -10 hex 

"|Command|Instruction|Register|Before|After|
|---|---|---|---|---|
|`reg 0 a2`|`lui a2, 0x1`|`a2`|`0x0000000000000000`|`0x0000000000001000`|
|`reg 0 a0`|`lui a0, 0x21`|`a0`|—|`0x0000000000021000`|
|`reg 0 sp`|`addi sp, sp, -16`|`sp`|`0x000000007f7e9b50`|`0x000000007f7e9b40`|"

in the -O1 optimization Load immediate is used `li` more commonly in O1
![[Pasted image 20260604185125.png]]
 