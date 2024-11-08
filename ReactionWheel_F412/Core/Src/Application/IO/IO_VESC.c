/*
 * IO_VESC.c
 *
 *  Created on: Nov 24, 2023
 *      Author: aloehr
 */

#include "IO_VESC.h"

/* Start Defines */
#define IO_VESC_FcnCallDivider 10
/* End Defines */

/* Start global function definitions */
void HO_VESC(CAN_HandleTypeDef *hcan, uint8_t *TxData){
	static uint32_t FcnCallCnt = 0;

	if (FcnCallCnt % IO_VESC_FcnCallDivider == 0){
//		if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK){
//			Error_Handler();
//		}
	}

	FcnCallCnt++;
}
/* End global function definitions */
