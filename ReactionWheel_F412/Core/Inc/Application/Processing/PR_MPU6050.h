/*
 * PR_MPU6050.h
 *
 *  Created on: Jul 7, 2023
 *      Author: aloehr
 */

#ifndef INC_PROCESSING_PR_MPU6050_H_
#define INC_PROCESSING_PR_MPU6050_H_

#include "IO_MPU6050.h"
#include "Config_MPU6050.h"
#include "main.h"
#include "SimulinkEnums.h"

/* Start Global Struct definitions */
typedef struct MPU6050_GyroData {
	float 		XOUT_dps;
	float 		YOUT_dps;
	float		ZOUT_dps;
} MPU6050_GyroData_t;

typedef struct MPU6050_AccelData {
	float 		XOUT_ms2;
	float 		YOUT_ms2;
	float		ZOUT_ms2;
} MPU6050_AccelData_t;

typedef struct IP_MPU6050_Bus {
	MPU6050_AccelData_t accel;
	MPU6050_GyroData_t 	gyro;
	bool IMUCalComplete;
} IP_MPU6050_Bus_t;
/* End Global Struct definitions */

/* Start Function Prototypes */
IP_MPU6050_Bus_t IP_MPU6050(HI_MPU6050_Bus_t HI_MPU6050_Bus, Config_MPU6050_Bus_t Config_MPU6050_Bus, I2C_HandleTypeDef *hi2c, PrimaryState CurrentState_enum);
/* End Function Prototypes */

#endif /* INC_PROCESSING_PR_MPU6050_H_ */
