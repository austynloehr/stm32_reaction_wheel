/*
 * IO_VESC.h
 *
 *  Created on: Nov 24, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_IO_IO_VESC_H_
#define INC_APPLICATION_IO_IO_VESC_H_

#include "main.h"
#include "Config_VESC.h"
#include "PR_VESC.h"

/* Start Global Struct definitions */
typedef struct HI_VESC_Bus {
	int32_t		MotorSpeed_rpm;
	int32_t 	MotorCurrent_mA;
} HI_VESC_Bus_t;
/* End Global Struct definitions */

/* Start Global Function Prototypes */
HI_VESC_Bus_t HI_VESC(void);
void HO_VESC(CAN_HandleTypeDef *hcan, uint8_t *TxData, uint32_t CanID);
/* End Global Function Prototypes */

#endif /* INC_APPLICATION_IO_IO_VESC_H_ */
