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
/* End defines */

/* Start global variables */
uint8_t SpeedReqPayload[4];
/* End global variables */

/* Start function prototypes */
static void CANPackMotorSpeed(int32_t speed);
static int8_t llsign(uint32_t x);
/* End function prototypes */

/* Start global function definitions */
OP_VESC_Bus_t OP_VESC(int32_t MotorSpeed_rpm){
	OP_VESC_Bus_t OP_VESC_Bus;

	int32_t MotorSpeed_erpm = MotorSpeed_rpm * (NUM_POLE_PAIRS);

	// Limit speed request
	if(llabs(MotorSpeed_erpm) > MAX_SPEED_ERPM){
		MotorSpeed_erpm = MAX_SPEED_ERPM * llsign(MotorSpeed_erpm);
	}

	CANPackMotorSpeed(MotorSpeed_erpm);

	OP_VESC_Bus.pTxData = SpeedReqPayload;

	return OP_VESC_Bus;
}

/* End global function definitions */

/* Start static function definitions */
static void CANPackMotorSpeed(int32_t speed){
	SpeedReqPayload[0] = speed >> 24;
	SpeedReqPayload[1] = (speed >> 16) & 0xFF;
	SpeedReqPayload[2] = (speed >> 8) & 0xFF;
	SpeedReqPayload[3] = speed & 0xFF;
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
