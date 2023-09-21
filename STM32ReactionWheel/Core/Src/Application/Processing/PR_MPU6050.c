/*
 * PR_MPU6050.c
 *
 *  Created on: Jul 7, 2023
 *      Author: aloehr
 */
#include <math.h>
#include "PR_MPU6050.h"

/* Start Global Variable Definition */
const float g = 9.8066;
/* End Global Variable Definition */

/* Start Function Prototypes */
static float clamp(float data, float clampVal, float threshold, uint8_t mode);
static IP_MPU6050_Bus_t MPU6050_IMUDataClamp(IP_MPU6050_Bus_t MPU6050_Bus);
static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel, float accelSens, MPU6050_IMUOffsetData_t IMUOffsets);
static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro, float gyroSens, MPU6050_IMUOffsetData_t IMUOffsets);
/* End Function Prototypes */

/* Start Global Function Definition */
IP_MPU6050_Bus_t IP_MPU6050(HI_MPU6050_Bus_t HI_MPU6050_Bus, Config_MPU6050_Bus_t Config_MPU6050_Bus){
	IP_MPU6050_Bus_t IP_MPU6050_Bus;

	IP_MPU6050_Bus.accel = MPU6050_CalcAccel(HI_MPU6050_Bus.RawAccel, Config_MPU6050_Bus.Sensitivity.AccelSens, Config_MPU6050_Bus.Offsets);
	IP_MPU6050_Bus.gyro = MPU6050_CalcGyro(HI_MPU6050_Bus.RawGyro, Config_MPU6050_Bus.Sensitivity.GyroSens, Config_MPU6050_Bus.Offsets);

	if(Config_MPU6050_Bus.Offsets.SensCalibrated_bool){
		IP_MPU6050_Bus = MPU6050_IMUDataClamp(IP_MPU6050_Bus);
	}

	return IP_MPU6050_Bus;
}
/* End Global Function Definition */

/* Start Static Function Definition */
static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel, float accelSens, MPU6050_IMUOffsetData_t IMUOffsets){
	MPU6050_AccelData_t Accel;

	/* Convert bits to g's, store in Accel struct*/
	Accel.XOUT_ms2 = (RawAccel.XOUT_na / accelSens) * g + IMUOffsets.AxOffset;
	Accel.YOUT_ms2 = (RawAccel.YOUT_na / accelSens) * g + IMUOffsets.AyOffset;
	Accel.ZOUT_ms2 = (RawAccel.ZOUT_na / accelSens) * g + IMUOffsets.AzOffset;

	/* Return Accel struct */
	return Accel;
}

static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro, float gyroSens, MPU6050_IMUOffsetData_t IMUOffsets){
	MPU6050_GyroData_t Gyro;

	/* Convert bits to deg/s, store in Gyro struct*/
	Gyro.XOUT_dps = RawGyro.XOUT_na / gyroSens + IMUOffsets.WxOffset;
	Gyro.YOUT_dps = RawGyro.YOUT_na / gyroSens + IMUOffsets.WyOffset;
	Gyro.ZOUT_dps = RawGyro.ZOUT_na / gyroSens + IMUOffsets.WzOffset;

	/* Return Gyro struct */
	return Gyro;
}

static float clamp(float data, float clampVal, float threshold, uint8_t mode){
	// mode = 0: Clamp to clampVal if data below threshold
	// mode = 1: Clamp to clampVal if data above threshold
	float result;

	uint8_t applyClamp_bool = 0;

	// Check mode and if clamp is required
	switch(mode){
	case 0:
		applyClamp_bool = (fabsf(data) < threshold);
		break;

	case 1:
		applyClamp_bool = (fabsf(data) > threshold);
		break;
	}

	// Apply clamp if above criteria are met
	if(applyClamp_bool){
		result = clampVal;
	} else{
		result = data;
	}

	return result;
}

static IP_MPU6050_Bus_t MPU6050_IMUDataClamp(IP_MPU6050_Bus_t MPU6050_Bus){
	IP_MPU6050_Bus_t Result_Bus;

	float clampVal = 0;
	float clampThreshold = .05;
	uint8_t clampMode = 0;

	Result_Bus.accel.XOUT_ms2 = clamp(MPU6050_Bus.accel.XOUT_ms2, clampVal, clampThreshold, clampMode);
	Result_Bus.accel.YOUT_ms2 = clamp(MPU6050_Bus.accel.YOUT_ms2, clampVal, clampThreshold, clampMode);
	Result_Bus.accel.ZOUT_ms2 = clamp(MPU6050_Bus.accel.ZOUT_ms2, clampVal, clampThreshold, clampMode);

	Result_Bus.gyro.XOUT_dps = clamp(MPU6050_Bus.gyro.XOUT_dps , clampVal, clampThreshold, clampMode);
	Result_Bus.gyro.YOUT_dps = clamp(MPU6050_Bus.gyro.YOUT_dps , clampVal, clampThreshold, clampMode);
	Result_Bus.gyro.ZOUT_dps = clamp(MPU6050_Bus.gyro.ZOUT_dps , clampVal, clampThreshold, clampMode);

	return Result_Bus;
}
/* End Static Function Definition */
