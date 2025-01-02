/*
 * PR_VESC.h
 *
 *  Created on: Nov 24, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_PROCESSING_PR_VESC_H_
#define INC_APPLICATION_PROCESSING_PR_VESC_H_

#include "main.h"
#include "CT_BalanceController.h"

/* Start Global Struct definitions */
typedef struct OP_VESC_Bus {
	uint8_t *pTxData;
	uint32_t CanID;
} OP_VESC_Bus_t;
/* End Global Struct definitions */

/* Start Function Prototypes */
OP_VESC_Bus_t OP_VESC(int32_t command, MotorCtrlMode_enum_t controlMode);
/* End Function Prototypes */

#endif /* INC_APPLICATION_PROCESSING_PR_VESC_H_ */
