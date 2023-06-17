/*
 * Application.c
 *
 *  Created on: Jul 9, 2023
 *      Author: aloehr
 */

#include "Application.h"

HI_Bus_t HI_Bus;
//IP_Bus_t IP_Bus;
//CT_Bus_t CT_Bus;
//OP_Bus_t OP_Bus;

void App_Config(void){
	Config_MPU6050(hi2c1);
}

void App_Main(void){
	HI_Bus = IO_HardwareInputs();
//	IP_Bus = PR_InputProcessing(HI_Bus);
//	CT_Bus = CT_Control(IP_Bus);
//	OP_Bus = PR_OutputProcessing(CT_Bus);
//	IO_HardwareOutputs(OP_Bus);
}
