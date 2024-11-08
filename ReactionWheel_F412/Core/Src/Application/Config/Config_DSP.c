/*
 * Config_DSP.c
 *
 *  Created on: Sep 13, 2023
 *      Author: aloehr
 */

#include "Config_DSP.h"

/*Start global variable definitions */
arm_biquad_casd_df1_inst_f32 S_Ax;
arm_biquad_casd_df1_inst_f32 S_Ay;
arm_biquad_casd_df1_inst_f32 S_PIOutput;

uint32_t blockSize = BLOCK_SIZE;

float FiltAxState[IIR_STATE_LEN];
float FiltAyState[IIR_STATE_LEN];
float FiltPIOutputState[IIR_STATE_LEN];

static const float iirAccelCoeffs[IIR_NUM_COEFF] = {0.013359200027856505, 0.02671840005571301, 0.013359200027856505, 1.6474599810769768, -0.7008967811884027};
static const float iirPICoeffs[IIR_NUM_COEFF] = {0.010432413371093418, 0.020864826742186836, 0.010432413371093418, 1.6909963768874425, -0.7327260303718163};
/*End global variable definitions */


void Config_DSP(void){
	arm_biquad_cascade_df1_init_f32(&S_Ax, IIR_NUM_STAGES, (float32_t *) &iirAccelCoeffs[0], (float32_t *) &FiltAxState[0]);
	arm_biquad_cascade_df1_init_f32(&S_Ay, IIR_NUM_STAGES, (float32_t *) &iirAccelCoeffs[0], (float32_t *) &FiltAyState[0]);
	arm_biquad_cascade_df1_init_f32(&S_PIOutput, IIR_NUM_STAGES, (float32_t *) &iirPICoeffs[0], (float32_t *) &FiltPIOutputState[0]);
}
