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

/* Start Enum definitions */
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
/* End Enum definitions */

/* Start Struct definitions */
typedef struct MPU6050_RawIMUDataSet {
	int16_t 	XOUT_na;
	int16_t  	YOUT_na;
	int16_t 	ZOUT_na;
} MPU6050_RawIMUDataSet_t;

typedef struct HI_MPU6050_Bus {
	MPU6050_RawIMUDataSet_t 	RawAccel;
	MPU6050_RawIMUDataSet_t 	RawGyro;
	AFS_SEL_enum_t 				accelRange;
	FS_SEL_enum_t				gyroRange;
} HI_MPU6050_Bus_t;
/* End Struct definitions */

/* End Function Prototypes */
HI_MPU6050_Bus_t HI_MPU6050(I2C_HandleTypeDef hi2c1);
void Config_MPU6050(I2C_HandleTypeDef hi2c1);
/* End Function Prototypes */

#endif /* INC_IO_IO_MPU6050_H_ */
