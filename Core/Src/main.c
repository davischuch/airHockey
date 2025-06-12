/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd5110.h"
#include "mpu6050.h"
#include "positionTracking.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum {
	DOWN = 0,
	UP
};

enum {
	RIGHT = 0,
	LEFT
};

typedef struct {
	int width;
	int height;
	int xPosition;
	int yPosition;
	int xDirection; //enum left or right
	int yDirection; //enum um or down
} object_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MIN_X 2
#define MAX_X 82
#define MIN_Y 2
#define MAX_Y 46
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
MPU6050_t MPU6050;

int data_index, shift=0, read=0;    //variable to keep track of the number of elements in the arrays

double slopeY, slopeX;

double Ax[DATA_ARRAY_SIZE]; //array to store x axis acceleration
double Vx[DATA_ARRAY_SIZE]; //array to store x axis velocity
double Px[DATA_ARRAY_SIZE]; //array to store x axis position

double Ay[DATA_ARRAY_SIZE];
double Vy[DATA_ARRAY_SIZE];
double Py[DATA_ARRAY_SIZE];

double timeTracking[DATA_ARRAY_SIZE]; //array to store time
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void checkCollision();
void moveBall();
void moveStick();
void drawBall();
void drawStick();
int _write(int file, uint8_t* p, int len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
object_t ball = {
	.width = 4,
	.height = 4,
	.xPosition = 1,
	.yPosition = 1,
	.xDirection = RIGHT,
	.yDirection = DOWN
};

object_t stick = {
	.width = 10,
	.height = 30,
	.xPosition = MAX_X / 2,
	.yPosition = MAX_Y / 2,
	.xDirection = RIGHT,
	.yDirection = DOWN
};
int ballVelocityDelay = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM14_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, &CMD, 1);

  //wait positive answer from MPU6050 and correct initialization
  while (MPU6050_Init(&hi2c1) == 1); 

  //configures lcd
  lcd5110_init();

  //initial calibration of the sensor (it has to be at rest)
  printf("\n\n\rCalibration in progress\n\rPlease STAY STILL!!!\n\r");
  lcd5110_clear();
  lcd5110_box(MAX_X/2, MAX_Y/2, MAX_X, MAX_Y);
  lcd5110_refresh();
  
  calibrate(Ax, Vx, Px, Ay, Vy, Py, timeTracking, &data_index, &slopeY, &slopeX, &MPU6050, &hi2c1);
  
  printf("Calibration done\n\r");
  lcd5110_clear();
  lcd5110_refresh();

  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_TIM_Base_Start_IT(&htim11);
  HAL_TIM_Base_Start_IT(&htim13);
  HAL_TIM_Base_Start_IT(&htim14);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    while(!read);	//read each 1ms
    MPU6050_Read_Accel(&hi2c1, &MPU6050);

    if(shift){ //if the array is not full, do not shift
      shiftArray(Ax);	//shifts the array to the left
      shiftArray(Vx);
      shiftArray(Px);
      shiftArray(Ay);
      shiftArray(Vy);
      shiftArray(Py);
      shiftArray(timeTracking);
    }
    appendAccelerationData(&MPU6050, Ax, timeTracking, data_index, slopeY, 'x');	//filter, convert to m/s^2 and append acceleration data to the array
    calculateVelocity(Ax, Vx, timeTracking, data_index);	//calculates and filters velocity based on acceleration data
    calculatePosition(Vx, Px, timeTracking, data_index);	//calculates position based on velocity data

    appendAccelerationData(&MPU6050, Ay, timeTracking, data_index, slopeX, 'y');
    calculateVelocity(Ay, Vy, timeTracking, data_index);
    calculatePosition(Vy, Py, timeTracking, data_index);

    //printf("slopeY: %i, Ax: %imm/s^2, Vx: %imm/s, Px: %imm,                 \r", (int)(slope[0]*1000), (int)(Ax[data_index]*1000), (int)(Vx[data_index]*1000), (int)(Px[data_index]*1000)); //print slopeY, acceleration, velocity and position data (X axis)

    if(data_index<DATA_ARRAY_SIZE-1)  data_index++; //if the array is not full, append data to the next index
    else                              shift=1;      //if the array is full, start shifting it and appending data to the last index
    
    read=0; //reset read flag

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Transmit(&huart2, &CMD, 1, TOUT);
	HAL_UART_Receive_IT(&huart2, &CMD, 1);
}

int _write(int file, uint8_t* p, int len)
{
	if(HAL_UART_Transmit(&huart2, p, len, len) == HAL_OK )
	{
		return len;
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM10) {  //20Hz stick speed
		moveStick();
	}
	if (htim->Instance == TIM11) {  //10Hz ball speed
		moveBall();
		checkCollision();

		if (htim->Init.Period == 1679) { //if its currently in 50 Hz
			if (ballVelocityDelay == 50) { //stay like this for one second
				__HAL_TIM_SET_AUTORELOAD(&htim11, 4199); //change back to 10 Hz
				ballVelocityDelay = 0;
			} else ballVelocityDelay++;
		}
	}
	if (htim->Instance == TIM14) {  //50Hz atualização da tela
		lcd5110_clear();

		drawBall();
		drawStick();

		lcd5110_refresh();
	}
	if (htim->Instance == TIM7) read = 1; //timer to read data from MPU6050 every 1ms
}

void checkCollision() {
	int p = stick.yPosition - (stick.height / 2) - 1, //get top position
		  q = stick.yPosition + (stick.height / 2) + 1, //get bottom position
		  r = stick.xPosition - (stick.width / 2) - 1,  //get right position
		  s = stick.xPosition + (stick.width / 2) + 1;  //get left position
	int x = ball.xPosition, y = ball.yPosition;


	if ((y > p) && (y < q) && (x == r) && (ball.xDirection == RIGHT)) { //hit from left
		ball.xDirection = !(ball.xDirection);
		__HAL_TIM_SET_AUTORELOAD(&htim11, 1679);
		ballVelocityDelay = 0;
	}
	if ((y > p) && (y < q) && (x == s) && (ball.xDirection == LEFT)) { //hit from right
		ball.xDirection = !(ball.xDirection);
		__HAL_TIM_SET_AUTORELOAD(&htim11, 1679);
		ballVelocityDelay = 0;
	}
	if ((x > r) && (x < s) && (y == q) && (ball.yDirection == UP)) { //hit from bellow
		ball.yDirection = !(ball.yDirection);
		__HAL_TIM_SET_AUTORELOAD(&htim11, 1679);
		ballVelocityDelay = 0;
	}
	if ((x > r) && (x < s) && (y == p) && (ball.yDirection == DOWN)) { //hit from above
		ball.yDirection = !(ball.yDirection);
		__HAL_TIM_SET_AUTORELOAD(&htim11, 1679);
		ballVelocityDelay = 0;
	}

	//diagonals
	if ((x == r) && (y == q) && (ball.xDirection == RIGHT) && (ball.yDirection == UP)) { //bottom left
		ball.xDirection = !(ball.xDirection);
		ball.yDirection = !(ball.yDirection);
	}
	if ((x == r) && (y == p) && (ball.xDirection == RIGHT) && (ball.yDirection == DOWN)) { //top left
		ball.xDirection = !(ball.xDirection);
		ball.yDirection = !(ball.yDirection);
	}
	if ((x == s) && (y == q) && (ball.xDirection == LEFT) && (ball.yDirection == UP)) { //bottom right
		ball.xDirection = !(ball.xDirection);
		ball.yDirection = !(ball.yDirection);
	}
	if ((x == s) && (y == p) && (ball.xDirection == LEFT) && (ball.yDirection == DOWN)) { //top right
		ball.xDirection = !(ball.xDirection);
		ball.yDirection = !(ball.yDirection);
	}
}

void moveStick() {
	int x = - (Py[28] * 100); //get instant position and change to decimal
	int y = - (Px[28] * 100); // ||

	//sensibility configuration
	stick.xPosition = 10 + (x);
	stick.yPosition = (MAX_Y/2) + (y * 2);
}

void moveBall() {
	checkCollision();

	//change direction if it hits the corners of the display
	if (ball.xPosition >= MAX_X) ball.xDirection = LEFT;
	if (ball.xPosition <= MIN_X) ball.xDirection = RIGHT;
	if (ball.yPosition >= MAX_Y) ball.yDirection = UP;
	if (ball.yPosition <= MIN_Y) ball.yDirection = DOWN;

	//increase x position moving right, decrease moving left
	if (!ball.xDirection) ball.xPosition = ++ball.xPosition;
	else ball.xPosition = --ball.xPosition;

	//increase y position moving down, decrease moving up 
	if (!ball.yDirection) ball.yPosition = ++ball.yPosition;
	else ball.yPosition = --ball.yPosition;
}

void drawBall() {
	lcd5110_box(ball.xPosition, ball.yPosition, ball.width, ball.height);
}

void drawStick() {
	lcd5110_box(stick.xPosition, stick.yPosition, stick.width, stick.height);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
