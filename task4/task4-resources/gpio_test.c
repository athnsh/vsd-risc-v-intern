#include <stdio.h>
#include "io.h"

int main(void){
    uint32_t val;

    IO_OUT(IO_GPIO,0x00000001);
    val = IO_IN(IO_GPIO);
    printf("GPIO readback: 0x%x\n",val);

    IO_OUT(IO_GPIO,0xFFFFFFFF);
    val = IO_IN(IO_GPIO);
    printf("GPIO readback: 0x%x\n",val);

    return 0;
}