/*
 * IO_VESC.c
 *
 *  Created on: Nov 24, 2023
 *      Author: aloehr
 */

#include "IO_VESC.h"

/* Start Defines */
#define IO_VESC_FcnCallDivider 5
#define IO_VESC_MaxFcnCallCnt 100
/* End Defines */

/* Start global function definitions */
HI_VESC_Bus_t HI_VESC(void){
	static HI_VESC_Bus_t HI_VESC_Bus;
	static uint16_t FcnCallCnt = 0;

	// Receive message every ten cycles
	if (FcnCallCnt % IO_VESC_FcnCallDivider == 0){
		uint8_t MsgPacked[8];
		int32_t MotorSpeed_rpm;
		int16_t MotorCurrent_dA;

		// Copy message from global variable
		memcpy(MsgPacked, RxData, sizeof(MsgPacked));

		MotorSpeed_rpm = (MsgPacked[0] << 24) | (MsgPacked[1] << 16) | (MsgPacked[2] << 8) | (MsgPacked[3]);
		MotorCurrent_dA = (MsgPacked[4] << 8) | (MsgPacked[5]);

		HI_VESC_Bus.MotorSpeed_rpm = MotorSpeed_rpm / 7; // 7 pole pairs
		HI_VESC_Bus.MotorCurrent_mA = (int32_t) (MotorCurrent_dA * 100);
	}

	// Rollover counter to prevent overrun
	if (FcnCallCnt >= IO_VESC_MaxFcnCallCnt){
		FcnCallCnt = 0;
	}
	FcnCallCnt++;

	return HI_VESC_Bus;
}


void HO_VESC(CAN_HandleTypeDef *hcan, uint8_t *TxData, uint32_t CanID){
	static uint16_t FcnCallCnt = 0;

	// Transmit every ten cycles
	if (FcnCallCnt % IO_VESC_FcnCallDivider == 0){
		// Set CAN ID
		TxHeader.ExtId = CanID;

		// Send CAN message
		if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK){
			Error_Handler();
		}
	}

	// Rollover counter to prevent overrun
	if (FcnCallCnt >= IO_VESC_MaxFcnCallCnt){
		FcnCallCnt = 0;
	}
	FcnCallCnt++;
}
/* End global function definitions */
