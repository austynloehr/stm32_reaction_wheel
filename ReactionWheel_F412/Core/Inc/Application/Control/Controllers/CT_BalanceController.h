/*
 * CT_BalanceController.h
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_
#define INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_

#include "main.h"
#include "SimulinkEnums.h"

/* Start enum definitions */
typedef enum MotorCtrlMode {
	DutyCycle 		= 0x00,
	Current 		= 0x01,
	CurrentBrake 	= 0x02,
	Speed 			= 0x03,
} MotorCtrlMode_enum_t;
/* End enum definitions */

/* Start Struct definitions */
typedef struct CT_Balance_Bus{
	int32_t MotorRequest_na;
	int32_t pTerm_mA;
	int32_t iTerm_mA;
	int32_t dTerm_mA;
	int32_t PIDTerm_mA;
	int32_t MotorSpeedTerm_mA;
	MotorCtrlMode_enum_t ControlMode;
}CT_Balance_Bus_t;
/* End Struct definitions */

/* Start global function prototypes */
CT_Balance_Bus_t CT_BalanceController(bool MotorEnable_bool, PrimaryState CurrentState_enum, float RollAng_deg, float dt, int32_t MotorSpeed_rpm);
/* End global function prototypes */

#endif /* INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_ */
