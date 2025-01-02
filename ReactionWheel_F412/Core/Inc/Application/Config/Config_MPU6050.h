/*
 * Config_MPU6050.h
 *
 *  Created on: Jul. 12, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONFIG_CONFIG_MPU6050_H_
#define INC_APPLICATION_CONFIG_CONFIG_MPU6050_H_

#include "main.h"

/* Start Global Defines */
/* Memory Addresses */
#define MPU6050_ADDR 		0xD0
/* I2C Parameters */
#define I2C_TIMEOUT 		50 // ms
#define REG_LEN 			0x01 // bytes
/* End Global Defines */

/* Start Global Typedefs */
typedef struct MPU6050_IMUSensitivityData{
	float AccelSens;
	float GyroSens;
}MPU6050_IMUSensitivityData_t;

typedef struct Config_MPU6050_Bus{
	MPU6050_IMUSensitivityData_t Sensitivity;
}Config_MPU6050_Bus_t;
/* End Global Typedefs */

/* Start Global Function Prototypes */
Config_MPU6050_Bus_t Config_MPU6050(I2C_HandleTypeDef *hi2c);
/* End Global Function Prototypes */

#endif /* INC_APPLICATION_CONFIG_CONFIG_MPU6050_H_ */
