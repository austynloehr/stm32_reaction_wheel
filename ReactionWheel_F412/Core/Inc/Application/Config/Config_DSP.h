/*
 * Config_DSP.h
 *
 *  Created on: Sep 13, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONFIG_CONFIG_DSP_H_
#define INC_APPLICATION_CONFIG_CONFIG_DSP_H_

#include "main.h"

/* Start Global Defines */
#define BLOCK_SIZE			1
#define IIR_NUM_STAGES		1
#define IIR_FILT_ORDER		2
#define IIR_NUM_COEFF		IIR_FILT_ORDER * 2 + 2
#define IIR_STATE_LEN		4 * IIR_NUM_STAGES
/* End Global Defines */

/* Start Global Variables */
extern arm_biquad_casd_df1_inst_f32 S_Ax;
extern arm_biquad_casd_df1_inst_f32 S_Ay;
extern arm_biquad_casd_df1_inst_f32 S_PIDOutput;
extern float FiltAxState[IIR_STATE_LEN];
extern float FiltAyState[IIR_STATE_LEN];
extern float FiltPIDOutputState[IIR_STATE_LEN];
extern uint32_t blockSize;
/* End Global Variables */

/* Start Global Function Prototypes */
void Config_DSP(void);
/* End Global Function Prototypes */

#endif /* INC_APPLICATION_CONFIG_CONFIG_DSP_H_ */
