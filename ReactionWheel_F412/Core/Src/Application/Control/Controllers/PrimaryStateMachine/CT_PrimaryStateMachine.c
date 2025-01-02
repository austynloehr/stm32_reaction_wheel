/*
 * PrimaryStateMachine.c
 *
 *  Created on: May 24, 2024
 *      Author: aloehr
 */

#include "CT_PrimaryStateMachine.h"
#include "PrimaryStateMachine.h"

/* Start Defines */

/* End Defines */

/* Start local enum definitions */

/* End local enum definitions */

/* Start Global Variables */
static bool StateMachineInit_bool = true;

/* Simulink Variables*/
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */
static float StartBalanceThreshold_deg = 10;
static float StopBalanceThreshold_deg = 30;
static PrimaryState CurrentState_enum;
static bool MotorEnable_bool;
static LEDState GLEDState_enum;
static LEDState RLEDState_enum;

/* End Global Variables */

/* Start Static Function Prototypes */

/* End Static Function Prototypes */

/* Start Global Function Definitions */
CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine(StateReq StateRequest_enum, bool ErrorInterrupt_bool, float RollAngle_deg, bool IMUCalComplete_bool){
	RT_MODEL *const rtM = rtMPtr;
	CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine_Bus;

	/* Initialize state machine outputs */
	if (StateMachineInit_bool){
		/* Pack model data into RTM */
		rtM->dwork = &rtDW;
		/* Initialize model */
		PrimaryStateMachine_initialize(rtM, &StateRequest_enum, &CurrentState_enum);

		StateMachineInit_bool = false;
	}

	PrimaryStateMachine_step(rtM, StateRequest_enum, ErrorInterrupt_bool, RollAngle_deg, StartBalanceThreshold_deg,
		StopBalanceThreshold_deg, IMUCalComplete_bool, &CurrentState_enum, &MotorEnable_bool, &GLEDState_enum, &RLEDState_enum);

	CT_PrimaryStateMachine_Bus.CurrentState_enum = CurrentState_enum;
	CT_PrimaryStateMachine_Bus.MotorEnable_bool = MotorEnable_bool;
	CT_PrimaryStateMachine_Bus.GLEDStateReq_enum = GLEDState_enum;
	CT_PrimaryStateMachine_Bus.RLEDStateReq_enum = RLEDState_enum;

	return CT_PrimaryStateMachine_Bus;

}

/* End Global Function Definitions */

/* Start Static Function Definitions */

/* End Static Function Definitions */
