/*
 * File: CT_PrimaryStateMachine.c
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

#include "SimulinkEnums.h"
#include <stdbool.h>
#include "CT_PrimaryStateMachine.h"
#include <stdint.h>
#include <math.h>

/* Named constants for Chart: '<S1>/PrimaryStateMachine' */
#define IN_Balance                     ((uint8_t)1U)
#define IN_CalibrateIMU                ((uint8_t)1U)
#define IN_Error                       ((uint8_t)1U)
#define IN_JumpUp                      ((uint8_t)2U)
#define IN_MotorEnabled                ((uint8_t)2U)
#define IN_NO_ACTIVE_CHILD             ((uint8_t)0U)
#define IN_Normal                      ((uint8_t)2U)
#define IN_Standby                     ((uint8_t)3U)

/* System initialize for atomic system: '<Root>/CT_PrimaryStateMachine' */
void CT_PrimaryStateMachine_Init(PrimaryState *rty_CT_CurrentState_enum, bool
  *rty_CT_MotorEnable_bool, LEDState *rty_CT_GLEDState_enum, LEDState
  *rty_CT_RLEDState_enum)
{
  /* SystemInitialize for Chart: '<S1>/PrimaryStateMachine' */
  *rty_CT_CurrentState_enum = Standby;
  *rty_CT_MotorEnable_bool = false;
  *rty_CT_GLEDState_enum = Off;
  *rty_CT_RLEDState_enum = Off;
}

/* Output and update for atomic system: '<Root>/CT_PrimaryStateMachine' */
void CT_PrimaryStateMachine(StateReq rtu_VS_StateRequest_enum, bool
  rtu_ErrorInterrupt_bool, float rtu_RollAngle_deg, float
  rtu_StartBalanceThreshold_deg, float rtu_StopBalanceThreshold_deg, bool
  rtu_IMUCalComplete_bool, PrimaryState *rty_CT_CurrentState_enum, bool
  *rty_CT_MotorEnable_bool, LEDState *rty_CT_GLEDState_enum, LEDState
  *rty_CT_RLEDState_enum, DW_CT_PrimaryStateMachine *localDW)
{
  bool rtb_Compare;

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   */
  rtb_Compare = (rtu_VS_StateRequest_enum == ReqBalance);

  /* Chart: '<S1>/PrimaryStateMachine' incorporates:
   *  Constant: '<S3>/Constant'
   *  RelationalOperator: '<S3>/Compare'
   *  RelationalOperator: '<S4>/FixPt Relational Operator'
   *  UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if (localDW->is_active_c3_PrimaryStateMachin == 0U) {
    localDW->is_active_c3_PrimaryStateMachin = 1U;
    localDW->is_c3_PrimaryStateMachine = IN_Normal;
    localDW->is_Normal = IN_Standby;
    *rty_CT_MotorEnable_bool = false;
    *rty_CT_CurrentState_enum = Standby;
    *rty_CT_RLEDState_enum = Off;
    *rty_CT_GLEDState_enum = On;
  } else if (localDW->is_c3_PrimaryStateMachine == IN_Error) {
    *rty_CT_CurrentState_enum = Error;
    *rty_CT_RLEDState_enum = On;
    *rty_CT_GLEDState_enum = Off;
    if (!rtu_ErrorInterrupt_bool) {
      localDW->is_c3_PrimaryStateMachine = IN_Normal;
      localDW->is_Normal = IN_Standby;
      *rty_CT_MotorEnable_bool = false;
      *rty_CT_CurrentState_enum = Standby;
      *rty_CT_RLEDState_enum = Off;
      *rty_CT_GLEDState_enum = On;
    }

    /* case IN_Normal: */
  } else if (rtu_ErrorInterrupt_bool) {
    if (localDW->is_Normal == IN_MotorEnabled) {
      localDW->is_MotorEnabled = IN_NO_ACTIVE_CHILD;
      localDW->is_Normal = IN_NO_ACTIVE_CHILD;
    } else {
      localDW->is_Normal = IN_NO_ACTIVE_CHILD;
    }

    localDW->is_c3_PrimaryStateMachine = IN_Error;
    *rty_CT_MotorEnable_bool = false;
    *rty_CT_CurrentState_enum = Error;
    *rty_CT_RLEDState_enum = On;
    *rty_CT_GLEDState_enum = Off;
  } else {
    switch (localDW->is_Normal) {
     case IN_CalibrateIMU:
      *rty_CT_CurrentState_enum = CalibrateIMU;
      *rty_CT_RLEDState_enum = Blink;
      *rty_CT_GLEDState_enum = Blink;
      if ((int32_t)rtu_IMUCalComplete_bool > (int32_t)
          localDW->DelayInput1_DSTATE) {
        localDW->is_Normal = IN_Standby;
        *rty_CT_MotorEnable_bool = false;
        *rty_CT_CurrentState_enum = Standby;
        *rty_CT_RLEDState_enum = Off;
        *rty_CT_GLEDState_enum = On;
      }
      break;

     case IN_MotorEnabled:
      *rty_CT_RLEDState_enum = Off;
      *rty_CT_GLEDState_enum = Blink;
      if (!rtb_Compare) {
        localDW->is_MotorEnabled = IN_NO_ACTIVE_CHILD;
        localDW->is_Normal = IN_Standby;
        *rty_CT_MotorEnable_bool = false;
        *rty_CT_CurrentState_enum = Standby;
        *rty_CT_RLEDState_enum = Off;
        *rty_CT_GLEDState_enum = On;
      } else if (localDW->is_MotorEnabled == IN_Balance) {
        *rty_CT_CurrentState_enum = Balance;
        if (fabsf(rtu_RollAngle_deg) > rtu_StopBalanceThreshold_deg) {
          localDW->is_MotorEnabled = IN_JumpUp;
          *rty_CT_CurrentState_enum = JumpUp;
        }
      } else {
        /* case IN_JumpUp: */
        *rty_CT_CurrentState_enum = JumpUp;
        if (fabsf(rtu_RollAngle_deg) < rtu_StartBalanceThreshold_deg) {
          localDW->is_MotorEnabled = IN_Balance;
          *rty_CT_CurrentState_enum = Balance;
        }
      }
      break;

     default:
      /* case IN_Standby: */
      *rty_CT_CurrentState_enum = Standby;
      *rty_CT_RLEDState_enum = Off;
      *rty_CT_GLEDState_enum = On;
      if (rtb_Compare) {
        localDW->is_Normal = IN_MotorEnabled;
        *rty_CT_MotorEnable_bool = true;
        *rty_CT_RLEDState_enum = Off;
        *rty_CT_GLEDState_enum = Blink;
        localDW->is_MotorEnabled = IN_JumpUp;
        *rty_CT_CurrentState_enum = JumpUp;
      } else if (rtu_VS_StateRequest_enum == ReqCalibrateIMU) {
        localDW->is_Normal = IN_CalibrateIMU;
        *rty_CT_CurrentState_enum = CalibrateIMU;
        *rty_CT_RLEDState_enum = Blink;
        *rty_CT_GLEDState_enum = Blink;
      }
      break;
    }
  }

  /* End of Chart: '<S1>/PrimaryStateMachine' */

  /* Update for UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_IMUCalComplete_bool;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
