/*
 * Config_VESC.h
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONFIG_CONFIG_VESC_H_
#define INC_APPLICATION_CONFIG_CONFIG_VESC_H_

#include <stdint.h>
#include "main.h"

/* Start Global Defines */
#define CAN_ID1 0x901
#define CAN_IDE1 0x01
#define CAN_RTR1 0x00

#define CAN_TSR 0x40006408
#define CAN_ESR 0x40006418
/* End Global Defines */

/* Start Global Variables */
extern CAN_TxHeaderTypeDef TxHeader;
extern uint32_t TxMailbox;

extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData[8];
/* End Global Variables */

/* Start Global Function Prototypes */
void Config_VESC(CAN_HandleTypeDef *hcan);
void CAN_HAL_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
/* End Global Function Prototypes */

#endif /* INC_APPLICATION_CONFIG_CONFIG_VESC_H_ */
