/*
 * VS_StateRequest.c
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#include "VS_StateRequest.h"
#include "StateRequest.h"              /* Model header file */

/* Start Global Variables */
static bool StateReqInit_bool = true;

/* Simulink Variables*/
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */
static float ShortPressTime_s = 0.2;
static float LongPressTime_s = 5.0;
static StateReq StateRequest_enum;
/* End Global Variables */

/* Start Global Function Definitions */
VS_StateRequest_Bus_t VS_StateRequest(bool EnableBtn_bool, PrimaryState LastState_enum, float dt){
	RT_MODEL *const rtM = rtMPtr;
	VS_StateRequest_Bus_t VS_StateRequest_Bus;

	/* Initialize state machine outputs */
	if (StateReqInit_bool){
		/* Pack model data into RTM */
		rtM->dwork = &rtDW;
		/* Initialize model */
		StateRequest_initialize(rtM, &LastState_enum, &StateRequest_enum);
		StateReqInit_bool = false;
	}

	/* Update states */
	StateRequest_step(rtM, EnableBtn_bool, LastState_enum, ShortPressTime_s, LongPressTime_s, dt, &StateRequest_enum);

	/* Store output */
	VS_StateRequest_Bus.StateRequest_enum = StateRequest_enum;

	return VS_StateRequest_Bus;
}
/* End Global Function Definitions */
