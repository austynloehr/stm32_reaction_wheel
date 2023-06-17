/*
 * Processing.h
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_PROCESSING_PROCESSING_H_
#define INC_APPLICATION_PROCESSING_PROCESSING_H_

#include "IO.h"
#include "Control.h"

/* Component Libraries */
#include "PR_HC05.h"
#include "PR_MPU6050.h"

/* Start global typedefs */
typedef struct IP_Bus{
	IP_MPU6050_Bus_t IP_MPU6050_Bus;
} IP_Bus_t;

typedef struct OP_Bus{
	OP_HC05_Bus_t OP_HC05_Bus;
}OP_Bus_t;
/* End global typedefs */

/* Start global function prototypes */
IP_Bus_t PR_InputProcessing(HI_Bus_t HI_Bus);
OP_Bus_t PR_OutputProcessing(CT_Bus_t CT_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_PROCESSING_PROCESSING_H_ */
