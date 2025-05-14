#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Inc/positionTracking.h"

int main(){
    calibrate();
    
    for(int i=0; i<DATA_ARRAY_SIZE-1; i++){
        appendAxData(); //append random data to Ax
    }
    updateVxData(); //calculate Vx based on Ax
    updatePxData(); //calculate Px based on Vx

    
    //print the results
    printf("Ax: ");
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        printf("%lf ", Ax[i]);
    }
    printf("\nVx: ");
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        printf("%lf ", Vx[i]);
    }
    printf("\nPx: ");
    for(int i=0; i<DATA_ARRAY_SIZE; i++){
        printf("%lf ", Px[i]);
    }

    return 0;
}

void appendAxData(){
    if(data_index<DATA_ARRAY_SIZE) data_index++;
    
    //tasks that have to be done simultaneously
    Ax[data_index] = readSensorData();
    timeTracking.current = (double)clock()/CLOCKS_PER_SEC;

    Tx[data_index] = timeTracking.current-timeTracking.initial;

    usleep(100000); //delay for 100ms so dt is not so small and the calculations are more visible
}

void updateVxData(){
    /*
        In this example, the Ax data is been filled and then we are calculating the Vx data.
        In the real application, the Vx data will be calculated in the same loop as the Ax data, and so
    data_index is needed to limit the number of elements in the arrays.
    */
    data_index = DATA_ARRAY_SIZE;
    
    //calculate velocity based on acceleration data
    for(int i=1; i<data_index; i++){
        Vx[i] = Ax[i-1] * (Tx[i]-Tx[i-1]) + Vx[i-1];
    }
}

void updatePxData(){
    /*
        In this example, the Vx data is been filled and then we are calculating the Px data.
        In the real application, the Px data will be calculated in the same loop as the Vx data, and so
    data_index is needed to limit the number of elements in the arrays.
    */
    data_index = DATA_ARRAY_SIZE;
    
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

    timeTracking.initial = (double)clock()/CLOCKS_PER_SEC;  //initialize the time tracking
}

double readSensorData(){
    if(data_index<50){
        return 1;
    }
    else return -1;
}
