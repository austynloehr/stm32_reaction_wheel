/*
 * File: PrimaryStateMachine.h
 *
 * Code generated for Simulink model 'PrimaryStateMachine'.
 *
 * Model version                  : 10.9
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Tue May 21 21:37:03 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef PrimaryStateMachine_h_
#define PrimaryStateMachine_h_
#ifndef PrimaryStateMachine_COMMON_INCLUDES_
#define PrimaryStateMachine_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "math.h"
#endif                                /* PrimaryStateMachine_COMMON_INCLUDES_ */

#include "PrimaryStateMachine_types.h"
#include "SimulinkEnums.h"
#include "CT_PrimaryStateMachine.h"
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
  DW_CT_PrimaryStateMachine CT_PrimaryStateMachine_l;/* '<Root>/CT_PrimaryStateMachine' */
} DW;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char * volatile errorStatus;
  DW *dwork;
};

/* Model entry point functions */
extern void PrimaryStateMachine_initialize(RT_MODEL *const rtM, StateReq
  *rtU_VS_StateRequest_enum, PrimaryState *rtY_CT_CurrentState_enum, bool
  *rtY_CT_MotorEnable_bool, LEDState *rtY_CT_GLEDState_enum, LEDState
  *rtY_CT_RLEDState_enum);
extern void PrimaryStateMachine_step(RT_MODEL *const rtM, StateReq
  rtU_VS_StateRequest_enum, bool rtU_ErrorInterrupt_bool, float
  rtU_RollAngle_deg, float rtU_StartBalanceThreshold_deg, float
  rtU_StopBalanceThreshold_deg, bool rtU_IMUCalComplete_bool, PrimaryState
  *rtY_CT_CurrentState_enum, bool *rtY_CT_MotorEnable_bool, LEDState
  *rtY_CT_GLEDState_enum, LEDState *rtY_CT_RLEDState_enum);

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
 * '<Root>' : 'PrimaryStateMachine'
 * '<S1>'   : 'PrimaryStateMachine/CT_PrimaryStateMachine'
 * '<S2>'   : 'PrimaryStateMachine/CT_PrimaryStateMachine/Compare To Constant'
 * '<S3>'   : 'PrimaryStateMachine/CT_PrimaryStateMachine/Compare To Constant1'
 * '<S4>'   : 'PrimaryStateMachine/CT_PrimaryStateMachine/Detect Increase'
 * '<S5>'   : 'PrimaryStateMachine/CT_PrimaryStateMachine/PrimaryStateMachine'
 */

/*-
 * Requirements for '<Root>': PrimaryStateMachine

 */
#endif                                 /* PrimaryStateMachine_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
