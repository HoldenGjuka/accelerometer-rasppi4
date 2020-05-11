// Author: Holden Gjuka
// Class: Operating Systems
// Instructor: Dr. Caley
// Date: 4/21/2020
// This program takes in data from an accelerometer and prints the data 
// to console. Utilizes threading. Runs on Raspberry Pi 4 Model B Rev 
// 1.1, running the Raspbian OS.
// TO COMPILE:
// gcc -pthread -o a.out main.c -lwiringPi
// TO RUN:
// ./a.out

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

//reads current x value from accelerometer
int generate_x(int fd){
    int x_msb = wiringPiI2CReadReg8(fd, 1) << 4;
    int x_lsb = wiringPiI2CReadReg8(fd, 2) >> 4;
    int x_sign = x_msb >> 11;
    int x_final = x_msb | x_lsb;
    if(x_sign == 1) { x_final = 0xFFFFF000 | x_final; }
    return x_final;
}

//reads current y value from accelerometer
int generate_y(int fd){
    int y_msb = wiringPiI2CReadReg8(fd, 3) << 4;
    int y_lsb = wiringPiI2CReadReg8(fd, 4) >> 4;
    int y_sign = y_msb >> 11;
    int y_final = y_msb | y_lsb;
    if(y_sign == 1) { y_final = 0xFFFFF000 | y_final; }
    return y_final;
}

//reads current z value from accelerometer
int generate_z(int fd){
    int z_msb = wiringPiI2CReadReg8(fd, 5) << 4;
    int z_lsb = wiringPiI2CReadReg8(fd, 6) >> 4;
    int z_sign = z_msb >> 7;
    int z_final = z_msb | z_lsb;
    if(z_sign == 1) { z_final = 0xFFFFF000 | z_final; }
    return z_final;
}

//calculates the average value of an array
double average(int counter, int *vals) {
    double sum = 0.0;
    for(int i = 0; i < counter; i++){
        sum += vals[i];
    }
    return sum / counter;
}

//finds the minimum value of an array
int find_min(int counter, int *vals) {
    int min = 1000000;
    for(int i = 0; i < counter; i++){
        if(i == 0 || vals[i] < min){
            min = vals[i];
        }
    }
    return min;
}

//finds the maximum value of an array
int find_max(int counter, int *vals) {
    int max = 0;
    for(int i = 0; i < counter; i++){
        if(i == 0 || vals[i] > max){
            max = vals[i];
        }
    }
    return max;
}

//displays the avg, min, and max of an array
void displayStats(int counter, int *vals){
    printf("Average: %d\n", average(counter, vals));
    printf("Minimum: %d\n", find_min(counter, vals));
    printf("Max: %d\n", find_max(counter, vals));
}

//handles the x value
void* runner1(int *fd_ptr){
    int fd = *fd_ptr;
    int x_counter = 0;
    int x_vals[1000];
    while(1){
        pthread_mutex_lock(&mutex1);
        int x = generate_x(fd);
        printf("x: %d\n" + x);
        // x_counter++;
        // displayStats(x_counter, x_vals);
        pthread_mutex_unlock(&mutex1);
    }
}

//handles the y value
void* runner2(int *fd_ptr){
    int fd = *fd_ptr;
    int y_counter = 0;
    int y_vals[1000];
    while(1){
        pthread_mutex_lock(&mutex2);
        int y = generate_y(fd);
        printf("y: %d\n" + y);
        // y_counter++;
        // displayStats(y_counter, y_vals);
        pthread_mutex_unlock(&mutex2);
    }
}

//handles the z value
void* runner3(int *fd_ptr){
    int fd = *fd_ptr;
    int z_counter = 0;
    int z_vals[1000];
    while(1){
        pthread_mutex_lock(&mutex3);
        int z = generate_z(fd);
        printf("z: %d\n" + z);
        // z_counter++;
        // displayStats(z_counter, z_vals);
        pthread_mutex_unlock(&mutex3);
    }
}

int main(){
    
    int fd = wiringPiI2CSetup(0x1d);
    int *fd_ptr = &fd;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    pthread_create(&tid1, &attr, (void*) &runner1, fd_ptr);
    pthread_create(&tid2, &attr, (void*) &runner2, fd_ptr);
    pthread_create(&tid3, &attr, (void*) &runner3, fd_ptr);
}
