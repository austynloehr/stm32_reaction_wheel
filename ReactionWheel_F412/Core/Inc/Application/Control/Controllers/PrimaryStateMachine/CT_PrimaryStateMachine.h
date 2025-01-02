/*
 * PrimaryStateMachine.h
 *
 *  Created on: May 24, 2024
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_CONTROLLERS_PRIMARYSTATEMACHINE_CT_PRIMARYSTATEMACHINE_H_
#define INC_APPLICATION_CONTROL_CONTROLLERS_PRIMARYSTATEMACHINE_CT_PRIMARYSTATEMACHINE_H_

#include "main.h"
#include "SimulinkEnums.h"

/* Start Global Struct definitions */
typedef struct CT_PrimaryStateMachine_Bus{
	PrimaryState CurrentState_enum;
	bool MotorEnable_bool;
	LEDState GLEDStateReq_enum;
	LEDState RLEDStateReq_enum;
}CT_PrimaryStateMachine_Bus_t;
/* End Global Struct definitions */

/* Start Function Prototypes */
CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine(StateReq StateRequest_enum, bool ErrorInterrupt_bool, float RollAngle_deg, bool IMUCalComplete_bool);
/* End Function Prototypes */

#endif /* INC_APPLICATION_CONTROL_CONTROLLERS_PRIMARYSTATEMACHINE_CT_PRIMARYSTATEMACHINE_H_ */
