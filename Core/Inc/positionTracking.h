#define DATA_ARRAY_SIZE 100 // Define the size of the arrays

void appendAxData();
void updateVxData();
void updatePxData();
void calibrate();

double readSensorData();

int data_index = 0; // Variable to keep track of the number of elements in the arrays
double Ax[DATA_ARRAY_SIZE]; // Array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; // Array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; // Array to store x axis position
double Tx[DATA_ARRAY_SIZE]; // Array to store time

struct timeTracking
{
    double initial;
    double current;
}timeTracking;  //in seconds