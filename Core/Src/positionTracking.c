#include "positionTracking.h"

void appendAxData(double acceleration, double *accelerationArray, double *timeArray, int index, double initial_time, double current_time){
    accelerationArray[index] = acceleration*G_TO_MS2;

    timeArray[index] = current_time-initial_time; //ps: current time is overwritten right after the mesurement in "MPU6050_Read_Accel"
}

void updateVxData(double *accelerationArray, double *velocityArray, double *timeArray, int index){   
    //calculate velocity based on acceleration data
    for(int i=1; i<=index; i++){
    	velocityArray[i] = accelerationArray[i-1] * (timeArray[i]-timeArray[i-1]) + velocityArray[i-1];
    }
}

void updatePxData(double *velocityArray, double *positionArray, double *timeArray, int index){
    //calculate position based on velocity data
    for(int i=1; i<=index; i++){
        positionArray[i] = velocityArray[i-1] * (timeArray[i]-timeArray[i-1]) + positionArray[i-1];
    }
}

void calibrate(double *accelerationArray, double *velocityArray, double *positionArray, double *timeArray, int *data_index, double *current_time){
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        accelerationArray[i] = 0;
        velocityArray[i] = 0;
        positionArray[i] = 0;
        timeArray[i] = 0;
    }

    *data_index = 0;
    *current_time = (double)(HAL_GetTick())/1000;    //initialize the time tracking
}

void shiftArray(double *array){
    //shift the array to the left
    for(int i=0; i<DATA_ARRAY_SIZE-1; i++){
        array[i] = array[i+1];
    }
    array[DATA_ARRAY_SIZE-1] = 0;
}
