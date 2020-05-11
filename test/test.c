//Tests bit shifting
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
    int x_msb = 0x00F8;
    int x_lsb = 0x0010;
    int x_sign = x_msb >> 7;
    int x_final = (x_msb << 4) | (x_lsb >> 4);
    printf("size of int %d\n", sizeof(int));
    //x_final = 0xFF81;  //-127
    printf("x_final: %08x\n", x_final);
    printf("x_final: %hi\n", x_final);
    return 0;
}