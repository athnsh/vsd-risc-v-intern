handle multiple registers inside one IP

ip spec:
GPIO_DATA: 0x00, gpio output data register
GPIO_DIR: 0x04, direction register (1 = out and 0 in)
GPIO_READ: 0x08, readback register

1. GPIO_DATA 
	1. Writing updates output values
	2. Reading returns last written value 
2. GPIO_DIR
	1. Each bit controls direction of corresponding GPIO 
	2. 1 → output enabled
	3. 0 → input mode 
3. GPIO_READ 
	1. Returns current GPIO pin values 
	2. For output pins, reflects driven value 
	3. For input pins, reflects pin state

Step 1: Study and plan
The CPU accessed this register through memory-mapped I/O using one address. The new GPIO peripheral must support both data storage and GPIO direction control.

Each register occupies one 32-bit word and is accessed using the same base address assigned in Task 4.

Step2: 
ok so we will do changes in the gpio_ip.v file now for accommodating the additional spec

![GPIO IP RTL extended with offset-decoded registers](Pasted%20image%2020260627021820.png)
wrote the verilog for the IP RTL

now integrating into the SoC
![GPIO IP integrated into the SoC with offset and chip-select wiring](Pasted%20image%2020260627023001.png)

step4: 
and the io.h
![Updated io.h with three offset-based GPIO register defines](Pasted%20image%2020260627023148.png)

gpio test
![Extended gpio_test.c exercising GPIO_DATA, GPIO_DIR, and GPIO_READ](Pasted%20image%2020260627023443.png)

then compiling by 
make gpio_test.bram.hex

then simulation by iverilog
![Icarus Verilog simulation run for the extended GPIO IP](Pasted%20image%2020260627024405.png)
