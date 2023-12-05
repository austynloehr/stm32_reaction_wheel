/*
 * Config_DSP.c
 *
 *  Created on: Sep 13, 2023
 *      Author: aloehr
 */

#include "Config_DSP.h"

/*Start global variable definitions */
arm_biquad_casd_df1_inst_f32 S_AccelRoll;
arm_biquad_casd_df1_inst_f32 S_AccelPitch;

uint32_t blockSize = BLOCK_SIZE;

float FiltAccelRollState[IIR_STATE_LEN];
float FiltAccelPitchState[IIR_STATE_LEN];

static const float iirAccelCoeffs[IIR_NUM_COEFF] = {0.010432413371093418, 0.020864826742186836, 0.010432413371093418, 1.6909963768874425, -0.7327260303718163};
/*End global variable definitions */


void Config_DSP(void){
	arm_biquad_cascade_df1_init_f32(&S_AccelRoll, IIR_NUM_STAGES, (float32_t *) &iirAccelCoeffs[0], (float32_t *) &FiltAccelRollState[0]);
	arm_biquad_cascade_df1_init_f32(&S_AccelPitch, IIR_NUM_STAGES, (float32_t *) &iirAccelCoeffs[0], (float32_t *) &FiltAccelPitchState[0]);
}
