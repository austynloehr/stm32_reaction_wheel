/*
 * PR_MPU6050.c
 *
 *  Created on: Jul 7, 2023
 *      Author: aloehr
 */
#include "PR_MPU6050.h"

/* Start Global Variable Definition */
uint8_t calcSens_bool = 1;
const float g = 9.8066;
float accelSens;
float gyroSens;
/* End Global Variable Definition */

/* Start Function Prototypes */
static void GetSens(AFS_SEL_enum_t accelRange, FS_SEL_enum_t gyroRange);
static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel);
static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro);
/* End Function Prototypes */

/* Start Global Function Definition */
IP_MPU6050_Bus_t IP_MPU6050(HI_MPU6050_Bus_t HI_MPU6050_Bus){
	static IP_MPU6050_Bus_t IP_MPU6050_Bus;

	if (calcSens_bool){
		GetSens(HI_MPU6050_Bus.accelRange, HI_MPU6050_Bus.gyroRange);
	}

	IP_MPU6050_Bus.accel = MPU6050_CalcAccel(HI_MPU6050_Bus.RawAccel);
	IP_MPU6050_Bus.gyro = MPU6050_CalcGyro(HI_MPU6050_Bus.RawGyro);

	return IP_MPU6050_Bus;
}
/* End Global Function Definition */

/* Start Static Function Definition */
static void GetSens(const AFS_SEL_enum_t accelRange, const FS_SEL_enum_t gyroRange){

	/* Determine sensor sensitivity based on sensor range */
	switch (accelRange){
	case g2:
		accelSens = 16384.0;
		break;
	case g4:
		accelSens = 8192.0;
		break;
	case g8:
		accelSens = 4096.0;
		break;
	case g16:
		accelSens = 2048.0;
		break;
	default:
		accelSens = 0;
		Error_Handler();
	}

	/* Determine sensor sensitivity based on sensor range */
	switch (gyroRange){
	case w250:
		gyroSens = 131.0;
		break;
	case w500:
		gyroSens = 65.5;
		break;
	case w1000:
		gyroSens = 32.8;
		break;
	case w2000:
		gyroSens = 16.4;
		break;
	default:
		gyroSens = 0;
		Error_Handler();
	}

	calcSens_bool = 0;
}

static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel){
	static MPU6050_AccelData_t Accel;

	/* Convert bits to g's, store in Accel struct*/
	Accel.XOUT_ms2 = (RawAccel.XOUT_na / accelSens) * g;
	Accel.YOUT_ms2 = (RawAccel.YOUT_na / accelSens) * g;
	Accel.ZOUT_ms2 = (RawAccel.ZOUT_na / accelSens) * g;

	/* Return Accel struct */
	return Accel;
}

static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro){
	static MPU6050_GyroData_t Gyro;

	/* Convert bits to deg/s, store in Gyro struct*/
	Gyro.XOUT_dps = RawGyro.XOUT_na / gyroSens;
	Gyro.YOUT_dps = RawGyro.YOUT_na / gyroSens;
	Gyro.ZOUT_dps = RawGyro.ZOUT_na / gyroSens;

	/* Return Gyro struct */
	return Gyro;
}
/* End Static Function Definition */
