- A memory-mapped Timer IP
- Integration into the RISC-V SoC
- A C program that configures and tests it
- Simulation and documentation

Each IP owner must submit:
```
/ip/<ip_name>/
rtl/
test/
README.md
```
Additionally: 
● Simulation logs or screenshots
● Brief explanation of integration approach

Provide a simple programmable timer that can generate a timeout event and a status flag after counting down.

| Offset | Register | Purpose                 |
| ------ | -------- | ----------------------- |
| 0x00   | CTRL     | Enable, mode, prescaler |
| 0x04   | LOAD     | Initial countdown value |
| 0x08   | VALUE    | Current count           |
| 0x0C   | STATUS   | Timeout flag            |

![Timer register map and register interface plan](ip/ap_timer_ip/test/Pasted%20image%2020260704185043.png)


Step 1:

![Timer register interface implementation](ip/ap_timer_ip/test/Pasted%20image%2020260704190522.png)

register interface implementation

next finished all the implementations of the registers

![Timer registers fully implemented](ip/ap_timer_ip/test/Pasted%20image%2020260704195947.png)

and completed integration in SOC

![Timer integrated into the SoC](ip/ap_timer_ip/test/Pasted%20image%2020260704200008.png)

next updated io.h

![Updated io.h with timer register macros](ip/ap_timer_ip/test/Pasted%20image%2020260704200129.png)

next we wrote a testbench program

![Timer validation test program](ip/ap_timer_ip/test/Pasted%20image%2020260704230331.png)

![Timer test program UART output and second run](ip/ap_timer_ip/test/Pasted%20image%2020260704230457.png)


for the hardware i added another output in the verilog file

```
output timeout
assign timeout = timeout_flag;
```

![Timer hardware output and flash run](ip/ap_timer_ip/test/Pasted%20image%2020260706002947.png)

and then did make clean make sudo make flash