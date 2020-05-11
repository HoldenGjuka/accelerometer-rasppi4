//Tests bit shifting
//size of int is 4 bytes
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
    int z_msb = 0x80 << 4;
    int z_lsb = 0x10 >> 4;
    printf("z_msb: %08x\n", z_msb);
    int z_sign = z_msb>>11;
    printf("z_sign: %08x\n", z_sign);
    int z_final = z_msb | z_lsb;
    printf("x_final: %08x\n", z_final);
    if(z_sign == 1) { z_final = 0xFFFFF000 | z_final; }
    printf("x_final: %08x\n", z_final);
    printf("x_final: %d\n", z_final);
    return 0;
}