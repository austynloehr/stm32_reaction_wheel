/*
 * File: PrimaryStateMachine.c
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

#include "PrimaryStateMachine.h"
#include "CT_PrimaryStateMachine.h"
#include "SimulinkEnums.h"
#include <stdbool.h>

/* Model step function */
void PrimaryStateMachine_step(RT_MODEL *const rtM, StateReq
  rtU_VS_StateRequest_enum, bool rtU_ErrorInterrupt_bool, float
  rtU_RollAngle_deg, float rtU_StartBalanceThreshold_deg, float
  rtU_StopBalanceThreshold_deg, bool rtU_IMUCalComplete_bool, PrimaryState
  *rtY_CT_CurrentState_enum, bool *rtY_CT_MotorEnable_bool, LEDState
  *rtY_CT_GLEDState_enum, LEDState *rtY_CT_RLEDState_enum)
{
  DW *rtDW = rtM->dwork;

  /* Outputs for Atomic SubSystem: '<Root>/CT_PrimaryStateMachine' */

  /* Inport: '<Root>/VS_StateRequest_enum' incorporates:
   *  Inport: '<Root>/ErrorInterrupt_bool'
   *  Inport: '<Root>/IMUCalComplete_bool'
   *  Inport: '<Root>/RollAngle_deg'
   *  Inport: '<Root>/StartBalanceThreshold_deg'
   *  Inport: '<Root>/StopBalanceThreshold_deg'
   *  Outport: '<Root>/CT_CurrentState_enum'
   *  Outport: '<Root>/CT_GLEDState_enum'
   *  Outport: '<Root>/CT_MotorEnable_bool'
   *  Outport: '<Root>/CT_RLEDState_enum'
   */
  CT_PrimaryStateMachine(rtU_VS_StateRequest_enum, rtU_ErrorInterrupt_bool,
    rtU_RollAngle_deg, rtU_StartBalanceThreshold_deg,
    rtU_StopBalanceThreshold_deg, rtU_IMUCalComplete_bool,
    rtY_CT_CurrentState_enum, rtY_CT_MotorEnable_bool, rtY_CT_GLEDState_enum,
    rtY_CT_RLEDState_enum, &rtDW->CT_PrimaryStateMachine_l);

  /* End of Outputs for SubSystem: '<Root>/CT_PrimaryStateMachine' */
}

/* Model initialize function */
void PrimaryStateMachine_initialize(RT_MODEL *const rtM, StateReq
  *rtU_VS_StateRequest_enum, PrimaryState *rtY_CT_CurrentState_enum, bool
  *rtY_CT_MotorEnable_bool, LEDState *rtY_CT_GLEDState_enum, LEDState
  *rtY_CT_RLEDState_enum)
{
  /* Registration code */

  /* external inputs */
  *rtU_VS_StateRequest_enum = ReqStandby;

  /* external outputs */
  *rtY_CT_CurrentState_enum = Standby;

  /* SystemInitialize for Atomic SubSystem: '<Root>/CT_PrimaryStateMachine' */

  /* SystemInitialize for Outport: '<Root>/CT_CurrentState_enum' incorporates:
   *  Outport: '<Root>/CT_GLEDState_enum'
   *  Outport: '<Root>/CT_MotorEnable_bool'
   *  Outport: '<Root>/CT_RLEDState_enum'
   */
  CT_PrimaryStateMachine_Init(rtY_CT_CurrentState_enum, rtY_CT_MotorEnable_bool,
    rtY_CT_GLEDState_enum, rtY_CT_RLEDState_enum);

  /* End of SystemInitialize for SubSystem: '<Root>/CT_PrimaryStateMachine' */
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
