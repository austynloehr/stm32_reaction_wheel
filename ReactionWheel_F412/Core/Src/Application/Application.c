/*
 * Application.c
 *
 *  Created on: Jul 9, 2023
 *      Author: aloehr
 */

#include "Application.h"

bool EnableBtn_bool;
float RollAngle_deg;

/* Start global function definition */
Config_Bus_t App_Config(Config_HAL_Bus_t Config_HAL_Bus){
	Config_Bus_t Config_Bus;

	HAL_Delay(200);

	Config_Bus.Config_HAL_Bus = Config_HAL_Bus;
	Config_Bus.Config_MPU6050_Bus = Config_MPU6050(Config_HAL_Bus.hi2c);

	Config_DSP();
	Config_VESC(Config_HAL_Bus.hcan);

	return Config_Bus;
}

void App_Main(Config_Bus_t Config_Bus){
	HI_Bus_t HI_Bus;
	IP_Bus_t IP_Bus;
	CT_Bus_t CT_Bus;
	OP_Bus_t OP_Bus;

	HI_Bus = IO_HardwareInputs(Config_Bus);
	IP_Bus = PR_InputProcessing(Config_Bus, HI_Bus);
	CT_Bus = CT_Control(HI_Bus, IP_Bus);
	OP_Bus = PR_OutputProcessing(CT_Bus);
	IO_HardwareOutputs(Config_Bus, OP_Bus);

	EnableBtn_bool = HI_Bus.HI_DiscreteInput_Bus.EnableBtn_bool;
	RollAngle_deg = CT_Bus.VS_Bus.VS_Orientation_Bus.CompFiltOrientation.roll_deg;

	HAL_Delay(4);
}
/* End global function definition */
