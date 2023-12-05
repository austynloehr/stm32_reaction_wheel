/*
 * Config_MPU6050.c
 *
 *  Created on: Jul. 12, 2023
 *      Author: aloehr
 */

#include "Config_MPU6050.h"
#include "IO_MPU6050.h"
#include "PR_MPU6050.h"
#include <math.h>

/* Start Defines */
/* Memory Addresses */
#define ACCEL_CONFIG_REG 	0x1C
#define GYRO_CONFIG_REG 	0x1B
#define PWR_MGMT1_REG 		0x6B
#define SMPRT_DIV_REG 		0x19
#define CONFIG_REG 			0x1A

/* Register Data */
#define CLKSEL 				0x00
#define SMPLRT_DIV 			0x07 // Sample Rate [kHz] = 8 / (1 + SMPLRT_DIV)
#define DLPF_CFG 			0x00
/* End Defines */

/* Start local enum definitions */
typedef enum AFS_SEL_enum {
	g2 = 	0x00, // +-2g
	g4 = 	0x01, // +-4g
	g8 = 	0x02, // +-8g
	g16 = 	0x03, // +-16g
} AFS_SEL_enum_t;

typedef enum FS_SEL_enum {
	w250 = 		0x00, // +-250 deg/s
	w500 = 		0x01, // +-500 deg/s
	w1000 = 	0x02, // +-1000 deg/s
	w2000 = 	0x03, // +-2000 deg/s
} FS_SEL_enum_t;
/* End local enum definitions */

/* Start global variable definitions */
const AFS_SEL_enum_t accelRange = g4;
const FS_SEL_enum_t gyroRange = w500;
/* End global variable definitions */

/* Start static function prototypes */
static void MPU6050_Init(I2C_HandleTypeDef hi2c1);
static MPU6050_IMUSensitivityData_t MPU6050_GetSens(const AFS_SEL_enum_t accelRange, const FS_SEL_enum_t gyroRange);
static MPU6050_IMUOffsetData_t MPU6050_CalibrateOffsets(I2C_HandleTypeDef hi2c1, Config_MPU6050_Bus_t Config_MPU6050_Bus);
/* Start static function prototypes */

/* Start global function definitions */
Config_MPU6050_Bus_t Config_MPU6050(I2C_HandleTypeDef hi2c1){
	Config_MPU6050_Bus_t Config_MPU6050_Bus;

	MPU6050_Init(hi2c1);

	Config_MPU6050_Bus.Sensitivity = MPU6050_GetSens(accelRange, gyroRange);

	Config_MPU6050_Bus.Offsets = (MPU6050_IMUOffsetData_t) {0, 0, 0, 0, 0, 0, 0};
	Config_MPU6050_Bus.Offsets = MPU6050_CalibrateOffsets(hi2c1, Config_MPU6050_Bus);

	return Config_MPU6050_Bus;
}
/* End global function definitions */

/* Start static function definitions */
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

static MPU6050_IMUSensitivityData_t MPU6050_GetSens(const AFS_SEL_enum_t accelRange, const FS_SEL_enum_t gyroRange){
	MPU6050_IMUSensitivityData_t IMUSensitivity;

	/* Determine sensor sensitivity based on sensor range */
	switch (accelRange){
	case g2:
		IMUSensitivity.AccelSens = 16384.0;
		break;
	case g4:
		IMUSensitivity.AccelSens = 8192.0;
		break;
	case g8:
		IMUSensitivity.AccelSens = 4096.0;
		break;
	case g16:
		IMUSensitivity.AccelSens = 2048.0;
		break;
	default:
		IMUSensitivity.AccelSens = 0;
		Error_Handler();
	}

	/* Determine sensor sensitivity based on sensor range */
	switch (gyroRange){
	case w250:
		IMUSensitivity.GyroSens = 131.0;
		break;
	case w500:
		IMUSensitivity.GyroSens = 65.5;
		break;
	case w1000:
		IMUSensitivity.GyroSens = 32.8;
		break;
	case w2000:
		IMUSensitivity.GyroSens = 16.4;
		break;
	default:
		IMUSensitivity.GyroSens = 0;
		Error_Handler();
	}

	return IMUSensitivity;
}

static MPU6050_IMUOffsetData_t MPU6050_CalibrateOffsets(I2C_HandleTypeDef hi2c1, Config_MPU6050_Bus_t Config_MPU6050_Bus){
	MPU6050_IMUOffsetData_t IMUOffsets;
	HI_MPU6050_Bus_t HI_MPU6050_Bus;
	IP_MPU6050_Bus_t IP_MPU6050_Bus;

	uint32_t numSamples = 2000;

	float sumAx = 0;
	float sumAy = 0;
	float sumAz = 0;
	float sumWx = 0;
	float sumWy = 0;
	float sumWz = 0;

	for(uint32_t i = 0; i < numSamples; i++){
		HI_MPU6050_Bus = HI_MPU6050(hi2c1);
		IP_MPU6050_Bus = IP_MPU6050(HI_MPU6050_Bus, Config_MPU6050_Bus);

		sumAx += IP_MPU6050_Bus.accel.XOUT_ms2;
		sumAy += IP_MPU6050_Bus.accel.YOUT_ms2;
		sumAz += IP_MPU6050_Bus.accel.ZOUT_ms2;

		sumWx += IP_MPU6050_Bus.gyro.XOUT_dps;
		sumWy += IP_MPU6050_Bus.gyro.YOUT_dps;
		sumWz += IP_MPU6050_Bus.gyro.ZOUT_dps;

		HAL_Delay(5);
	}

	IMUOffsets.AxOffset = 0 - (sumAx / numSamples);
	IMUOffsets.AyOffset = (9.80665 * sin(0 * PI / 180)) - (sumAy / numSamples);
	IMUOffsets.AzOffset = (9.80665 * cos(0 * PI / 180)) - (sumAz / numSamples);

	IMUOffsets.WxOffset = 0 - (sumWx / numSamples);
	IMUOffsets.WyOffset = 0 - (sumWy / numSamples);
	IMUOffsets.WzOffset = 0 - (sumWz / numSamples);

	IMUOffsets.SensCalibrated_bool = 1;

	return IMUOffsets;
}
/* End static function definitions */
