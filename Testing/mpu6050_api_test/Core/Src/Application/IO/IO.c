/*
 * IO.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */
#include "IO.h"

/* Start global function definitions */
HI_Bus_t IO_HardwareInputs(void){
	static HI_Bus_t HI_Bus;

	HI_Bus.HI_MPU6050_Bus = HI_MPU6050(hi2c1);

	return HI_Bus;
}

void IO_HardwareOutputs(OP_Bus_t OP_Bus){
	HO_HCO5(huart2, OP_Bus.OP_HC05_Bus.pData, OP_Bus.OP_HC05_Bus.Size);
}
/* End global function definitions */
