#include <stdint.h>

#define IO_BASE       	0x400000

#define IO_LEDS       	4
#define IO_UART_DAT   	8
#define IO_UART_CNTL  	16

#define IO_GPIO_DATA  	32
#define IO_GPIO_DIR   	36
#define IO_GPIO_READ  	40

#define IO_TIMER_CTRL   64
#define IO_TIMER_LOAD   68
#define IO_TIMER_VALUE  72
#define IO_TIMER_STATUS 76

#define IO_IN(port)       *(volatile uint32_t*)(IO_BASE + (port))
#define IO_OUT(port,val)  *(volatile uint32_t*)(IO_BASE + (port)) = (val)