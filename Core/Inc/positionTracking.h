#include "stm32f4xx_hal.h"

#define DATA_ARRAY_SIZE 100 //define the size of the arrays
#define G_TO_MS2 9.80665 //define the conversion factor from g to m/s^2

void appendAxData(double acceleration);
void updateVxData();
void updatePxData();
void calibrate();
double getTime(uint32_t cycles);

int data_index = 0;         //variable to keep track of the number of elements in the arrays
double Ax[DATA_ARRAY_SIZE]; //array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; //array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; //array to store x axis position
double Tx[DATA_ARRAY_SIZE]; //array to store time

struct timeTracking
{
    double initial;
    double current;
}timeTracking;  //in seconds
