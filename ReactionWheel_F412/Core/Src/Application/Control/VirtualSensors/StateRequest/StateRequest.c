/*
 * File: StateRequest.c
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

#include "StateRequest.h"
#include <stdint.h>
#include <stdbool.h>
#include "SimulinkEnums.h"

/* Named constants for Chart: '<Root>/RequestStateMachine' */
#define IN_Balance                     ((uint8_t)1U)
#define IN_ButtonInput                 ((uint8_t)1U)
#define IN_CalibrateIMU                ((uint8_t)2U)
#define IN_GoToStandby                 ((uint8_t)2U)
#define IN_NO_ACTIVE_CHILD             ((uint8_t)0U)
#define IN_NoCommand                   ((uint8_t)3U)
#define IN_Standby                     ((uint8_t)3U)

/* Model step function */
void StateRequest_step(RT_MODEL *const rtM, bool rtU_ButtonPressed_bool,
  PrimaryState rtU_LastState_enum, float rtU_ShortPressTime_s, float
  rtU_LongPressTime_s, float rtU_dt, StateReq *rtY_VS_StateRequest_enum)
{
  DW *rtDW = rtM->dwork;
  uint32_t rtb_DataTypeConversion1;
  uint32_t rtb_DataTypeConversion2;
  StateReq tmp;
  bool rtb_Compare;
  bool rtb_Compare_n;
  bool rtb_LogicalOperator2;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/LastState_enum'
   */
  rtb_Compare = (rtU_LastState_enum == Error);

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/LastState_enum'
   */
  rtb_Compare_n = (rtU_LastState_enum == CalibrateIMU);

  /* Logic: '<Root>/Logical Operator2' incorporates:
   *  RelationalOperator: '<S3>/FixPt Relational Operator'
   *  RelationalOperator: '<S4>/FixPt Relational Operator'
   *  UnitDelay: '<S3>/Delay Input1'
   *  UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S3>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_LogicalOperator2 = (((int32_t)rtb_Compare > (int32_t)
    rtDW->DelayInput1_DSTATE) || ((int32_t)rtb_Compare_n > (int32_t)
    rtDW->DelayInput1_DSTATE_h));

  /* DataTypeConversion: '<Root>/Data Type Conversion2' incorporates:
   *  Inport: '<Root>/ShortPressTime_s'
   *  Inport: '<Root>/dt'
   *  Product: '<Root>/Divide2'
   */
  rtb_DataTypeConversion2 = (uint32_t)(rtU_ShortPressTime_s / rtU_dt);

  /* DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
   *  Inport: '<Root>/LongPressTime_s'
   *  Inport: '<Root>/dt'
   *  Product: '<Root>/Divide1'
   */
  rtb_DataTypeConversion1 = (uint32_t)(rtU_LongPressTime_s / rtU_dt);

  /* Chart: '<Root>/RequestStateMachine' incorporates:
   *  Inport: '<Root>/ButtonPressed_bool'
   *  Outport: '<Root>/VS_StateRequest_enum'
   */
  if (rtDW->temporalCounter_i1 < UINT32_MAX) {
    rtDW->temporalCounter_i1++;
  }

  if (rtDW->is_active_c2_StateRequest == 0U) {
    rtDW->is_active_c2_StateRequest = 1U;

    /* Outport: '<Root>/VS_StateRequest_enum' */
    *rtY_VS_StateRequest_enum = ReqStandby;
    rtDW->is_c2_StateRequest = IN_NoCommand;
  } else {
    switch (rtDW->is_c2_StateRequest) {
     case IN_ButtonInput:
      if (rtb_LogicalOperator2) {
        rtDW->is_ButtonInput = IN_NO_ACTIVE_CHILD;
        rtDW->is_c2_StateRequest = IN_GoToStandby;

        /* Outport: '<Root>/VS_StateRequest_enum' */
        *rtY_VS_StateRequest_enum = ReqStandby;
      } else {
        switch (rtDW->is_ButtonInput) {
         case IN_Balance:
          if (rtDW->temporalCounter_i1 >= rtb_DataTypeConversion1) {
            rtDW->is_ButtonInput = IN_CalibrateIMU;
          } else if ((rtDW->temporalCounter_i1 >= rtb_DataTypeConversion2) &&
                     (!rtU_ButtonPressed_bool)) {
            /* Outport: '<Root>/VS_StateRequest_enum' */
            *rtY_VS_StateRequest_enum = ReqStandby;
            rtDW->is_ButtonInput = IN_NO_ACTIVE_CHILD;
            rtDW->is_c2_StateRequest = IN_NoCommand;
          }
          break;

         case IN_CalibrateIMU:
          if (!rtU_ButtonPressed_bool) {
            /* Outport: '<Root>/VS_StateRequest_enum' */
            *rtY_VS_StateRequest_enum = ReqCalibrateIMU;
            rtDW->is_ButtonInput = IN_NO_ACTIVE_CHILD;
            rtDW->is_c2_StateRequest = IN_NoCommand;
          }
          break;

         default:
          /* case IN_Standby: */
          if (rtDW->temporalCounter_i1 >= rtb_DataTypeConversion1) {
            rtDW->is_ButtonInput = IN_CalibrateIMU;
          } else if ((rtDW->temporalCounter_i1 >= rtb_DataTypeConversion2) &&
                     (!rtU_ButtonPressed_bool)) {
            /* Outport: '<Root>/VS_StateRequest_enum' */
            *rtY_VS_StateRequest_enum = ReqBalance;
            rtDW->is_ButtonInput = IN_NO_ACTIVE_CHILD;
            rtDW->is_c2_StateRequest = IN_NoCommand;
          }
          break;
        }
      }
      break;

     case IN_GoToStandby:
      if (!rtb_LogicalOperator2) {
        rtDW->is_c2_StateRequest = IN_NoCommand;
      } else {
        /* Outport: '<Root>/VS_StateRequest_enum' */
        *rtY_VS_StateRequest_enum = ReqStandby;
      }
      break;

     default:
      /* case IN_NoCommand: */
      if (rtb_LogicalOperator2) {
        rtDW->is_c2_StateRequest = IN_GoToStandby;

        /* Outport: '<Root>/VS_StateRequest_enum' */
        *rtY_VS_StateRequest_enum = ReqStandby;
      } else {
        tmp = *rtY_VS_StateRequest_enum;
        if (rtU_ButtonPressed_bool && (tmp == ReqStandby)) {
          rtDW->is_c2_StateRequest = IN_ButtonInput;
          rtDW->temporalCounter_i1 = 0U;
          rtDW->is_ButtonInput = IN_Standby;
        } else if (rtU_ButtonPressed_bool && (tmp == ReqBalance)) {
          rtDW->is_c2_StateRequest = IN_ButtonInput;
          rtDW->temporalCounter_i1 = 0U;
          rtDW->is_ButtonInput = IN_Balance;
        }
      }
      break;
    }
  }

  /* End of Chart: '<Root>/RequestStateMachine' */

  /* Update for UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtDW->DelayInput1_DSTATE = rtb_Compare;

  /* Update for UnitDelay: '<S3>/Delay Input1'
   *
   * Block description for '<S3>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtDW->DelayInput1_DSTATE_h = rtb_Compare_n;
}

/* Model initialize function */
void StateRequest_initialize(RT_MODEL *const rtM, PrimaryState
  *rtU_LastState_enum, StateReq *rtY_VS_StateRequest_enum)
{
  /* Registration code */

  /* external inputs */
  *rtU_LastState_enum = Standby;

  /* external outputs */
  *rtY_VS_StateRequest_enum = ReqStandby;

  /* SystemInitialize for Outport: '<Root>/VS_StateRequest_enum' incorporates:
   *  Chart: '<Root>/RequestStateMachine'
   */
  *rtY_VS_StateRequest_enum = ReqStandby;
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
