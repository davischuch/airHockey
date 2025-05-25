/*
 * positionTracking.h
 *
 *  Created on: May 15, 2025
 *      Author: edric
 */

#ifndef INC_POSITIONTRACKING_H_
#define INC_POSITIONTRACKING_H_

#endif /* INC_POSITIONTRACKING_H_ */


#define DATA_ARRAY_SIZE 500 //define the size of the arrays
#define G_TO_MS2 9.80665 //define the conversion factor from g to m/s^2

void appendAxData(double acceleration, double *accelerationArray, double *timeArray, int index, double initial_time, double current_time);
void updateVxData(double *accelerationArray, double *velocityArray, double *timeArray, int index);
void updatePxData(double *velocityArray, double *positionArray, double *timeArray, int index);
void calibrate(double *accelerationArray, double *velocityArray, double *positionArray, double *timeArray, int *data_index, double *current_time);
void shiftArray(double *array);
