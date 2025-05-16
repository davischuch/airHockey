/*
 * positionTracking.h
 *
 *  Created on: May 15, 2025
 *      Author: edric
 */

#ifndef INC_POSITIONTRACKING_H_
#define INC_POSITIONTRACKING_H_

#endif /* INC_POSITIONTRACKING_H_ */


#include "stm32f4xx_hal.h"

#define DATA_ARRAY_SIZE 100 //define the size of the arrays
#define G_TO_MS2 9.80665 //define the conversion factor from g to m/s^2

typedef struct
{
    double initial;
    double current;
}timeTracking_t;  //in seconds

void appendAxData(double acceleration);
void updateVxData();
void updatePxData();
void calibrate();
double getTime(uint32_t cycles);

extern int data_index;         //variable to keep track of the number of elements in the arrays
extern double Ax[DATA_ARRAY_SIZE]; //array to store x axis acceleration
extern double Vx[DATA_ARRAY_SIZE]; //array to store x axis velocity
extern double Px[DATA_ARRAY_SIZE]; //array to store x axis position
extern double Tx[DATA_ARRAY_SIZE]; //array to store time
extern timeTracking_t timeTracking;
