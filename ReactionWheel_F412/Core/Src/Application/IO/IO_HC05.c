/*
 * IO_HC05.c
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */
#include "IO_HC05.h"

/* Start Defines */
#define UART_TIMEOUT 50 //MS
#define HC05_FCN_CALL_DIV 7 //CYCLES
/* End Defines */


/* Start Global Function Definitions */
void HO_HCO5(UART_HandleTypeDef huart, const uint8_t *pData, uint16_t Size){
	static uint32_t FcnCallCnt = 0;

	if (FcnCallCnt >= HC05_FCN_CALL_DIV - 1) {
		HAL_UART_Transmit_DMA(&huart, pData, Size);

		FcnCallCnt = 0;
	} else {
		FcnCallCnt++;
	}

//	HAL_UART_Transmit(&huart, pData, Size, (uint32_t) UART_TIMEOUT);
}
/* End Global Function Definitions */

