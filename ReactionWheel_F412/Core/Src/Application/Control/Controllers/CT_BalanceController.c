/*
 * CT_BalanceController.c
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */
#include "CT_BalanceController.h"
#include <math.h>

/* Start Static function prototypes */
static int8_t signf(float x);
/* End Static function prototypes */

/* Start Global Function Definitions */
CT_Balance_Bus_t CT_BalanceController(uint8_t BalanceEnable_bool, float RollAng_deg){
	CT_Balance_Bus_t CT_Balance_Bus;

	uint32_t MaxMotorSpeed_rpm = 5000;
	uint32_t MaxRollAng_deg = 45;

	// Limit throttle request to -1 / 1
	float ThrottleReq_pct = RollAng_deg / MaxRollAng_deg;

	if(fabsf(ThrottleReq_pct) > 1){
		ThrottleReq_pct = 1 * signf(ThrottleReq_pct);
	}

	// Throttle deadzone
	if(fabsf(RollAng_deg) < 2){
		BalanceEnable_bool = 0;
	} else{
		BalanceEnable_bool = 1;
	}

	// Zero out throttle if not enabled
	if(!BalanceEnable_bool){
		ThrottleReq_pct = 0;
	}

	CT_Balance_Bus.MotorSpeedReq_rpm = ThrottleReq_pct * MaxMotorSpeed_rpm;
	CT_Balance_Bus.ThrottleReq_pct = ThrottleReq_pct;

	return CT_Balance_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static int8_t signf(float x){
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
/* End Static Function Definitions */
