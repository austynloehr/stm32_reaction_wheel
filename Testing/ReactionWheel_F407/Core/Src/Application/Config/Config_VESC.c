/*
 * Config_VESC.c
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */

#include "Config_VESC.h"

/* Start global variables */
CAN_TxHeaderTypeDef TxHeader;
uint32_t TxMailbox;

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
/* End global variables */

/* Start global function definitions */
void Config_VESC(CAN_HandleTypeDef *hcan){
	CAN_FilterTypeDef canfilterconfig;

	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 0;
	canfilterconfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canfilterconfig.FilterIdHigh = 0x0000;
	canfilterconfig.FilterIdLow = ((CAN_ID1 << 3) ^ (CAN_IDE1 << 2)) ^ (CAN_RTR1 << 1);
	canfilterconfig.FilterMaskIdHigh = 0x0000;
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDLIST;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(hcan, &canfilterconfig);

	void (*pCAN_RxCallback)(CAN_HandleTypeDef *_hcan) = &CAN_HAL_RxFifo0MsgPendingCallback;

	if(HAL_CAN_RegisterCallback(hcan, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, pCAN_RxCallback) != HAL_OK){
	  Error_Handler();
	}

	HAL_CAN_Start(hcan);

	HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	TxHeader.DLC = 4;
	TxHeader.ExtId = 0x301;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = 0;
	TxHeader.TransmitGlobalTime = DISABLE;
}

void CAN_HAL_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
}
/* End global function definitions */
