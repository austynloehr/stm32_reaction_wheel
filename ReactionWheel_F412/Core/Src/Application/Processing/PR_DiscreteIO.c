/*
 * PR_DiscreteIO.c
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#include "PR_DiscreteIO.h"

/* Start Defines */
#define BLINK_PERIOD_TIC 300U
/* End Defines */

/* Start Global Variable Definition */
static bool GLED_LastState = false;
static bool RLED_LastState = false;
static uint32_t GLED_BlinkCnt = 0;
static uint32_t RLED_BlinkCnt = 0;
/* End Global Variable Definition */

/* Start Function Prototypes */
static bool LEDStateToCommand(LEDState StateReq_enum, bool* pLastState, uint32_t* pBlinkCnt);
/* End Function Prototypes */

/* Start Global Function Definition */
OP_DiscreteOutput_Bus_t OP_DiscreteOutput(LEDState GLEDStateReq_enum, LEDState RLEDStateReq_enum){
	OP_DiscreteOutput_Bus_t OP_DiscreteOutput_Bus;

	OP_DiscreteOutput_Bus.GLEDCmd_bool = LEDStateToCommand(GLEDStateReq_enum, &GLED_LastState, &GLED_BlinkCnt);
	OP_DiscreteOutput_Bus.RLEDCmd_bool = LEDStateToCommand(RLEDStateReq_enum, &RLED_LastState, &RLED_BlinkCnt);

	return OP_DiscreteOutput_Bus;
}
/* End Global Function Definition */

/* Start Static Function Definition */
static bool LEDStateToCommand(LEDState StateReq_enum, bool* pLastState, uint32_t* pBlinkCnt){
	bool LEDstate;
	bool lastState = *pLastState;
	uint32_t blinkCnt = *pBlinkCnt;

	switch (StateReq_enum){
	case On:
		LEDstate = true;
		*pBlinkCnt = 0;
		break;

	case Blink:
		if (blinkCnt == 0){
			LEDstate = true; // First tick
		} else if ((blinkCnt % BLINK_PERIOD_TIC) == 0){
			LEDstate = !lastState; // Toggle state if period has passed
		} else{
			LEDstate = lastState; // Hold last state
		}
		*pBlinkCnt = *pBlinkCnt + 1; // Increment blink count by one tick
		break;

	default:
		LEDstate = false;
		*pBlinkCnt = 0;
	}

	*pLastState = LEDstate; // Store last state for next iteration

	return LEDstate;
}

/* End Static Function Definition */
