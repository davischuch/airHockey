/*
 * positionTracking.h
 *
 *  Created on: May 15, 2025
 *      Author: edrick
 */

#ifndef INC_POSITIONTRACKING_H_
#define INC_POSITIONTRACKING_H_

#include "i2c.h"

#define DATA_ARRAY_SIZE 500 //define the size of the arrays
#define G_TO_MS2 9.79274 //define the conversion factor from g to m/s^2
#define DEG_TO_RAD 0.01745329251994329576923690768489 //define the conversion factor from degrees to radians

void calculateVx(double *accelerationArray, double *velocityArray, double *timeArray, int index);
void calculatePx(double *velocityArray, double *positionArray, double *timeArray, int index);
void shiftArray(double *array);

#endif /* INC_POSITIONTRACKING_H_ */
