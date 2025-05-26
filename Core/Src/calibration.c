#include "calibration.h"
#include <math.h>

void calibrate(double *slopeX, double *slopeY, double Ax, double Ay, double Az){
    double aux;
    
    //calculate slope X and Y based on accelerometer readings
    aux = atan(-Ax/Az);
    *slopeY = aux*180/PI;
    
    aux = atan(Ay/Az);
    *slopeX = aux*180/PI;
}
