/*
 * Application.c
 *
 *  Created on: Jul 9, 2023
 *      Author: aloehr
 */

#include "Application.h"

/* Start Global Variables */
static CT_Bus_t Last_CT_Bus;

bool EnableBtn_bool;
float RollAngle_deg;
float dt;
int32_t SpeedRequest;
PrimaryState CurrentState;
int32_t dTerm;
int32_t pTerm;
int32_t iTerm;
/* End Global Variables */

/* Start global function definition */
Config_Bus_t App_Config(Config_HAL_Bus_t Config_HAL_Bus){
	Config_Bus_t Config_Bus;

	// Startup delay for IMU and VESC
	HAL_Delay(5000);

	Config_Bus.Config_HAL_Bus = Config_HAL_Bus;
	Config_Bus.Config_MPU6050_Bus = Config_MPU6050(Config_HAL_Bus.hi2c);

	Config_DSP();
	Config_VESC(Config_HAL_Bus.hcan);

	// Initialize unit delayed bus states
	Last_CT_Bus.CT_PrimaryStateMachine_Bus.CurrentState_enum = Standby;

	return Config_Bus;
}

void App_Main(Config_Bus_t Config_Bus){
	HI_Bus_t HI_Bus;
	IP_Bus_t IP_Bus;
	CT_Bus_t CT_Bus;
	OP_Bus_t OP_Bus;

	HI_Bus = IO_HardwareInputs(Config_Bus);
	IP_Bus = PR_InputProcessing(Config_Bus, HI_Bus, Last_CT_Bus);
	CT_Bus = CT_Control(HI_Bus, IP_Bus);
	OP_Bus = PR_OutputProcessing(CT_Bus);
	IO_HardwareOutputs(Config_Bus, OP_Bus);

	Last_CT_Bus = CT_Bus;

	EnableBtn_bool = HI_Bus.HI_DiscreteInput_Bus.EnableBtn_bool;
	dt = CT_Bus.VS_Bus.VS_ExecutionRate_Bus.dt;
	SpeedRequest = CT_Bus.CT_Balance_Bus.MotorSpeedReq_rpm;
	RollAngle_deg = CT_Bus.VS_Bus.VS_Orientation_Bus.RollAngle_deg;
	CurrentState = CT_Bus.CT_PrimaryStateMachine_Bus.CurrentState_enum;
	pTerm = CT_Bus.CT_Balance_Bus.pTerm_rpm;
	iTerm = CT_Bus.CT_Balance_Bus.iTerm_rpm;
	dTerm = CT_Bus.CT_Balance_Bus.dTerm_rpm;


	HAL_Delay(1);
}
/* End global function definition */
