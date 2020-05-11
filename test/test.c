//Tests bit shifting
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
    short x_msb = 0x00 << 4;
    short x_lsb = 0xF0 >> 4;
    short x_sign = x_msb >> 11;
    short x_final = x_msb | x_lsb;
    if(x_sign == 1) { x_final = (x_sign << 15) | ~x_final; }
    printf("x_msb: %hi\n", x_msb);
    printf("x_lsb: %hi\n", x_lsb);
    printf("x_final: %hi\n", x_final);
    printf("x_sign: %hi\n", x_sign);
    return 0;
}