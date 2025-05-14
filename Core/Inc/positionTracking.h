<<<<<<< HEAD
#define DATA_ARRAY_SIZE 100 //define the size of the arrays
=======
#define DATA_ARRAY_SIZE 100 // Define the size of the arrays
>>>>>>> c8f1072ad48098600d9be3050242b5fef86587d4

void appendAxData();
void updateVxData();
void updatePxData();
void calibrate();

double readSensorData();

<<<<<<< HEAD
int data_index = 0;         //variable to keep track of the number of elements in the arrays
double Ax[DATA_ARRAY_SIZE]; //array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; //array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; //array to store x axis position
double Tx[DATA_ARRAY_SIZE]; //array to store time
=======
int data_index = 0; // Variable to keep track of the number of elements in the arrays
double Ax[DATA_ARRAY_SIZE]; // Array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; // Array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; // Array to store x axis position
double Tx[DATA_ARRAY_SIZE]; // Array to store time
>>>>>>> c8f1072ad48098600d9be3050242b5fef86587d4

struct timeTracking
{
    double initial;
    double current;
<<<<<<< HEAD
}timeTracking;  //in seconds
=======
}timeTracking;  //in seconds
>>>>>>> c8f1072ad48098600d9be3050242b5fef86587d4
