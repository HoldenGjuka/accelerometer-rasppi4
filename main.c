// Author: Holden Gjuka
// Class: Operating Systems
// Instructor: Dr. Caley
// Date: 4/21/2020
// This program takes in data from an accelerometer and prints the data to console.
// Utilizes threading/multiple processes. Runs on Raspberry Pi 4 Model B Rev 1.1,
// running the Raspbian OS.
// TO COMPILE:
// gcc -pthread -o a main.c -lwiringPi
// ./a

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


const int limit = 1000;
int counter = 0;
int nums[1000];
pthread_mutex_t mutex1;


void generate(){
    pthread_mutex_lock(&mutex1);
    counter++;
    srand(time(0));
    nums[counter] = rand() % 100 + 1;
    pthread_mutex_unlock(&mutex1);
}


void* thread_1_main(){
    unsigned int exec_period = 1000000;
    while(1){
        usleep(exec_period);
        generate();
    }
}

double average() {
    double sum = 0.0;
    for(int i = 0; i < counter; i++){
        sum += nums[i];
    }
    return sum / counter;
}

int find_min() {
    int min = 101;
    for(int i = 0; i < counter; i++){
        if(nums[counter] < min){
            min = nums[counter];
        }
    }
    return min;
}

int find_max() {
    int max;
    for(int i = 0; i < counter; i++){
        if(i == 0 || nums[i] > max){
            max = nums[i];
        }
    }
    return max;
}

void* thread_2_main(){
    unsigned int exec_period = 1000000;
    while(1){
        usleep(exec_period);
        printf("Average: %f\n", average());
        printf("Minimum: %i\n", find_min());
        printf("Max: %i\n", find_max());
    }
}

int main(){
    int fd = wiringPiI2CSetup(0x1d);
    int val = wiringPiI2CRead(fd);
    //Dr. Caley, you said in lecture that int produces weird behavior but
    //that makes no sense to me because I can't see how it would do that?
    int x_lsb = wiringPiI2CReadReg8(fd, 1);  //least_significant_byte LSB
    int x_msb = wiringPiI2CReadReg8(fd, 2)>>4;  //most_significant_byte MSB
    int x_sign = x_msb>>7;
    int y_lsb = wiringPiI2CReadReg8(fd, 3);
    int y_msb = wiringPiI2CReadReg8(fd, 4)>>4;
    int y_sign = y_msb>>7;
    int z_lsb = wiringPiI2CReadReg8(fd, 5);
    int z_msb = wiringPiI2CReadReg8(fd, 6)>>4;
    int z_sign = z_msb>>7;
    int x_final = ((x_msb << 4| x_lsb) ^ (x_sign<<7)) | (x_sign<<31);
    int y_final = ((y_msb << 4| y_lsb) ^ (y_sign<<7)) | (y_sign<<31);
    int z_final = ((z_msb << 4| z_lsb) ^ (z_sign<<7)) | (z_sign<<31);
    if(x_sign == 1) { x_final = (x_sign<<31) | ~x_final; }
    if(y_sign == 1) { y_final = (y_sign<<31) | ~y_final; }
    if(z_sign == 1) { z_final = (z_sign<<31) | ~z_final; }
    printf("x: %d, y: %d, z: %d", x_final, y_final, z_final);
    

    pthread_t tid1;
    pthread_t tid2;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    int* nums_ptr = nums;
    pthread_create(&tid1, &attr, (void*) &thread_1_main, nums_ptr);
    pthread_create(&tid2, &attr, (void*) &thread_2_main, nums_ptr);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}
