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

/* Start Global Function Prototypes */
void HO_VESC(CAN_HandleTypeDef *hcan, uint8_t *TxData);
/* End Global Function Prototypes */

#endif /* INC_APPLICATION_IO_IO_VESC_H_ */
