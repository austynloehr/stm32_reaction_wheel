/*
 * File: PrimaryStateMachine.c
 *
 * Code generated for Simulink model 'PrimaryStateMachine'.
 *
 * Model version                  : 10.16
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Fri May 24 00:55:26 2024
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
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "SimulinkEnums.h"

/* Named constants for Chart: '<Root>/PrimaryStateMachine' */
#define IN_Balance                     ((uint8_t)1U)
#define IN_CalibrateIMU                ((uint8_t)1U)
#define IN_Error                       ((uint8_t)1U)
#define IN_JumpUp                      ((uint8_t)2U)
#define IN_MotorEnabled                ((uint8_t)2U)
#define IN_NO_ACTIVE_CHILD             ((uint8_t)0U)
#define IN_Normal                      ((uint8_t)2U)
#define IN_Standby                     ((uint8_t)3U)

/* Model step function */
void PrimaryStateMachine_step(RT_MODEL *const rtM, StateReq
  rtU_VS_StateRequest_enum, bool rtU_ErrorInterrupt_bool, float
  rtU_RollAngle_deg, float rtU_StartBalanceThreshold_deg, float
  rtU_StopBalanceThreshold_deg, bool rtU_IMUCalComplete_bool, PrimaryState
  *rtY_CT_CurrentState_enum, bool *rtY_CT_MotorEnable_bool, LEDState
  *rtY_CT_GLEDState_enum, LEDState *rtY_CT_RLEDState_enum)
{
  DW *rtDW = rtM->dwork;
  bool rtb_Compare;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/VS_StateRequest_enum'
   */
  rtb_Compare = (rtU_VS_StateRequest_enum == ReqBalance);

  /* Chart: '<Root>/PrimaryStateMachine' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/ErrorInterrupt_bool'
   *  Inport: '<Root>/IMUCalComplete_bool'
   *  Inport: '<Root>/RollAngle_deg'
   *  Inport: '<Root>/StartBalanceThreshold_deg'
   *  Inport: '<Root>/StopBalanceThreshold_deg'
   *  Inport: '<Root>/VS_StateRequest_enum'
   *  RelationalOperator: '<S2>/Compare'
   *  RelationalOperator: '<S3>/FixPt Relational Operator'
   *  UnitDelay: '<S3>/Delay Input1'
   *
   * Block description for '<S3>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if (rtDW->is_active_c3_PrimaryStateMachin == 0U) {
    rtDW->is_active_c3_PrimaryStateMachin = 1U;
    rtDW->is_c3_PrimaryStateMachine = IN_Normal;
    rtDW->is_Normal = IN_Standby;

    /* Outport: '<Root>/CT_MotorEnable_bool' */
    *rtY_CT_MotorEnable_bool = false;

    /* Outport: '<Root>/CT_CurrentState_enum' */
    *rtY_CT_CurrentState_enum = Standby;

    /* Outport: '<Root>/CT_RLEDState_enum' */
    *rtY_CT_RLEDState_enum = Off;

    /* Outport: '<Root>/CT_GLEDState_enum' */
    *rtY_CT_GLEDState_enum = On;
  } else if (rtDW->is_c3_PrimaryStateMachine == IN_Error) {
    /* Outport: '<Root>/CT_CurrentState_enum' */
    *rtY_CT_CurrentState_enum = Error;

    /* Outport: '<Root>/CT_RLEDState_enum' */
    *rtY_CT_RLEDState_enum = On;

    /* Outport: '<Root>/CT_GLEDState_enum' */
    *rtY_CT_GLEDState_enum = Off;
    if (!rtU_ErrorInterrupt_bool) {
      rtDW->is_c3_PrimaryStateMachine = IN_Normal;
      rtDW->is_Normal = IN_Standby;

      /* Outport: '<Root>/CT_MotorEnable_bool' */
      *rtY_CT_MotorEnable_bool = false;

      /* Outport: '<Root>/CT_CurrentState_enum' */
      *rtY_CT_CurrentState_enum = Standby;

      /* Outport: '<Root>/CT_RLEDState_enum' */
      *rtY_CT_RLEDState_enum = Off;

      /* Outport: '<Root>/CT_GLEDState_enum' */
      *rtY_CT_GLEDState_enum = On;
    }

    /* case IN_Normal: */
  } else if (rtU_ErrorInterrupt_bool) {
    if (rtDW->is_Normal == IN_MotorEnabled) {
      rtDW->is_MotorEnabled = IN_NO_ACTIVE_CHILD;
      rtDW->is_Normal = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW->is_Normal = IN_NO_ACTIVE_CHILD;
    }

    rtDW->is_c3_PrimaryStateMachine = IN_Error;

    /* Outport: '<Root>/CT_MotorEnable_bool' */
    *rtY_CT_MotorEnable_bool = false;

    /* Outport: '<Root>/CT_CurrentState_enum' */
    *rtY_CT_CurrentState_enum = Error;

    /* Outport: '<Root>/CT_RLEDState_enum' */
    *rtY_CT_RLEDState_enum = On;

    /* Outport: '<Root>/CT_GLEDState_enum' */
    *rtY_CT_GLEDState_enum = Off;
  } else {
    switch (rtDW->is_Normal) {
     case IN_CalibrateIMU:
      /* Outport: '<Root>/CT_CurrentState_enum' */
      *rtY_CT_CurrentState_enum = CalibrateIMU;

      /* Outport: '<Root>/CT_RLEDState_enum' */
      *rtY_CT_RLEDState_enum = Blink;

      /* Outport: '<Root>/CT_GLEDState_enum' */
      *rtY_CT_GLEDState_enum = Blink;
      if ((int32_t)rtU_IMUCalComplete_bool > (int32_t)rtDW->DelayInput1_DSTATE)
      {
        rtDW->is_Normal = IN_Standby;

        /* Outport: '<Root>/CT_MotorEnable_bool' */
        *rtY_CT_MotorEnable_bool = false;

        /* Outport: '<Root>/CT_CurrentState_enum' */
        *rtY_CT_CurrentState_enum = Standby;

        /* Outport: '<Root>/CT_RLEDState_enum' */
        *rtY_CT_RLEDState_enum = Off;

        /* Outport: '<Root>/CT_GLEDState_enum' */
        *rtY_CT_GLEDState_enum = On;
      }
      break;

     case IN_MotorEnabled:
      /* Outport: '<Root>/CT_RLEDState_enum' */
      *rtY_CT_RLEDState_enum = Off;

      /* Outport: '<Root>/CT_GLEDState_enum' */
      *rtY_CT_GLEDState_enum = Blink;
      if (!rtb_Compare) {
        rtDW->is_MotorEnabled = IN_NO_ACTIVE_CHILD;
        rtDW->is_Normal = IN_Standby;

        /* Outport: '<Root>/CT_MotorEnable_bool' */
        *rtY_CT_MotorEnable_bool = false;

        /* Outport: '<Root>/CT_CurrentState_enum' */
        *rtY_CT_CurrentState_enum = Standby;

        /* Outport: '<Root>/CT_GLEDState_enum' */
        *rtY_CT_GLEDState_enum = On;
      } else if (rtDW->is_MotorEnabled == IN_Balance) {
        /* Outport: '<Root>/CT_CurrentState_enum' */
        *rtY_CT_CurrentState_enum = Balance;
        if (fabsf(rtU_RollAngle_deg) > rtU_StopBalanceThreshold_deg) {
          rtDW->is_MotorEnabled = IN_JumpUp;

          /* Outport: '<Root>/CT_CurrentState_enum' */
          *rtY_CT_CurrentState_enum = JumpUp;
        }
      } else {
        /* Outport: '<Root>/CT_CurrentState_enum' */
        /* case IN_JumpUp: */
        *rtY_CT_CurrentState_enum = JumpUp;
        if (fabsf(rtU_RollAngle_deg) < rtU_StartBalanceThreshold_deg) {
          rtDW->is_MotorEnabled = IN_Balance;

          /* Outport: '<Root>/CT_CurrentState_enum' */
          *rtY_CT_CurrentState_enum = Balance;
        }
      }
      break;

     default:
      /* Outport: '<Root>/CT_CurrentState_enum' */
      /* case IN_Standby: */
      *rtY_CT_CurrentState_enum = Standby;

      /* Outport: '<Root>/CT_RLEDState_enum' */
      *rtY_CT_RLEDState_enum = Off;

      /* Outport: '<Root>/CT_GLEDState_enum' */
      *rtY_CT_GLEDState_enum = On;
      if (rtb_Compare) {
        rtDW->is_Normal = IN_MotorEnabled;

        /* Outport: '<Root>/CT_MotorEnable_bool' */
        *rtY_CT_MotorEnable_bool = true;

        /* Outport: '<Root>/CT_GLEDState_enum' */
        *rtY_CT_GLEDState_enum = Blink;
        rtDW->is_MotorEnabled = IN_JumpUp;

        /* Outport: '<Root>/CT_CurrentState_enum' */
        *rtY_CT_CurrentState_enum = JumpUp;
      } else if (rtU_VS_StateRequest_enum == ReqCalibrateIMU) {
        rtDW->is_Normal = IN_CalibrateIMU;

        /* Outport: '<Root>/CT_CurrentState_enum' */
        *rtY_CT_CurrentState_enum = CalibrateIMU;

        /* Outport: '<Root>/CT_RLEDState_enum' */
        *rtY_CT_RLEDState_enum = Blink;

        /* Outport: '<Root>/CT_GLEDState_enum' */
        *rtY_CT_GLEDState_enum = Blink;
      }
      break;
    }
  }

  /* End of Chart: '<Root>/PrimaryStateMachine' */

  /* Update for UnitDelay: '<S3>/Delay Input1' incorporates:
   *  Inport: '<Root>/IMUCalComplete_bool'
   *
   * Block description for '<S3>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtDW->DelayInput1_DSTATE = rtU_IMUCalComplete_bool;
}

/* Model initialize function */
void PrimaryStateMachine_initialize(RT_MODEL *const rtM, StateReq
  *rtU_VS_StateRequest_enum, PrimaryState *rtY_CT_CurrentState_enum)
{
  /* Registration code */

  /* external inputs */
  *rtU_VS_StateRequest_enum = ReqStandby;

  /* external outputs */
  *rtY_CT_CurrentState_enum = Standby;

  /* SystemInitialize for Outport: '<Root>/CT_CurrentState_enum' incorporates:
   *  Chart: '<Root>/PrimaryStateMachine'
   */
  *rtY_CT_CurrentState_enum = Standby;
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
