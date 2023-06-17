/*
 * IO_HC05.c
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */
#include "IO_HC05.h"

/* Start Defines */
#define UART_TIMEOUT 50 //MS
/* End Defines */

/* Start Global Function Definitions */
void HO_HCO5(UART_HandleTypeDef huart, const uint8_t *pData, uint16_t Size){
	HAL_UART_Transmit(&huart, pData, Size, (uint32_t) UART_TIMEOUT);
}
/* End Global Function Definitions */

