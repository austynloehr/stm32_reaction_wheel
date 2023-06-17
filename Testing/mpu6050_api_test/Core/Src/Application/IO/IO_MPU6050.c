/*
 * HI_MPU6050.c
 *
 *  Created on: Jul. 7, 2023
 *      Author: aloehr
 */
#include "IO_MPU6050.h"

/* Start Defines */
/* Memory Addresses */
#define ACCEL_OUT_START_REG 0x3B
#define GYRO_OUT_START_REG 	0x43
#define ACCEL_CONFIG_REG 	0x1C
#define GYRO_CONFIG_REG 	0x1B
#define PWR_MGMT1_REG 		0x6B
#define SMPRT_DIV_REG 		0x19
#define MPU6050_ADDR 		0xD0
#define CONFIG_REG 			0x1A

/* Register Data */
#define CLKSEL 				0x00
#define SMPLRT_DIV 			0x07 // Sample Rate [kHz] = 8 / (1 + SMPLRT_DIV)
#define DLPF_CFG 			0x04

/* I2C Parameters */
#define I2C_TIMEOUT 		50 // ms
#define REG_LEN 			0x01 // bytes
/* End Defines */

/* Start global variable definitions */
const AFS_SEL_enum_t accelRange = g4;
const FS_SEL_enum_t gyroRange = w500;
/* End global variable definitions */

/* Start function prototypes */
static MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(I2C_HandleTypeDef hi2c1);
static MPU6050_RawIMUDataSet_t MPU6050_GetRawGyro(I2C_HandleTypeDef hi2c1);
static void MPU6050_Init(I2C_HandleTypeDef hi2c1);
/* End function prototypes */

/* Start global function definitions */
HI_MPU6050_Bus_t HI_MPU6050(I2C_HandleTypeDef hi2c1){
	static HI_MPU6050_Bus_t HI_MPU6050_Bus;

	HI_MPU6050_Bus.RawAccel = MPU6050_GetRawAccel(hi2c1);
	HI_MPU6050_Bus.RawGyro = MPU6050_GetRawGyro(hi2c1);
	HI_MPU6050_Bus.accelRange = accelRange;
	HI_MPU6050_Bus.gyroRange = gyroRange;

	return HI_MPU6050_Bus;
}

void Config_MPU6050(I2C_HandleTypeDef hi2c1){
	MPU6050_Init(hi2c1);
	HAL_Delay(50);
}

/* End global function definitions */

/* Start static function definitions */
static MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(I2C_HandleTypeDef hi2c1){
	static MPU6050_RawIMUDataSet_t RawAccel;
	uint8_t	numReg = 6;
	uint8_t regData[numReg];

	/* Read accel measurement registers and store in regData array */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, ACCEL_OUT_START_REG, REG_LEN, (uint8_t*) &regData, numReg, I2C_TIMEOUT);

	/* Combine high and low bits to 16 bit integer, update struct contents*/
	RawAccel.XOUT_na = (int16_t) ((regData[0] << 8) | (regData[1]));
	RawAccel.YOUT_na = (int16_t) ((regData[2] << 8) | (regData[3]));
	RawAccel.ZOUT_na = (int16_t) ((regData[4] << 8) | (regData[5]));

	/* Return RawAccel struct*/
	return RawAccel;
}

static MPU6050_RawIMUDataSet_t MPU6050_GetRawGyro(I2C_HandleTypeDef hi2c1){
	static MPU6050_RawIMUDataSet_t RawGyro;
	uint8_t	numReg = 6;
	uint8_t regData[numReg];

	/* Read gyro measurement registers and store in regData array */
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, GYRO_OUT_START_REG, REG_LEN, (uint8_t*) &regData, numReg, I2C_TIMEOUT);

	/* Combine high and low bits to 16 bit integer, update struct contents*/
	RawGyro.XOUT_na = (int16_t) ((regData[0] << 8) | (regData[1]));
	RawGyro.YOUT_na = (int16_t) ((regData[2] << 8) | (regData[3]));
	RawGyro.ZOUT_na = (int16_t) ((regData[4] << 8) | (regData[5]));

	/* Return RawGyro struct*/
	return RawGyro;
}

static void MPU6050_Init(I2C_HandleTypeDef hi2c1){
	static uint8_t regData;
	static uint8_t numReg = 1;

	/* I2C device check */
	// Raise error later
	HAL_StatusTypeDef deviceCheck = HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_ADDR, 1, I2C_TIMEOUT);

	if (deviceCheck != HAL_OK){
		Error_Handler();
	}

	/* Wake IMU */
	regData = CLKSEL;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWR_MGMT1_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Calculate SMPLRT_DIV from desired sample rate */
	regData = SMPLRT_DIV;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, SMPRT_DIV_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Set gyro config */
	regData = (uint8_t) (gyroRange << 3);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/* Set accel config */
	regData = (uint8_t) (accelRange << 3);
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACCEL_CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);

	/*Configure low pass filter*/
	regData = DLPF_CFG;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, CONFIG_REG, REG_LEN, &regData, numReg, I2C_TIMEOUT);
}
/* End static function definitions */
