#include "positionTracking.h"
#include "mpu6050.h"
#include <math.h>

void appendAxData(MPU6050_t *DataStruct, double *accelerationArray, double *timeArray, int index, double initial_time, double current_time);
void calibrate(double *accelerationArray, double *velocityArray, double *positionArray, double *timeArray, int *data_index, double *current_time, MPU6050_t *DataStruct, I2C_HandleTypeDef *i2c);

void appendAxData(MPU6050_t *DataStruct, double *accelerationArray, double *timeArray, int index, double initial_time, double current_time){
    //filter influence of gravity in Ax
    double realA;
    realA = cos(DataStruct->KalmanAngleY*DEG_TO_RAD) * (DataStruct->Ax + sin(DataStruct->KalmanAngleY*DEG_TO_RAD));
    realA *= G_TO_MS2; //convert to m/s^2

    accelerationArray[index] = sigmoidFilter(realA);

    timeArray[index] = current_time-initial_time; //ps: current time is overwritten right after the mesurement in "MPU6050_Read_Accel"
}

void calculateVx(double *accelerationArray, double *velocityArray, double *timeArray, int index){   
    //calculate velocity based on acceleration data
    velocityArray[index] = accelerationArray[index-1] * (timeArray[index]-timeArray[index-1]) + velocityArray[index-1];
}

void calculatePx(double *velocityArray, double *positionArray, double *timeArray, int index){
    //calculate position based on velocity data
    positionArray[index] = velocityArray[index-1] * (timeArray[index]-timeArray[index-1]) + positionArray[index-1];
}

void calibrate(double *accelerationArray, double *velocityArray, double *positionArray, double *timeArray, int *data_index, double *current_time, MPU6050_t *DataStruct, I2C_HandleTypeDef *i2c){
    double *placeHolder, gravity;
    double averageAx=0;

	//Kalman filter algorithm requires some time to stabilize
    for (int i = 0; i < 2000; i++) {
        MPU6050_Read_All(i2c, DataStruct, &placeHolder);
        HAL_Delay(1);
    }
    
    //reset data
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

double sigmoidFilter(double acceleration){
    double exponent, Fx;
    
    exponent = -21.05*fabs(acceleration)+7.75;
    Fx = 1/(1+exp(exponent));

    return Fx*acceleration;
}
