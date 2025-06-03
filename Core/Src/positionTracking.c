#include "positionTracking.h"
#include "mpu6050.h"
#include <math.h>

void appendAccelerationData(MPU6050_t *DataStruct, double *accelerationArray, double *timeArray, int index, double slope, char axis);
void calibrate(double *XaccelerationArray, double *XvelocityArray, double *XpositionArray, double *YaccelerationArray, double *YvelocityArray, double *YpositionArray, double *timeArray, int *data_index, double *slope, MPU6050_t *DataStruct, I2C_HandleTypeDef *i2c);

void appendAccelerationData(MPU6050_t *DataStruct, double *accelerationArray, double *timeArray, int index, double slope, char axis){
    double realA;
    
    if(index==0){
		timeArray[index]=0;
	}
    else{
    	timeArray[index] = timeArray[index-1]+0.001; //next time is 1ms later
    }

	//filter influence of gravity in Ax
    if(axis=='x'){
        realA = cos(slope) * (DataStruct->Ax + sin(slope));
        realA *= G_TO_MS2; //convert to m/s^2
    }
    else{
        realA = cos(slope) * (DataStruct->Ay - sin(slope));
        realA *= G_TO_MS2;
    }

    if(fabs(realA)<0.1)	return;

    accelerationArray[index] = realA;
}

void calculateVelocity(double *accelerationArray, double *velocityArray, double *timeArray, int index){   
    if(index==0)	return;

	//calculate velocity based on acceleration data
    velocityArray[index] = accelerationArray[index-1] * (timeArray[index]-timeArray[index-1]) + velocityArray[index-1];

    //if all accelerations are zero, set velocity to zero
    for(int i=DATA_ARRAY_SIZE-1; i>=0; i--){
    	if(accelerationArray[i]!=0)	return;
    }
    velocityArray[index] = 0;
}

void calculatePosition(double *velocityArray, double *positionArray, double *timeArray, int index){
	if(index==0)	return;

	//calculate position based on velocity data
    positionArray[index] = velocityArray[index-1] * (timeArray[index]-timeArray[index-1]) + positionArray[index-1];
}

void calibrate(double *XaccelerationArray, double *XvelocityArray, double *XpositionArray, double *YaccelerationArray, double *YvelocityArray, double *YpositionArray, double *timeArray, int *data_index, double *slope, MPU6050_t *DataStruct, I2C_HandleTypeDef *i2c){
	//Kalman filter algorithm requires some time to stabilize
    for (int i = 0; i < 2000; i++) {
        MPU6050_Read_All(i2c, DataStruct);
        HAL_Delay(1);
    }
    slope[0] = DataStruct->KalmanAngleY*DEG_TO_RAD;
    slope[1] = DataStruct->KalmanAngleX*DEG_TO_RAD;
    
    //reset data
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        XaccelerationArray[i] = 0;
        XvelocityArray[i] = 0;
        XpositionArray[i] = 0;
        YaccelerationArray[i] = 0;
        YvelocityArray[i] = 0;
        YpositionArray[i] = 0;
        timeArray[i] = 0;
    }

    *data_index = 0;
}

void shiftArray(double *array){
    //shift the array to the left
    for(int i=0; i<DATA_ARRAY_SIZE-1; i++){
        array[i] = array[i+1];
    }
    array[DATA_ARRAY_SIZE-1] = 0;
}
