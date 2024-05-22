/*
 * File: StateRequest.h
 *
 * Code generated for Simulink model 'StateRequest'.
 *
 * Model version                  : 10.5
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Tue May 21 21:35:05 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. RAM efficiency
 *    2. Debugging
 *    3. Execution efficiency
 * Validation result: Not run
 */

#ifndef StateRequest_h_
#define StateRequest_h_
#ifndef StateRequest_COMMON_INCLUDES_
#define StateRequest_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "math.h"
#endif                                 /* StateRequest_COMMON_INCLUDES_ */

#include "StateRequest_types.h"
#include "SimulinkEnums.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  uint32_t temporalCounter_i1;         /* '<Root>/RequestStateMachine' */
  uint8_t is_active_c2_StateRequest;   /* '<Root>/RequestStateMachine' */
  uint8_t is_c2_StateRequest;          /* '<Root>/RequestStateMachine' */
  uint8_t is_ButtonInput;              /* '<Root>/RequestStateMachine' */
  bool DelayInput1_DSTATE;             /* '<S4>/Delay Input1' */
  bool DelayInput1_DSTATE_h;           /* '<S3>/Delay Input1' */
} DW;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char * volatile errorStatus;
  DW *dwork;
};

/* Model entry point functions */
extern void StateRequest_initialize(RT_MODEL *const rtM, PrimaryState
  *rtU_LastState_enum, StateReq *rtY_VS_StateRequest_enum);
extern void StateRequest_step(RT_MODEL *const rtM, bool rtU_ButtonPressed_bool,
  PrimaryState rtU_LastState_enum, float rtU_ShortPressTime_s, float
  rtU_LongPressTime_s, float rtU_dt, StateReq *rtY_VS_StateRequest_enum);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'StateRequest'
 * '<S1>'   : 'StateRequest/Compare To Constant'
 * '<S2>'   : 'StateRequest/Compare To Constant2'
 * '<S3>'   : 'StateRequest/Detect Increase1'
 * '<S4>'   : 'StateRequest/Detect Increase2'
 * '<S5>'   : 'StateRequest/RequestStateMachine'
 */

/*-
 * Requirements for '<Root>': StateRequest

 */
#endif                                 /* StateRequest_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
