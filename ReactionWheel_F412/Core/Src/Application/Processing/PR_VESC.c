/*
 * PR_VESC.c
 *
 *  Created on: Nov 24, 2023
 *      Author: aloehr
 */

#include "PR_VESC.h"
#include <stdlib.h>

/* Start defines */
#define NUM_POLES 14
#define NUM_POLE_PAIRS NUM_POLES / 2
#define MAX_SPEED_RPM 11000
#define MAX_SPEED_ERPM MAX_SPEED_RPM * (NUM_POLES / 2)
#define MAX_CURRENT_A 20000
/* End defines */

/* Start function prototypes */
static void CANPackMotorCmd(int32_t Command, uint8_t ReqPayload[]);
static int8_t llsign(uint32_t x);
/* End function prototypes */

/* Start global function definitions */
OP_VESC_Bus_t OP_VESC(int32_t Request, MotorCtrlMode_enum_t ControlMode){
	OP_VESC_Bus_t OP_VESC_Bus;
	static int32_t Command;
	static uint8_t ReqPayload[4];

	// Request limiting based on control mode
	switch(ControlMode){
	case Speed:
		int32_t MotorSpeed_erpm = Request * (NUM_POLE_PAIRS);

		// Limit speed request
		if(llabs(MotorSpeed_erpm) > MAX_SPEED_ERPM){
			MotorSpeed_erpm = MAX_SPEED_ERPM * llsign(MotorSpeed_erpm);
		}

		// Set command and update CAN ID for control mode
		Command = MotorSpeed_erpm;
		OP_VESC_Bus.CanID = 0x301;
		break;

	case Current:
		int32_t MotorCurrent_mA = Request;

		// Limit current request
		if(llabs(MotorCurrent_mA) > MAX_CURRENT_A){
			MotorCurrent_mA = MAX_CURRENT_A * llsign(MotorCurrent_mA);
		}

		// Set command and update CAN ID for control mode
		Command = MotorCurrent_mA;
		OP_VESC_Bus.CanID = 0x101;
		break;

	default:
		Command = 0;
	}

	// Pack CAN message
	CANPackMotorCmd(Command, ReqPayload);

	// Store pointer to packed CAN message
	OP_VESC_Bus.pTxData = ReqPayload;

	return OP_VESC_Bus;
}

/* End global function definitions */

/* Start static function definitions */
static void CANPackMotorCmd(int32_t Command, uint8_t* pReqPayload){
	pReqPayload[0] = Command >> 24;
	pReqPayload[1] = (Command >> 16) & 0xFF;
	pReqPayload[2] = (Command >> 8) & 0xFF;
	pReqPayload[3] = Command & 0xFF;
}

static int8_t llsign(uint32_t x){
	int8_t sign;

	if (x > 0){
		sign = 1;
	}
	else if (x < 0) {
		sign = -1;
	}
	else {
		sign = 0;
	}

	return sign;
}
/* End static function definitions */
