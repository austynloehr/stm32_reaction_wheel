/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum AFS_SEL {
	g2 = 	0x00, // +-2g
	g4 = 	0x01, // +-4g
	g8 = 	0x02, // +-8g
	g16 = 	0x03, // +-16g
};

enum FS_SEL {
	w250 = 		0x00, // +-250 deg/s
	w500 = 		0x01, // +-500 deg/s
	w1000 = 	0x02, // +-1000 deg/s
	w2000 = 	0x03, // +-2000 deg/s
};

typedef struct MPU6050_RawIMUDataSet {
	int16_t 	XOUT_na;
	int16_t  	YOUT_na;
	int16_t 	ZOUT_na;
} MPU6050_RawIMUDataSet_t;

typedef struct MPU6050_AccelDataSet {
	float 		XOUT_g;
	float 		YOUT_g;
	float		ZOUT_g;
} MPU6050_AccelDataSet_t;

typedef struct MPU6050_GyroDataSet {
	float 		XOUT_dps;
	float 		YOUT_dps;
	float		ZOUT_dps;
} MPU6050_GyroDataSet_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Device Addresses */
#define MPU6050_ADDR1 0xD0
#define MPU6050_ADDR2 0xD2
/* Memory registers */
#define PWR_MGMT1_REG 0x6B
#define SMPRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define GYRO_OUT_START_REG 0x43
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_OUT_START_REG 0x3B
#define CONFIG_REG 0x1A
/* I2C Data */
#define REG_LEN 0x01
#define I2C_TIMEOUT 50
#define CLKSEL 0x00
#define SMPLRT_DIV 0x01 // Sample Rate [kHz] = 8 / (1 + SMPLRT_DIV)
#define DLPF_CFG 0x04
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
const enum AFS_SEL accelRange = g4;
const enum FS_SEL gyroRange = w500;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void MPU6050_Init(void);

float getAccelSens(const enum AFS_SEL accelRange);
float getGyroSens(const enum FS_SEL gyroRange);
void HC05_PrintIMUData(const MPU6050_AccelDataSet_t Accel1, const MPU6050_AccelDataSet_t Accel2, const MPU6050_GyroDataSet_t Gyro1, const MPU6050_GyroDataSet_t Gyro2);

MPU6050_AccelDataSet_t MPU6050_GetAccel(const uint8_t deviceAddr);
MPU6050_GyroDataSet_t MPU6050_GetGyro(const uint8_t deviceAddr);
MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(const uint8_t deviceAddr);
MPU6050_RawIMUDataSet_t MPU6050_GetRawGyro(const uint8_t deviceAddr);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  MPU6050_AccelDataSet_t Accel1;
  MPU6050_AccelDataSet_t Accel2;
  MPU6050_GyroDataSet_t Gyro1;
  MPU6050_GyroDataSet_t Gyro2;

  MPU6050_Init();
  HAL_Delay(10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Accel1 = MPU6050_GetAccel(MPU6050_ADDR1);
	  Gyro1 = MPU6050_GetGyro(MPU6050_ADDR1);

	  Accel2 = MPU6050_GetAccel(MPU6050_ADDR2);
	  Gyro2 = MPU6050_GetGyro(MPU6050_ADDR2);

	  HC05_PrintIMUData(Accel1, Accel2, Gyro1, Gyro2);
	  HAL_Delay(10);

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void MPU6050_Init(void){
	uint8_t const msgLen = 30;
	uint8_t msg[30];
	uint8_t regData;
	uint8_t numReg = 1;

	/* I2C device check */
	HAL_StatusTypeDef deviceCheck1 = HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_ADDR1, 1, I2C_TIMEOUT);
	HAL_StatusTypeDef deviceCheck2 = HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_ADDR2, 1, I2C_TIMEOUT);

	/* Clear string */
	memset(msg, '\0', msgLen);

	/* Check status */
	if((deviceCheck1 == HAL_OK) && (deviceCheck2== HAL_OK)){
		strcpy((char*) msg, "Devices Ready\n\r");
	} else {
		strcpy((char*) msg, "Devices Not Ready\n\r");
	}

	/* Report status */
	HAL_UART_Transmit(&huart2, msg, sizeof(msg), 50);

	/* Wake IMUs */
	regData = CLKSEL;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR1, PWR_MGMT1_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR2, PWR_MGMT1_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Calculate SMPLRT_DIV from desired sample rate */
	regData = SMPLRT_DIV;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR1, SMPRT_DIV_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR2, SMPRT_DIV_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Set gyro config */
	regData = (uint8_t) (gyroRange << 3);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR1, GYRO_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR2, GYRO_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Set accel config */
	regData = (uint8_t) (accelRange << 3);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR1, ACCEL_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR2, ACCEL_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	regData = (uint8_t) (DLPF_CFG);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR2, CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
}

float getAccelSens(const enum AFS_SEL accelRange){
	float sensitivity;

	/* Determine sensor sensitivity based on sensor range */
	switch (accelRange){
	case g2:
		sensitivity = 16384.0;
		break;
	case g4:
		sensitivity = 8192.0;
		break;
	case g8:
		sensitivity = 4096.0;
		break;
	case g16:
		sensitivity = 2048.0;
		break;
	default:
		sensitivity = 0; // Create error code here later
	}

	return sensitivity;
}

float getGyroSens(const enum FS_SEL gyroRange){
	float sensitivity;

	/* Determine sensor sensitivity based on sensor range */
	switch (gyroRange){
	case w250:
		sensitivity = 131.0;
		break;
	case w500:
		sensitivity = 65.5;
		break;
	case w1000:
		sensitivity = 32.8;
		break;
	case w2000:
		sensitivity = 16.4;
		break;
	default:
		sensitivity = 0; // Create error code here later
	}

	return sensitivity;
}

MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(const uint8_t deviceAddr){
	uint8_t	numReg = 6;
	uint8_t regData[numReg];
	MPU6050_RawIMUDataSet_t RawAccel;

	/* Read accel measurement registers and store in regData array */
	HAL_I2C_Mem_Read(&hi2c1, deviceAddr, ACCEL_OUT_START_REG, REG_LEN, (uint8_t*) &regData, numReg, I2C_TIMEOUT);

	/* Combine high and low bits to 16 bit integer, update struct contents*/
	RawAccel.XOUT_na = (int16_t) ((regData[0] << 8) | (regData[1]));
	RawAccel.YOUT_na = (int16_t) ((regData[2] << 8) | (regData[3]));
	RawAccel.ZOUT_na = (int16_t) ((regData[4] << 8) | (regData[5]));

	/* Return RawAccel struct*/
	return RawAccel;
}

MPU6050_RawIMUDataSet_t MPU6050_GetRawGyro(const uint8_t deviceAddr){
	uint8_t	numReg = 6;
	uint8_t regData[numReg];
	MPU6050_RawIMUDataSet_t RawGyro;

	/* Read gyro measurement registers and store in regData array */
	HAL_I2C_Mem_Read(&hi2c1, deviceAddr, GYRO_OUT_START_REG, REG_LEN, (uint8_t*) &regData, numReg, I2C_TIMEOUT);

	/* Combine high and low bits to 16 bit integer, update struct contents*/
	RawGyro.XOUT_na = (int16_t) ((regData[0] << 8) | (regData[1]));
	RawGyro.YOUT_na = (int16_t) ((regData[2] << 8) | (regData[3]));
	RawGyro.ZOUT_na = (int16_t) ((regData[4] << 8) | (regData[5]));

	/* Return RawGyro struct*/
	return RawGyro;
}

MPU6050_AccelDataSet_t MPU6050_GetAccel(const uint8_t deviceAddr){
	MPU6050_RawIMUDataSet_t RawAccel;
	MPU6050_AccelDataSet_t Accel;
	float sensitivity;

	sensitivity = getAccelSens(accelRange);
	RawAccel = MPU6050_GetRawAccel(deviceAddr);

	/* Convert bits to g's, store in Accel struct*/
	Accel.XOUT_g = RawAccel.XOUT_na / sensitivity;
	Accel.YOUT_g = RawAccel.YOUT_na / sensitivity;
	Accel.ZOUT_g = RawAccel.ZOUT_na / sensitivity;

	/* Return Accel struct */
	return Accel;
}

MPU6050_GyroDataSet_t MPU6050_GetGyro(const uint8_t deviceAddr){
	MPU6050_RawIMUDataSet_t RawGyro;
	MPU6050_GyroDataSet_t Gyro;
	float sensitivity;

	sensitivity = getGyroSens(gyroRange);
	RawGyro = MPU6050_GetRawGyro(deviceAddr);

	/* Convert bits to deg/s, store in Gyro struct*/
	Gyro.XOUT_dps = RawGyro.XOUT_na / sensitivity;
	Gyro.YOUT_dps = RawGyro.YOUT_na / sensitivity;
	Gyro.ZOUT_dps = RawGyro.ZOUT_na / sensitivity;

	/* Return Gyro struct */
	return Gyro;
}

void HC05_PrintIMUData(const MPU6050_AccelDataSet_t Accel1, const MPU6050_AccelDataSet_t Accel2, const MPU6050_GyroDataSet_t Gyro1, const MPU6050_GyroDataSet_t Gyro2){
	uint8_t const msgLen = 140;
	uint8_t msg[msgLen];

	memset(msg, '\0', msgLen);
	sprintf((char*) msg, "AX1:%.3f,AY1:%.3f,AZ1:%.3f,WX1:%.3f,WY1:%.3f,WZ1:%.3f,AX2:%.3f,AY2:%.3f,AZ2:%.3f,WX2:%.3f,WY2:%.3f,WZ2:%.3f\n\r",
			Accel1.XOUT_g, Accel1.YOUT_g, Accel1.ZOUT_g, Gyro1.XOUT_dps, Gyro1.YOUT_dps, Gyro1.ZOUT_dps,
			Accel2.XOUT_g, Accel2.YOUT_g, Accel2.ZOUT_g, Gyro2.XOUT_dps, Gyro2.YOUT_dps, Gyro2.ZOUT_dps);

	HAL_UART_Transmit(&huart2, msg, msgLen, 50);
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
