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

/* Start Struct definitions */
typedef struct CT_Balance_Bus{
	int32_t MotorSpeedReq_rpm;
	int32_t pTerm_rpm;
	int32_t iTerm_rpm;
	int32_t dTerm_rpm;
}CT_Balance_Bus_t;
/* End Struct definitions */

/* Start global function prototypes */
CT_Balance_Bus_t CT_BalanceController(bool MotorEnable_bool, PrimaryState CurrentState_enum, float RollAng_deg, float dt);
/* End global function prototypes */

#endif /* INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_ */
