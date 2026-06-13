Environment Setup & RISC-V Reference Bring-Up
Step 1: Set up GitHub Codespace
Step 2: Verify RISC-V Reference Flow 
Completed in task 1


Step 3: Clone and Run VSDFPGA Labs 
![](Pasted%20image%2020260611144344.png)
# General dependencies
sudo apt-get install git vim autoconf automake autotools-dev curl libmpc-dev \
libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool \
patchutils bc zlib1g-dev libexpat1-dev gtkwave picocom -y

installed
![](Pasted%20image%2020260611145237.png)

Now the task pdf mentions not to download the fpga related toolchain and will be downloaded later when RTL and integration confidence is built. But here i have downloaded the toolchain so as to make the environment ready.

sudo apt-get install yosys nextpnr-ice40 icestorm iverilog -y
![](Pasted%20image%2020260611175853.png)
![](Pasted%20image%2020260611180249.png)
icestorm unavailable so installed fpga-icestorm package

![](Pasted%20image%2020260611180541.png)
cd ~
mkdir -p riscv_toolchain && cd riscv_toolchain
wget "https://static.dev.sifive.com/dev-tools/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14.tar.gz"

now
![](Pasted%20image%2020260611180703.png)
tar -xvzf riscv64-unknown-elf-gcc-*.tar.gz

![](Pasted%20image%2020260611180941.png)
echo 'export PATH=$HOME/riscv_toolchain/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-linux-ubuntu14/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
![](Pasted%20image%2020260613014525.png)

SETUP
![](Pasted%20image%2020260613015004.png)
in the github repo its written to attach a CH340 module which is a **USB-to-UART adapter** but in the board datasheet its already written that it has FTDI FT232H USB to SPI, so i will try to display the serial output using that otherwise i will attach another microcontroller which has that module

![](Pasted%20image%2020260613015418.png)
looking at the code
`cd ~/vsdfpga_labs/basicRISCV/Firmware
nano riscv_logo.c  # Review and close (Ctrl+X)
make riscv_logo.bram.hex`
was success
![](Pasted%20image%2020260613015833.png)

```
make clean
make build
```

![](Pasted%20image%2020260613015947.png)
got this error

just removed the -abc9 -device u part from it and it ran
![](Pasted%20image%2020260613130903.png)

ok so this is a mistake we should be doing this on a vm not on the virtual codespace given so we will not include all these steps as 

-- Completed setup of VDI --
make riscv_logo.bram.hex
![](Pasted%20image%2020260613135834.png)

now we do cross compiling of this code in riscv and simulate with spike
![](Pasted%20image%2020260613141830.png)

after which we complete the setup of the vm and then run the vsdfpga_labs github and follow its readme.
![](Pasted%20image%2020260613162344.png)
Build the firmware and FPGA bitstream

now we flash it to the fpga
![](Pasted%20image%2020260613163208.png)

error (timeout) then we shall try to fix it
![](Pasted%20image%2020260613164621.png)

ok so basically that was an issue with the USB controller "
USB 1.1 (OHCI) Controller."
![](Pasted%20image%2020260613170644.png)

which stops the dumping of code to the flash of the fpga effectively and introduces irregularities, so i installed the VirtualBox extension pack which contains support for USB 2.0 and 3.0
![](Pasted%20image%2020260613170756.png)
which fixes the issue and the board gets flashed successfully

![](Pasted%20image%2020260613170600.png)

then the terminal doesnt show the output 
![](Pasted%20image%2020260613172316.png)

maybe both flashing and viewing from the FT232H is not possible which is why the original lab demanded a CH340 USB-UART module

but this lab is overstepping its bounds and requirements so i will stop 