/*
 * VS_ExecutionRate.c
 *
 *  Created on: May 23, 2024
 *      Author: aloehr
 */

#include "VS_ExecutionRate.h"

/* Start Global Variables */
uint32_t lastTick_ms = 0;
float initialDt_s = .005;
bool oneShot_bool = true;
/* End Global Variables */

/* Start Global Function Definitions */
VS_ExecutionRate_Bus_t VS_ExecutionRate(void){
	VS_ExecutionRate_Bus_t VS_ExecutionRate_Bus;
	uint32_t currentTick_ms = HAL_GetTick();

	float dt;

	if (oneShot_bool){
		dt = initialDt_s;
		oneShot_bool = false;
	} else{
		dt = (currentTick_ms - lastTick_ms) / 1000.0;
	}

	lastTick_ms = currentTick_ms;

	VS_ExecutionRate_Bus.dt = dt;

	return VS_ExecutionRate_Bus;
}
/* End Global Function Definitions */

