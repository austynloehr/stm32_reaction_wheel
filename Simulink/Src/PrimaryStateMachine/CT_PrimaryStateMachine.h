/*
 * File: CT_PrimaryStateMachine.h
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

#ifndef CT_PrimaryStateMachine_h_
#define CT_PrimaryStateMachine_h_
#ifndef PrimaryStateMachine_COMMON_INCLUDES_
#define PrimaryStateMachine_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "math.h"
#endif                                /* PrimaryStateMachine_COMMON_INCLUDES_ */

#include "SimulinkEnums.h"

/* Block signals and states (default storage) for system '<Root>/CT_PrimaryStateMachine' */
typedef struct {
  uint8_t is_active_c3_PrimaryStateMachin;/* '<S1>/PrimaryStateMachine' */
  uint8_t is_c3_PrimaryStateMachine;   /* '<S1>/PrimaryStateMachine' */
  uint8_t is_Normal;                   /* '<S1>/PrimaryStateMachine' */
  uint8_t is_MotorEnabled;             /* '<S1>/PrimaryStateMachine' */
  bool DelayInput1_DSTATE;             /* '<S4>/Delay Input1' */
} DW_CT_PrimaryStateMachine;

extern void CT_PrimaryStateMachine_Init(PrimaryState *rty_CT_CurrentState_enum,
  bool *rty_CT_MotorEnable_bool, LEDState *rty_CT_GLEDState_enum, LEDState
  *rty_CT_RLEDState_enum);
extern void CT_PrimaryStateMachine(StateReq rtu_VS_StateRequest_enum, bool
  rtu_ErrorInterrupt_bool, float rtu_RollAngle_deg, float
  rtu_StartBalanceThreshold_deg, float rtu_StopBalanceThreshold_deg, bool
  rtu_IMUCalComplete_bool, PrimaryState *rty_CT_CurrentState_enum, bool
  *rty_CT_MotorEnable_bool, LEDState *rty_CT_GLEDState_enum, LEDState
  *rty_CT_RLEDState_enum, DW_CT_PrimaryStateMachine *localDW);

#endif                                 /* CT_PrimaryStateMachine_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
