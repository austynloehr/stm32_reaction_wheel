/*
 * IO_MPU6050.h
 *
 *  Created on: Jul. 7, 2023
 *      Author: aloehr
 */

#ifndef INC_IO_IO_MPU6050_H_
#define INC_IO_IO_MPU6050_H_

#include <stdint.h>
#include "main.h"

/* Start Struct definitions */
typedef struct MPU6050_RawIMUDataSet {
	int16_t 	XOUT_na;
	int16_t  	YOUT_na;
	int16_t 	ZOUT_na;
} MPU6050_RawIMUDataSet_t;

typedef struct HI_MPU6050_Bus {
	MPU6050_RawIMUDataSet_t 	RawAccel;
	MPU6050_RawIMUDataSet_t 	RawGyro;
} HI_MPU6050_Bus_t;
/* End Struct definitions */

/* End Function Prototypes */
HI_MPU6050_Bus_t HI_MPU6050(I2C_HandleTypeDef *hi2c);
/* End Function Prototypes */

#endif /* INC_IO_IO_MPU6050_H_ */
