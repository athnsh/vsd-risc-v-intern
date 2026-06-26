#include <stdio.h>
#include "io.h"

int main(void){
    IO_OUT(IO_GPIO_DIR, 0xFFFFFFFF);
    printf("GPIO_DIR : 0x%x\n", IO_IN(IO_GPIO_DIR));
    IO_OUT(IO_GPIO_DATA, 0x12345678);
    printf("GPIO_DATA: 0x%x\n", IO_IN(IO_GPIO_DATA));
    printf("GPIO_READ: 0x%x\n", IO_IN(IO_GPIO_READ));

    IO_OUT(IO_GPIO_DIR, 0x000000FF);
    printf("GPIO_DIR : 0x%x\n", IO_IN(IO_GPIO_DIR));
    IO_OUT(IO_GPIO_DATA, 0xA5A5A5A5);
    printf("GPIO_DATA: 0x%x\n", IO_IN(IO_GPIO_DATA));
    printf("GPIO_READ: 0x%x\n", IO_IN(IO_GPIO_READ));
    return 0;
}