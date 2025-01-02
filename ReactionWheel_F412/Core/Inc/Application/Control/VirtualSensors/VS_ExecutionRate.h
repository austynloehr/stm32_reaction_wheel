/*
 * VS_ExecutionRate.h
 *
 *  Created on: May 23, 2024
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_EXECUTIONRATE_H_
#define INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_EXECUTIONRATE_H_

#include "main.h"

/* Start Global Struct definitions */
typedef struct VS_ExecutionRate_Bus {
	float dt; //s
} VS_ExecutionRate_Bus_t;
/* End Global Struct definitions */

/* Start Function Prototypes */
VS_ExecutionRate_Bus_t VS_ExecutionRate(void);
/* End Function Prototypes */

#endif /* INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_EXECUTIONRATE_H_ */
