/*
 * IO.h
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_IO_IO_H_
#define INC_APPLICATION_IO_IO_H_

#include "main.h"
#include "Processing.h"

/* Input libraries */
#include "IO_MPU6050.h"
#include "IO_HC05.h"

/* Start global typedefs */
typedef struct HI_Bus{
	HI_MPU6050_Bus_t HI_MPU6050_Bus;
} HI_Bus_t;
/* End global typedefs */

/* Start global function prototypes */
HI_Bus_t IO_HardwareInputs(void);
void IO_HardwareOutputs(OP_Bus_t OP_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_IO_IO_H_ */
