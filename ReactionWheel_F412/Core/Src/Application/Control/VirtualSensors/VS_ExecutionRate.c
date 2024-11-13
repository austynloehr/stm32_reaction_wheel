/*
 * VS_ExecutionRate.c
 *
 *  Created on: May 23, 2024
 *      Author: aloehr
 */

#include "VS_ExecutionRate.h"

/* Start Global Function Definitions */
VS_ExecutionRate_Bus_t VS_ExecutionRate(void){
	VS_ExecutionRate_Bus_t VS_ExecutionRate_Bus;
	static float dt = .002;
	static uint32_t lastTick_ms = 0;
	static bool OneShot_bool = true;

	uint32_t currentTick_ms = HAL_GetTick();

	if (OneShot_bool){
		// Use initial dt value for first cycle
		OneShot_bool = false;
	} else{
		// Calculate new dt
		dt = (currentTick_ms - lastTick_ms) / 1000.0;
	}

	lastTick_ms = currentTick_ms;

	VS_ExecutionRate_Bus.dt = dt;

	return VS_ExecutionRate_Bus;
}
/* End Global Function Definitions */

