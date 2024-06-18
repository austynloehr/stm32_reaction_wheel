/*
 * CT_BalanceController.c
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */
#include "CT_BalanceController.h"
#include <math.h>

/* Start Defines */
#define JUMP_UP_MTR_SPD 4000
#define JUMP_UP_HOLD_CNT 750
#define PI_PTERM 100
#define PI_ITERM 0
#define PI_IMAX 1000
#define PI_RESET_VAL 0
#define PI_ERROR_GAIN 1
/* End Defines */

/* Start local enum definitions */
typedef struct PI_Input_Data{
	float setPoint;
	float feedback;
	float errorGain;
	float p;
	float i;
	float iMin;
	float iMax;
	bool reset;
	float resetValue;
	float dt;
}PI_Input_Data_t;

typedef struct PI_Output_Data{
	float output;
	float error;
	float pTerm;
	float iTerm;
	float lastI;
}PI_Output_Data_t;
/* End local enum definitions */

/* Start Global Variables */
bool initPIController_bool = true;
static uint32_t InitRevCnt_tic = 0;
static PI_Input_Data_t PI_InputData;
static PI_Output_Data_t PI_OutputData;
/* End Global Variables */

/* Start Static function prototypes */
static int8_t signf(float x);
static int32_t BalanceInit(float RollAng_deg, uint32_t *pRevCnt);
static void PI_Resettable(PI_Input_Data_t *pInputData, PI_Output_Data_t *pOutputData);
/* End Static function prototypes */

/* Start Global Function Definitions */
CT_Balance_Bus_t CT_BalanceController(bool MotorEnable_bool, PrimaryState CurrentState_enum, float RollAng_deg, float dt){
	CT_Balance_Bus_t CT_Balance_Bus;
	int32_t MotorSpeedReq_rpm;
	uint32_t MaxMotorSpeed_rpm = 10000;
	float PIOutput_filt;

	if (initPIController_bool){
		// Initialize PI input instance values
		PI_InputData.errorGain = PI_ERROR_GAIN;
		PI_InputData.resetValue = PI_RESET_VAL;
		PI_InputData.p = PI_PTERM;
		PI_InputData.i = PI_ITERM;
		PI_InputData.iMin = -1 * PI_IMAX;
		PI_InputData.iMax = PI_IMAX;
		PI_InputData.resetValue = PI_RESET_VAL;

		// Initialize PI output instance values
		PI_OutputData.lastI = PI_RESET_VAL;

		initPIController_bool = false;
	}
	PI_InputData.setPoint = 0;
	PI_InputData.feedback = RollAng_deg;
	PI_InputData.dt = dt;

	if (!(CurrentState_enum == Balance)){
		PI_InputData.reset = true;
	} else {
		PI_InputData.reset = false;
	}

	PI_Resettable(&PI_InputData, &PI_OutputData);
	arm_biquad_cascade_df1_f32(&S_PIOutput, &PI_OutputData.output, &PIOutput_filt, blockSize);

	// Reset jump up rev timer
	if(!(CurrentState_enum == JumpUp)){
		InitRevCnt_tic = 0;
	}

	if (MotorEnable_bool){
		switch(CurrentState_enum){

		case JumpUp:
			MotorSpeedReq_rpm = BalanceInit(RollAng_deg, &InitRevCnt_tic);
			break;

		case Balance:
			MotorSpeedReq_rpm = (int32_t) PIOutput_filt;
			break;

		default:
			MotorSpeedReq_rpm = 0;
		}


		/* Speed limit */
		if (abs(MotorSpeedReq_rpm) > MaxMotorSpeed_rpm){
			MotorSpeedReq_rpm = MaxMotorSpeed_rpm * signf(MotorSpeedReq_rpm);
		}

	} else{
		MotorSpeedReq_rpm = 0;
	}

	CT_Balance_Bus.MotorSpeedReq_rpm = MotorSpeedReq_rpm;

	return CT_Balance_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static int32_t BalanceInit(float RollAng_deg, uint32_t *pRevCnt){
	int32_t MotorSpeedRequest_rpm;

	int8_t sign = -1 * signf(RollAng_deg);

	if (*pRevCnt < JUMP_UP_HOLD_CNT){
		MotorSpeedRequest_rpm = sign * JUMP_UP_MTR_SPD;
	} else{
		MotorSpeedRequest_rpm = -1 * sign * JUMP_UP_MTR_SPD;
	}

	*pRevCnt = *pRevCnt + 1;

	return MotorSpeedRequest_rpm;
}

static void PI_Resettable(PI_Input_Data_t *pInputData, PI_Output_Data_t *pOutputData){
	float error = pInputData->errorGain * (pInputData->setPoint - pInputData->feedback);

	float pTerm = pInputData->p * error;

	float iVal = pOutputData->lastI + (pInputData->dt * error); // Eulers method
	// Reset i term to reset value
	if (pInputData->reset){
		iVal= pInputData->resetValue;
	}
	float iTerm = pInputData->i * iVal;
	// Limit i term
	if (iTerm < pInputData->iMin){
		iTerm = pInputData->iMin;
	} else if (iTerm > pInputData->iMax){
		iTerm = pInputData->iMax;
	}

	// Update output instance
	pOutputData->output = pTerm + iTerm;
	pOutputData->error = error;
	pOutputData->pTerm = pTerm;
	pOutputData->iTerm = iTerm;
	pOutputData->lastI = iVal; // store for next integration step
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
