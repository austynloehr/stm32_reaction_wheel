/*
 * CT_BalanceController.c
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */
#include "CT_BalanceController.h"
#include <math.h>

/* Start Defines */
#define JUMP_UP_MTR_SPD 3700
#define MAX_MTR_SPD 10000
#define JUMP_UP_HOLD_CNT 750
#define PID_PTERM 100
#define PID_ITERM 75
#define PID_DTERM 2
#define PID_IMAX 3000
#define PID_RESET_VAL 0
#define PID_ERROR_GAIN 1
/* End Defines */

/* Start local enum definitions */
typedef struct PID_Input_Data{
	float setPoint;
	float feedback;
	float errorGain;
	float p;
	float i;
	float d;
	float iMin;
	float iMax;
	bool reset;
	float resetValue;
	float dt;
}PID_Input_Data_t;

typedef struct PID_Output_Data{
	float output;
	float error;
	float pTerm;
	float iTerm;
	float dTerm;
}PID_Output_Data_t;
/* End local enum definitions */

/* Start Static function prototypes */
static int8_t signf(float x);
static int32_t JumpUpControl(float RollAng_deg, uint32_t *pJumpUpTimer_cnt);
static void PID_Resettable(PID_Input_Data_t *pInputData, PID_Output_Data_t *pOutputData);
/* End Static function prototypes */

/* Start Global Function Definitions */
CT_Balance_Bus_t CT_BalanceController(bool MotorEnable_bool, PrimaryState CurrentState_enum, float RollAng_deg, float dt){
	CT_Balance_Bus_t CT_Balance_Bus;
	static PID_Input_Data_t PID_InputData;
	PID_Output_Data_t PID_OutputData;
	int32_t MotorSpeedReq_rpm;
	int32_t PID_MotorSpeedReq_rpm;
	int32_t JumpUp_MotorSpeedReq_rpm;
	static uint32_t JumpUpTimer_cnt = 0;
	static bool OneShot = true;

	if (OneShot){
		// Initialize PI input instance values
		PID_InputData.errorGain = PID_ERROR_GAIN;
		PID_InputData.resetValue = PID_RESET_VAL;
		PID_InputData.p = PID_PTERM;
		PID_InputData.i = PID_ITERM;
		PID_InputData.d = PID_DTERM;
		PID_InputData.iMin = -1 * PID_IMAX;
		PID_InputData.iMax = PID_IMAX;
		PID_InputData.resetValue = PID_RESET_VAL;

		OneShot = false;
	}
	PID_InputData.setPoint = 0;
	PID_InputData.feedback = RollAng_deg;
	PID_InputData.dt = dt;

	if (!(CurrentState_enum == Balance)){
		PID_InputData.reset = true;
	} else {
		PID_InputData.reset = false;
	}

	PID_Resettable(&PID_InputData, &PID_OutputData);
	PID_MotorSpeedReq_rpm = (int32_t) floor(PID_OutputData.output);
	JumpUp_MotorSpeedReq_rpm = JumpUpControl(RollAng_deg, &JumpUpTimer_cnt);

	// Reset jump up rev timer
	if(!(CurrentState_enum == JumpUp)){
		JumpUpTimer_cnt = 0;
	}

	if (MotorEnable_bool){
		switch(CurrentState_enum){

		case JumpUp:
			MotorSpeedReq_rpm = JumpUp_MotorSpeedReq_rpm;
			break;

		case Balance:
			MotorSpeedReq_rpm = PID_MotorSpeedReq_rpm;
			break;

		default:
			MotorSpeedReq_rpm = 0;
		}

		/* Speed limit */
		if (abs(MotorSpeedReq_rpm) > MAX_MTR_SPD){
			MotorSpeedReq_rpm = MAX_MTR_SPD * signf(MotorSpeedReq_rpm);
		}

	} else{
		MotorSpeedReq_rpm = 0;
	}

	CT_Balance_Bus.MotorSpeedReq_rpm = MotorSpeedReq_rpm;
	CT_Balance_Bus.pTerm_rpm = (int32_t) floor(PID_OutputData.pTerm);
	CT_Balance_Bus.iTerm_rpm = (int32_t) floor(PID_OutputData.iTerm);
	CT_Balance_Bus.dTerm_rpm = (int32_t) floor(PID_OutputData.dTerm);

	return CT_Balance_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static int32_t JumpUpControl(float RollAng_deg, uint32_t *pJumpUpTimer_cnt){
	int32_t MotorSpeedRequest_rpm;

	int8_t sign = -1 * signf(RollAng_deg);

	if (*pJumpUpTimer_cnt < JUMP_UP_HOLD_CNT){
		MotorSpeedRequest_rpm = sign * JUMP_UP_MTR_SPD;
	} else{
		MotorSpeedRequest_rpm = -1 * sign * JUMP_UP_MTR_SPD;
	}

	*pJumpUpTimer_cnt = *pJumpUpTimer_cnt + 1;

	return MotorSpeedRequest_rpm;
}

static void PID_Resettable(PID_Input_Data_t *pInputData, PID_Output_Data_t *pOutputData){
	float iVal;
	static float lastFeedback = 0;
	static float lastIVal = PID_RESET_VAL;
	static bool iTermLimited = false;

	float error = pInputData->errorGain * (pInputData->setPoint - pInputData->feedback);

	float pTerm = pInputData->p * error;

	// Eulers method for integration
	if (iTermLimited){
		iVal = lastIVal; // Stop integrating
	} else {
		iVal = lastIVal + (pInputData->dt * error);
	}

	// Reset integrator to reset value
	if (pInputData->reset){
		iVal= pInputData->resetValue;
	}

	lastIVal = iVal; // store for next integration step

	float iTerm = pInputData->i * iVal;
	// Limit i term
	if (iTerm < pInputData->iMin){
		iTerm = pInputData->iMin;
		iTermLimited = true;
	} else if (iTerm > pInputData->iMax){
		iTerm = pInputData->iMax;
		iTermLimited = true;
	} else {
		iTermLimited = false;
	}

	float dVal = (lastFeedback - pInputData->feedback) / pInputData->dt;

	float dTerm = dVal * pInputData->d;

	lastFeedback = pInputData->feedback;

	// Update output instance
	pOutputData->output = pTerm + iTerm + dTerm;
	pOutputData->error = error;
	pOutputData->pTerm = pTerm;
	pOutputData->iTerm = iTerm;
	pOutputData->dTerm = dTerm;
}

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
