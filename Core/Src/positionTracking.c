#include "positionTracking.h"
#include <math.h>

void appendAxData(double acceleration, double *accelerationArray, double *timeArray, int index, double initial_time, double current_time){
    if(fabs(acceleration)>0.0714){  //disconsider small accelerations (noise)
        accelerationArray[index] = acceleration*G_TO_MS2;
    }

    timeArray[index] = current_time-initial_time; //ps: current time is overwritten right after the mesurement in "MPU6050_Read_Accel"
}

void calculateVx(double *accelerationArray, double *velocityArray, double *timeArray, int index){   
    //calculate velocity based on acceleration data
    velocityArray[index] = accelerationArray[index-1] * (timeArray[index]-timeArray[index-1]) + velocityArray[index-1];

    //if the last 30 acceleration measurements were 0, set the velocity to 0
    for(int i=index; i>index-30; i--){
        if(accelerationArray[i]!=0) return;
    }
    velocityArray[index] = 0;
}

void calculatePx(double *velocityArray, double *positionArray, double *timeArray, int index){
    //calculate position based on velocity data
    positionArray[index] = velocityArray[index-1] * (timeArray[index]-timeArray[index-1]) + positionArray[index-1];
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
