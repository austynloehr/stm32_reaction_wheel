/*
 * IO_HardwareInputs.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */
#include "IO.h"

/* Start global function definitions */
HI_Bus_t IO_HardwareInputs(Config_Bus_t Config_Bus){
	HI_Bus_t HI_Bus;

	HI_Bus.HI_MPU6050_Bus = HI_MPU6050(Config_Bus.Config_HAL_Bus.hi2c);

	return HI_Bus;
}

void IO_HardwareOutputs(Config_Bus_t Config_Bus, OP_Bus_t OP_Bus){
	HO_VESC(Config_Bus.Config_HAL_Bus.hcan, OP_Bus.OP_VESC_Bus.pTxData);
	HO_HCO5(Config_Bus.Config_HAL_Bus.huart2, OP_Bus.OP_HC05_Bus.pData, OP_Bus.OP_HC05_Bus.Size);
}
/* End global function definitions */

