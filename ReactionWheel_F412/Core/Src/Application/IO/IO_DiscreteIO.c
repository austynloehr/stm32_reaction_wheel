/*
 * IO_Discrete.c
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#include "IO_DiscreteIO.h"

/* Start global function definitions */
HI_DiscreteInput_Bus_t HI_DiscreteInput(void){
	HI_DiscreteInput_Bus_t HI_DiscreteInput_Bus;

	// Input pulled up, need to NOT
	HI_DiscreteInput_Bus.EnableBtn_bool = !HAL_GPIO_ReadPin(EnableBtn_GPIO_Port, EnableBtn_Pin);

	return HI_DiscreteInput_Bus;
}

void HO_DiscreteIO(bool GLEDCmd_bool, bool RLEDCmd_bool){
	HAL_GPIO_WritePin(GLED_GPIO_Port, GLED_Pin, (GPIO_PinState) GLEDCmd_bool);
	HAL_GPIO_WritePin(RLED_GPIO_Port, RLED_Pin, (GPIO_PinState) RLEDCmd_bool);
}
/* End global function definitions */
