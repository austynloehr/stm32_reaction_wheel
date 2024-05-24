/*
 * InputProcessing.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#include "Processing.h"

/* Start global function definitions */
IP_Bus_t PR_InputProcessing(Config_Bus_t Config_Bus, HI_Bus_t HI_Bus){
	IP_Bus_t IP_Bus;

	IP_Bus.IP_MPU6050_Bus = IP_MPU6050(HI_Bus.HI_MPU6050_Bus, Config_Bus.Config_MPU6050_Bus);

	return IP_Bus;
}

OP_Bus_t PR_OutputProcessing(CT_Bus_t CT_Bus){
	OP_Bus_t OP_Bus;

	OP_Bus.OP_HC05_Bus = OP_HC05(CT_Bus.CT_Log_Bus);
	OP_Bus.OP_VESC_Bus = OP_VESC(CT_Bus.CT_Balance_Bus.MotorSpeedReq_rpm);
	OP_Bus.OP_DiscreteOutput_Bus = OP_DiscreteOutput(CT_Bus.CT_PrimaryStateMachine_Bus.GLEDStateReq_enum, CT_Bus.CT_PrimaryStateMachine_Bus.RLEDStateReq_enum);

	return OP_Bus;
}
/* End global function definitions */
