#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "positionTracking.h"
#include "stm32f4xx_hal.h"

int data_index;         //variable to keep track of the number of elements in the arrays
double Ax[DATA_ARRAY_SIZE]; //array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; //array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; //array to store x axis position
double Tx[DATA_ARRAY_SIZE]; //array to store time

timeTracking_t timeTracking;

void appendAxData(double acceleration){
    //STILL NOT FEATURED: start data overwriting when the array is full
    if(data_index<DATA_ARRAY_SIZE) data_index++;

    Ax[data_index] = acceleration*G_TO_MS2;

    Tx[data_index] = timeTracking.current-timeTracking.initial; //ps: current time is overwritten right after the mesurement in "MPU6050_Read_Accel"
}

void updateVxData(){
    //calculate velocity based on acceleration data
    for(int i=1; i<data_index; i++){
        Vx[i] = Ax[i-1] * (Tx[i]-Tx[i-1]) + Vx[i-1];
    }
}

void updatePxData(){
    //calculate position based on velocity data
    for(int i=1; i<data_index; i++){
        Px[i] = Vx[i-1] * (Tx[i]-Tx[i-1]) + Px[i-1];
    }
}

void calibrate(){
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        Ax[i] = 0;
        Vx[i] = 0;
        Px[i] = 0;
        Tx[i] = 0;
    }

    data_index = 0;
    timeTracking.initial = getTime(DWT->CYCCNT);    //initialize the time tracking
}

double getTime(uint32_t cycles){
    return (double)cycles/SystemCoreClock; //convert the number of cycles to seconds
}
