/*
 * HI_MPU6050.c
 *
 *  Created on: Jul. 7, 2023
 *      Author: aloehr
 */
#include "IO_MPU6050.h"
#include "Config_MPU6050.h"

/* Start Defines */
/* Memory Addresses */
#define ACCEL_OUT_START_REG 0x3B
#define GYRO_OUT_START_REG 	0x43
/* End Defines */

/* Start function prototypes */
static MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(I2C_HandleTypeDef hi2c1);
static MPU6050_RawIMUDataSet_t MPU6050_GetRawGyro(I2C_HandleTypeDef hi2c1);
/* End function prototypes */

/* Start global function definitions */
HI_MPU6050_Bus_t HI_MPU6050(I2C_HandleTypeDef hi2c1){
	HI_MPU6050_Bus_t HI_MPU6050_Bus;

	HI_MPU6050_Bus.RawAccel = MPU6050_GetRawAccel(hi2c1);
	HI_MPU6050_Bus.RawGyro = MPU6050_GetRawGyro(hi2c1);

	return HI_MPU6050_Bus;
}
/* End global function definitions */

/* Start static function definitions */
static MPU6050_RawIMUDataSet_t MPU6050_GetRawAccel(I2C_HandleTypeDef hi2c1){
	MPU6050_RawIMUDataSet_t RawAccel;
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
	MPU6050_RawIMUDataSet_t RawGyro;
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
/* End static function definitions */
