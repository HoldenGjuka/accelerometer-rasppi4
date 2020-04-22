// Author: Holden Gjuka
// Class: Operating Systems
// Instructor: Dr. Caley
// This program takes in data from an accelerometer and prints the data to console. Utilizes threading/multiple processes.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <time.h>


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