/*
 * PR_MPU6050.c
 *
 *  Created on: Jul 7, 2023
 *      Author: aloehr
 */
#include <math.h>
#include "PR_MPU6050.h"
#include "Config_DSP.h"

/* Start local enum definitions */
typedef struct IMUOffsetData{
	float AxOffset;
	float AyOffset;
	float AzOffset;
	float WxOffset;
	float WyOffset;
	float WzOffset;
}IMUOffsetData_t;
/* End local enum definitions */

/* Start Global Variable Definition */
const float g = 9.8066;
static IMUOffsetData_t IMUOffsets = {-0.203655735,0.0263493527,0.189724252, 0.399456531,-1.28966808,-0.282766312}; // Initial values
static IMUOffsetData_t *const pIMUOffsets = &IMUOffsets;
/* End Global Variable Definition */

/* Start Function Prototypes */
static float clamp(float data, float clampVal, float threshold, uint8_t mode);
static IP_MPU6050_Bus_t MPU6050_IMUDataClamp(IP_MPU6050_Bus_t MPU6050_Bus);
static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel, float accelSens, IMUOffsetData_t *const pIMUOffsetData);
static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro, float gyroSens, IMUOffsetData_t *const pIMUOffsetData);
static void MPU6050_CalibrateOffsets(I2C_HandleTypeDef *hi2c, float accelSens, float gyroSens, IMUOffsetData_t *const pGlobalIMUOffsets);
/* End Function Prototypes */

/* Start Global Function Definition */
IP_MPU6050_Bus_t IP_MPU6050(HI_MPU6050_Bus_t HI_MPU6050_Bus, Config_MPU6050_Bus_t Config_MPU6050_Bus, I2C_HandleTypeDef *hi2c, PrimaryState CurrentState_enum){
	IP_MPU6050_Bus_t IP_MPU6050_Bus;

	IP_MPU6050_Bus.IMUCalComplete = false;
	if (CurrentState_enum == CalibrateIMU){
		MPU6050_CalibrateOffsets(hi2c,Config_MPU6050_Bus.Sensitivity.AccelSens,Config_MPU6050_Bus.Sensitivity.GyroSens,pIMUOffsets);
		IP_MPU6050_Bus.IMUCalComplete = true;
	}

	IP_MPU6050_Bus.accel = MPU6050_CalcAccel(HI_MPU6050_Bus.RawAccel, Config_MPU6050_Bus.Sensitivity.AccelSens, pIMUOffsets);
	IP_MPU6050_Bus.gyro = MPU6050_CalcGyro(HI_MPU6050_Bus.RawGyro, Config_MPU6050_Bus.Sensitivity.GyroSens, pIMUOffsets);

	// Filter accel data
	arm_biquad_cascade_df1_f32(&S_Ax, &IP_MPU6050_Bus.accel.XOUT_ms2, &IP_MPU6050_Bus.AxFilt_mps2, blockSize);
	arm_biquad_cascade_df1_f32(&S_Ay, &IP_MPU6050_Bus.accel.YOUT_ms2, &IP_MPU6050_Bus.AyFilt_mps2, blockSize);

	return IP_MPU6050_Bus;
}
/* End Global Function Definition */

/* Start Static Function Definition */
static MPU6050_AccelData_t MPU6050_CalcAccel(MPU6050_RawIMUDataSet_t RawAccel, float accelSens, IMUOffsetData_t *const pIMUOffsetData){
	MPU6050_AccelData_t Accel;

	/* Convert bits to g's, store in Accel struct*/
	Accel.XOUT_ms2 = (RawAccel.XOUT_na / accelSens) * g + pIMUOffsetData->AxOffset;
	Accel.YOUT_ms2 = (RawAccel.YOUT_na / accelSens) * g + pIMUOffsetData->AyOffset;
	Accel.ZOUT_ms2 = (RawAccel.ZOUT_na / accelSens) * g + pIMUOffsetData->AzOffset;

	/* Return Accel struct */
	return Accel;
}

static MPU6050_GyroData_t MPU6050_CalcGyro(MPU6050_RawIMUDataSet_t RawGyro, float gyroSens, IMUOffsetData_t *const pIMUOffsetData){
	MPU6050_GyroData_t Gyro;

	/* Convert bits to deg/s, store in Gyro struct*/
	Gyro.XOUT_dps = RawGyro.XOUT_na / gyroSens + pIMUOffsetData->WxOffset;
	Gyro.YOUT_dps = RawGyro.YOUT_na / gyroSens + pIMUOffsetData->WyOffset;
	Gyro.ZOUT_dps = RawGyro.ZOUT_na / gyroSens + pIMUOffsetData->WzOffset;

	/* Return Gyro struct */
	return Gyro;
}

static void MPU6050_CalibrateOffsets(I2C_HandleTypeDef *hi2c, float accelSens, float gyroSens, IMUOffsetData_t *const pGlobalIMUOffsets){
	HI_MPU6050_Bus_t HI_MPU6050_Bus;
	MPU6050_AccelData_t AccelData;
	MPU6050_GyroData_t GyroData;

	IMUOffsetData_t IMUOffsetData = {0,0,0,0,0,0};
	IMUOffsetData_t *const pIMUOffsetData = &IMUOffsetData;
	uint32_t numSamples = 50000;
	int8_t sign;

	float sumAx = 0;
	float sumAy = 0;
	float sumAz = 0;
	float sumWx = 0;
	float sumWy = 0;
	float sumWz = 0;

	for(uint32_t i = 0; i < numSamples; i++){
		HI_MPU6050_Bus = HI_MPU6050(hi2c);
		AccelData = MPU6050_CalcAccel(HI_MPU6050_Bus.RawAccel, accelSens, pIMUOffsetData);
		GyroData = MPU6050_CalcGyro(HI_MPU6050_Bus.RawGyro, gyroSens, pIMUOffsetData);

		sumAx += AccelData.XOUT_ms2;
		sumAy += AccelData.YOUT_ms2;
		sumAz += AccelData.ZOUT_ms2;

		sumWx += GyroData.XOUT_dps;
		sumWy += GyroData.YOUT_dps;
		sumWz += GyroData.ZOUT_dps;

		HAL_Delay(1);
	}

	// Allow for calibration at -45 or 45 deg
	if (AccelData.XOUT_ms2 < 0){
		sign = -1;
	} else{
		sign = 1;
	}

	pGlobalIMUOffsets->AxOffset = sign * (9.80665 * cos(45 * PI / 180)) - (sumAx / numSamples);
	pGlobalIMUOffsets->AyOffset = (9.80665 * sin(45 * PI / 180)) - (sumAy / numSamples);
	pGlobalIMUOffsets->AzOffset = 0 - (sumAz / numSamples);

	pGlobalIMUOffsets->WxOffset = 0 - (sumWx / numSamples);
	pGlobalIMUOffsets->WyOffset = 0 - (sumWy / numSamples);
	pGlobalIMUOffsets->WzOffset = 0 - (sumWz / numSamples);
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
	IP_MPU6050_Bus_t Result_Bus = MPU6050_Bus;

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
