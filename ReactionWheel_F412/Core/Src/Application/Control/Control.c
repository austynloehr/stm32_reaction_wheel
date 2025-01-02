/*
 * Control.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */
#include "Control.h"

/* Start Global Variables */
static CT_Bus_t Last_CT_Bus;
/* End Global Variables */

/* Start Static function prototypes */
static VS_Bus_t CT_VirtualSensors(HI_Bus_t HI_Bus, IP_Bus_t IP_Bus, CT_Bus_t Last_CT_Bus);
static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus, VS_Bus_t VS_Bus, HI_Bus_t HI_Bus);
/* End Static function prototypes */

/* Start global function definitions */
CT_Bus_t CT_Control(HI_Bus_t HI_Bus, IP_Bus_t IP_Bus){
	VS_Bus_t VS_Bus;
	CT_Bus_t CT_Bus;

	VS_Bus = CT_VirtualSensors(HI_Bus, IP_Bus, Last_CT_Bus);
	CT_Bus = CT_Controllers(IP_Bus, VS_Bus, HI_Bus);

	Last_CT_Bus = CT_Bus;

	return CT_Bus;
}
/* End global function definitions */

/* Start Static function definitions */
static VS_Bus_t CT_VirtualSensors(HI_Bus_t HI_Bus, IP_Bus_t IP_Bus, CT_Bus_t Last_CT_Bus){
	VS_Bus_t VS_Bus;

	VS_Bus.VS_ExecutionRate_Bus = VS_ExecutionRate();
	VS_Bus.VS_Orientation_Bus = EstimateOrientation(IP_Bus.IP_MPU6050_Bus, VS_Bus.VS_ExecutionRate_Bus.dt);
	VS_Bus.VS_StateRequest_Bus = VS_StateRequest(HI_Bus.HI_DiscreteInput_Bus.EnableBtn_bool, Last_CT_Bus.CT_PrimaryStateMachine_Bus.CurrentState_enum, VS_Bus.VS_ExecutionRate_Bus.dt);

	return VS_Bus;
}

static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus, VS_Bus_t VS_Bus, HI_Bus_t HI_Bus){
	CT_Bus_t CT_Bus;

	CT_Bus.VS_Bus = VS_Bus;

	CT_Bus.CT_PrimaryStateMachine_Bus = CT_PrimaryStateMachine(VS_Bus.VS_StateRequest_Bus.StateRequest_enum,
			false,
			VS_Bus.VS_Orientation_Bus.RollAngle_deg,
			IP_Bus.IP_MPU6050_Bus.IMUCalComplete);

	CT_Bus.CT_Balance_Bus = CT_BalanceController(CT_Bus.CT_PrimaryStateMachine_Bus.MotorEnable_bool,
			CT_Bus.CT_PrimaryStateMachine_Bus.CurrentState_enum,
			VS_Bus.VS_Orientation_Bus.RollAngle_deg,
			VS_Bus.VS_ExecutionRate_Bus.dt,
			HI_Bus.HI_VESC_Bus.MotorSpeed_rpm);

	CT_Bus.CT_Log_Bus = CT_Logging(IP_Bus.IP_MPU6050_Bus, HI_Bus.HI_VESC_Bus, VS_Bus.VS_Orientation_Bus, VS_Bus.VS_StateRequest_Bus, CT_Bus.CT_PrimaryStateMachine_Bus, CT_Bus.CT_Balance_Bus);

	return CT_Bus;
}
/* End static function definitions */
