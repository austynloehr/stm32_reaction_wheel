/*
 * CT_BalanceController.c
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */
#include "CT_BalanceController.h"
#include "Config_DSP.h"
#include <math.h>

/* Start Defines */
// Motor limits
#define MAX_MTR_SPD 10000
#define MAX_MTR_CURRENT 20000

// Jump up cals
#define JUMP_UP_MTR_SPD 4100
#define JUMP_UP_HOLD_CNT 30
#define JUMP_UP_TIMEOUT 60

// PID cals
#define PID_PTERM 1000
#define PID_ITERM 0
#define PID_DTERM 100
#define PID_IMAX 0
#define PID_RESET_VAL 0
#define PID_ERROR_GAIN -1
#define MOTOR_SPEED_GAIN 2
#define SIZE_SPEED_TBL 3

// Control loop rate
#define CT_BAL_FcnCallDivider 5  // Cycles
#define CT_BAL_MaxFcnCallCnt 100
/* End Defines */

/* Start local enum definitions */
typedef struct PID_Input_Data{
	float setPoint;
	float feedback;
	float errorGain;
	float p;
	float i;
	float d;
	bool dFiltEnable;
	float iMin;
	float iMax;
	bool iReset;
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
static float LookupTable1D(float x, const float* xValues, const float* yValues, uint32_t tableSize);
static int32_t JumpUpControl(bool Reset, float RollAng_deg);
static void PID_Resettable(PID_Input_Data_t *pInputData, PID_Output_Data_t *pOutputData);
/* End Static function prototypes */

/* Start Global Function Definitions */
CT_Balance_Bus_t CT_BalanceController(bool MotorEnable_bool, PrimaryState CurrentState_enum, float RollAng_deg, float dt, int32_t MotorSpeed_rpm){
	static PID_Input_Data_t PID_InputData;
	static CT_Balance_Bus_t CT_Balance_Bus;
	static uint16_t FcnCallCnt = 0;
	static bool OneShot_bool = true;

	// Look up tables
	static const float MtrSpeedTermTblIdx[SIZE_SPEED_TBL] = {-3, 0, 3};  // Roll angle (deg)
	static const float MtrSpeedTermTblGain[SIZE_SPEED_TBL] = {2.5, 2.75, 2.5};  // Motor speed gain

	bool JumpUpReset_bool;
	PID_Output_Data_t PID_OutputData;
	int32_t MotorRequest;
	int32_t MotorCurrentReq_mA;
	int32_t MotorSpeedReq_rpm;
	int32_t MotorSpeedControl_mA;
	float MotorSpeedGain_na;

	// Execute once
	if (OneShot_bool){
		// Initialize PI input instance values
		PID_InputData.errorGain = PID_ERROR_GAIN;
		PID_InputData.resetValue = PID_RESET_VAL;
		PID_InputData.dFiltEnable = false;
		PID_InputData.setPoint = -0.55;
		PID_InputData.p = PID_PTERM;
		PID_InputData.i = PID_ITERM;
		PID_InputData.d = PID_DTERM;
		PID_InputData.iReset = false;
		PID_InputData.iMin = -1 * PID_IMAX;
		PID_InputData.iMax = PID_IMAX;
		PID_InputData.resetValue = PID_RESET_VAL;

		OneShot_bool = false;
	}

	// Execute every 5 cycles to sync with VESC input data
	if (FcnCallCnt % CT_BAL_FcnCallDivider == 0){

		/* --------- PID CONTROL ---------  */
		// Update PID inputs
		PID_InputData.dt = dt * CT_BAL_FcnCallDivider;;
		PID_InputData.feedback = RollAng_deg;;

		// Control loop
		MotorSpeedGain_na = LookupTable1D(RollAng_deg, MtrSpeedTermTblIdx, MtrSpeedTermTblGain, SIZE_SPEED_TBL);
		MotorSpeedControl_mA = MotorSpeedGain_na * MotorSpeed_rpm;
		PID_Resettable(&PID_InputData, &PID_OutputData);
		MotorCurrentReq_mA = ((int32_t) floor(PID_OutputData.output)) + MotorSpeedControl_mA;

		/* --------- JUMP UP CONTROL ---------  */
		JumpUpReset_bool = !(CurrentState_enum == JumpUp);  // Hold process in reset until in JumpUp state
		MotorSpeedReq_rpm = JumpUpControl(JumpUpReset_bool, RollAng_deg);

		/* --------- MOTOR REQUEST ---------  */
		if (MotorEnable_bool){
			switch(CurrentState_enum){

			case JumpUp:
				CT_Balance_Bus.ControlMode = Speed;
				MotorRequest = MotorSpeedReq_rpm;

				/* Speed limit */
				if (abs(MotorRequest) > MAX_MTR_SPD){
					MotorRequest = MAX_MTR_SPD * signf(MotorRequest);
				}
				break;

			case Balance:
				CT_Balance_Bus.ControlMode = Current;
				MotorRequest = MotorCurrentReq_mA;

				/*Current limit*/
				if (abs(MotorRequest) > MAX_MTR_CURRENT){
					MotorRequest = MAX_MTR_CURRENT * signf(MotorRequest);
				}
				break;

			default:
				CT_Balance_Bus.ControlMode = Current;
				MotorRequest = 0;
			}

		} else{
			CT_Balance_Bus.ControlMode = Current;
			MotorRequest = 0;
		}

		// Store other information for logging
		CT_Balance_Bus.MotorRequest_na = MotorRequest;
		CT_Balance_Bus.PIDTerm_mA = (int32_t) floor(PID_OutputData.output);
		CT_Balance_Bus.MotorSpeedTerm_mA = MotorSpeedControl_mA;
		CT_Balance_Bus.pTerm_mA = (int32_t) floor(PID_OutputData.pTerm);
		CT_Balance_Bus.iTerm_mA = (int32_t) floor(PID_OutputData.iTerm);
		CT_Balance_Bus.dTerm_mA = (int32_t) floor(PID_OutputData.dTerm);
	}

	// Rollover counter to prevent overrun
	if (FcnCallCnt >= CT_BAL_MaxFcnCallCnt){
		FcnCallCnt = 0;
	}
	FcnCallCnt++;

	return CT_Balance_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static int32_t JumpUpControl(bool Reset, float RollAng_deg){
	// The purpose of this function is to jump the robot up to balance point
	// Using speed control, spin motor in one direction for some time period, then flip direction
	// Robot will keep trying jump up until balance point is reached

	int32_t MotorSpeedRequest_rpm;
	static uint32_t JumpUpTimer_cnt = 0;
	static uint32_t JumpUpTimeout_cnt = 0;

	int8_t sign = -1 * signf(RollAng_deg);  // Get sign of roll angle

	if (JumpUpTimer_cnt < JUMP_UP_HOLD_CNT){
		// Spin motor for some time period
		MotorSpeedRequest_rpm = sign * JUMP_UP_MTR_SPD;
		JumpUpTimeout_cnt = 0; // Not timed out yet
	} else{
		// Try to jump up, spin motor in oppose direction
		// Keep spinning for some timeout period
		MotorSpeedRequest_rpm = -1 * sign * JUMP_UP_MTR_SPD;
		JumpUpTimeout_cnt += 1;
	}

	if((Reset) || (JumpUpTimeout_cnt >= JUMP_UP_TIMEOUT)){
		// Reset requested or jump up timeout reached
		// Retry process
		JumpUpTimer_cnt = 0;
	} else{
		// Jump up in progress
		JumpUpTimer_cnt = JumpUpTimer_cnt + 1;
	}

	return MotorSpeedRequest_rpm;
}

static void PID_Resettable(PID_Input_Data_t *pInputData, PID_Output_Data_t *pOutputData){
	// PID controller with integrator reset and derivative filter
	float iVal;
	float dVal;
	float dValFilt;
	static float lastFeedback = 0;
	static float lastIVal = PID_RESET_VAL;
	static bool iTermLimited = false;
	float error;
	float pTerm;
	float iTerm;
	float dTerm;

	error = pInputData->errorGain * (pInputData->setPoint - pInputData->feedback);

	pTerm = pInputData->p * error;

	// Eulers method for integration
	if (iTermLimited){
		iVal = lastIVal; // Stop integrating
	} else {
		iVal = lastIVal + (pInputData->dt * error);
	}

	// Reset integrator to reset value
	if (pInputData->iReset){
		iVal= pInputData->resetValue;
	}

	lastIVal = iVal; // store for next integration step

	iTerm = pInputData->i * iVal;
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

	dVal = (pInputData->feedback - lastFeedback) / pInputData->dt;

	arm_biquad_cascade_df1_f32(&S_PIDOutput, &dVal, &dValFilt, BLOCK_SIZE);

	if (pInputData->dFiltEnable){
		dVal = dValFilt;
	}

	dTerm = dVal * pInputData->d;

	lastFeedback = pInputData->feedback;

	// Update output instance
	pOutputData->output = pTerm + iTerm + dTerm;
	pOutputData->error = error;
	pOutputData->pTerm = pTerm;
	pOutputData->iTerm = iTerm;
	pOutputData->dTerm = dTerm;
}

static float LookupTable1D(float x, const float* xValues, const float* yValues, uint32_t tableSize){
    // Find the two closest x values in the table
    if (x <= xValues[0]) {
        return yValues[0];  // If x is below the first table value, return the first y value
    }

    if (x >= xValues[tableSize - 1]) {
        return yValues[tableSize - 1];  // If x is above the last table value, return the last y value
    }

    for (int32_t i = 0; i < tableSize - 1; i++) {
        if (x >= xValues[i] && x < xValues[i + 1]) {
            // Perform linear interpolation
            float x0 = xValues[i];
            float x1 = xValues[i + 1];
            float y0 = yValues[i];
            float y1 = yValues[i + 1];

            // Formula for linear interpolation: y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
            float interpolated_y = y0 + (x - x0) * (y1 - y0) / (x1 - x0);
            return interpolated_y;
        }
    }

    return 0.0;  // This should never happen due to bounds checking above
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
