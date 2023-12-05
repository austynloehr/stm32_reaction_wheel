/*
 * VS_OrientationEstimation.c
 *
 *  Created on: Jul. 20, 2023
 *      Author: aloehr
 */

#include "VS_OrientationEstimation.h"
#include "Config_DSP.h"
#include <math.h>

/* Start Global Variables */
static uint8_t resetGyroRoll_bool = 0;
static uint8_t resetGyroPitch_bool = 0;
static uint32_t resetAccelRollCount = 0;
static uint32_t resetAccelPitchCount = 0;
static uint32_t lastTick = 0;
static float lastRollAng_deg = 0;
static float lastPitchAng_deg = 0;
/* Start Global Variables */

/* Start Static Function Prototypes */
static int8_t signf(float x);
static void ResetGyroIntegrators(float accelRoll_deg, float accelPitch_deg);
static VS_OrientationData_t CalcGyroAngle(MPU6050_GyroData_t MPU6050_GyroData, VS_OrientationData_t FiltAccelOrientation);
static VS_OrientationData_t CalcAccelAngle(MPU6050_AccelData_t MPU6050_AccelData);
static VS_OrientationData_t AccelLowPassFilt(VS_OrientationData_t AccelOrientation);
static VS_OrientationData_t ComplimentaryFilter(VS_OrientationData_t GyroOrientation, VS_OrientationData_t FiltAccelOrientation);
/* End Static Function Prototypes */

/* Start Global Function Definitions */
VS_Orientation_Bus_t EstimateOrientation(IP_MPU6050_Bus_t IP_MPU6050_Bus){
	VS_Orientation_Bus_t VS_Orientation_Bus;

	VS_Orientation_Bus.AccelOrientation = CalcAccelAngle(IP_MPU6050_Bus.accel);

	ResetGyroIntegrators(VS_Orientation_Bus.AccelOrientation.roll_deg, VS_Orientation_Bus.AccelOrientation.pitch_deg);

	VS_Orientation_Bus.FiltAccelOrientation = AccelLowPassFilt(VS_Orientation_Bus.AccelOrientation);

	VS_Orientation_Bus.GyroOrientation = CalcGyroAngle(IP_MPU6050_Bus.gyro, VS_Orientation_Bus.FiltAccelOrientation);

	VS_Orientation_Bus.CompFiltOrientation = ComplimentaryFilter(VS_Orientation_Bus.GyroOrientation, VS_Orientation_Bus.FiltAccelOrientation);

	return VS_Orientation_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static VS_OrientationData_t CalcAccelAngle(MPU6050_AccelData_t MPU6050_AccelData){
	VS_OrientationData_t AccelOrientation;

	float roll_rad = atan2f(MPU6050_AccelData.YOUT_ms2, MPU6050_AccelData.ZOUT_ms2);
	float pitch_rad = atan2f(-1 * MPU6050_AccelData.XOUT_ms2, sqrtf(powf(MPU6050_AccelData.YOUT_ms2, 2) + powf(MPU6050_AccelData.ZOUT_ms2, 2)));

	AccelOrientation.roll_deg = roll_rad * 180 / M_PI;
	AccelOrientation.pitch_deg = pitch_rad * 180 / M_PI;
	AccelOrientation.yaw_deg = 0;  // No accurate way to calculate this

	return AccelOrientation;
}

static VS_OrientationData_t CalcGyroAngle(MPU6050_GyroData_t MPU6050_GyroData, VS_OrientationData_t FiltAccelOrientation){
	VS_OrientationData_t GyroOrientation;
	uint32_t currentTick = HAL_GetTick();
	float dt = ((float) currentTick - (float) lastTick) / 1000;

	// Integrator reset logic
	if(resetGyroRoll_bool){
		lastRollAng_deg = FiltAccelOrientation.roll_deg;
		resetGyroRoll_bool = 0;
	}

	if(resetGyroPitch_bool){
		lastPitchAng_deg = 0;
		resetGyroPitch_bool = 0;
	}

	// Integrate angular velocities
	GyroOrientation.roll_deg = lastRollAng_deg + MPU6050_GyroData.XOUT_dps * dt;
	GyroOrientation.pitch_deg = lastPitchAng_deg + MPU6050_GyroData.YOUT_dps * dt;
	GyroOrientation.yaw_deg = 0;  // No accurate way to calculate this

	// Store previous values
	lastTick = currentTick;
	lastRollAng_deg = GyroOrientation.roll_deg;
	lastPitchAng_deg = GyroOrientation.pitch_deg;

	return GyroOrientation;
}

static void ResetGyroIntegrators(float accelRoll_deg, float accelPitch_deg){
	// Reset gyro integrators if accel values are near 45 for X consecutive cycles

	float resetAccelTolerance_deg = 1.0;
	float resetAccelRollAngle_deg = 0;
	float resetAccelPitchAngle_deg = 0;// Angle of system when integrator can be reset
	uint8_t resetAccelCntThreshold = 20;

	float zeroAccelRollUpperThreshold = resetAccelRollAngle_deg + resetAccelTolerance_deg;
	float zeroAccelRollLowerThreshold = resetAccelRollAngle_deg - resetAccelTolerance_deg;

	float zeroAccelPitchUpperThreshold = resetAccelPitchAngle_deg + resetAccelTolerance_deg;
	float zeroAccelPitchLowerThreshold = resetAccelPitchAngle_deg - resetAccelTolerance_deg;

	if (fabsf(accelRoll_deg) < zeroAccelRollUpperThreshold && fabsf(accelRoll_deg) > zeroAccelRollLowerThreshold){
		resetAccelRollCount++;
	}else{
		resetAccelRollCount = 0;
	}

	if (fabsf(accelPitch_deg) < zeroAccelPitchUpperThreshold && fabsf(accelPitch_deg) > zeroAccelPitchLowerThreshold){
		resetAccelPitchCount++;
	}else{
		resetAccelPitchCount = 0;
	}

	if (resetAccelRollCount == resetAccelCntThreshold){
		resetGyroRoll_bool = 1;
		resetAccelRollCount = 0;
	}else{
		resetGyroRoll_bool = 0;
	}

	if (resetAccelPitchCount == resetAccelCntThreshold){
		resetGyroPitch_bool = 1;
		resetAccelPitchCount = 0;
	}else{
		resetGyroPitch_bool = 0;
	}
}

static VS_OrientationData_t AccelLowPassFilt(VS_OrientationData_t AccelOrientation){
	VS_OrientationData_t FiltAccelOrientation;

	arm_biquad_cascade_df1_f32(&S_AccelRoll, &AccelOrientation.roll_deg, &FiltAccelOrientation.roll_deg, blockSize);
	arm_biquad_cascade_df1_f32(&S_AccelPitch, &AccelOrientation.pitch_deg, &FiltAccelOrientation.pitch_deg, blockSize);

	FiltAccelOrientation.yaw_deg = 0;

	return FiltAccelOrientation;
}

static VS_OrientationData_t ComplimentaryFilter(VS_OrientationData_t GyroOrientation, VS_OrientationData_t FiltAccelOrientation){
	VS_OrientationData_t CompFiltOrientation;

	float alpha = .90;

	CompFiltOrientation.roll_deg = GyroOrientation.roll_deg * alpha + FiltAccelOrientation.roll_deg * (1 - alpha);
	CompFiltOrientation.pitch_deg = GyroOrientation.pitch_deg * alpha + FiltAccelOrientation.pitch_deg * (1 - alpha);
	CompFiltOrientation.yaw_deg = 0;

	return CompFiltOrientation;
}

static int8_t signf(float x){
	int8_t sign;

	if (x > 0){
		sign = 1;
	}
	else if (x < 0) {
		sign = -1;
	}
	else {
		sign = 0;
	}

	return sign;
}
/* End Static Function Definitions */
