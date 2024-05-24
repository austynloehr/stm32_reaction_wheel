/*
 * File: ert_main.c
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

#include <stddef.h>
#include <stdio.h>            /* This example main program uses printf/fflush */
#include "PrimaryStateMachine.h"       /* Model header file */

static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */

/* '<Root>/VS_StateRequest_enum' */
static StateReq rtU_VS_StateRequest_enum;

/* '<Root>/ErrorInterrupt_bool' */
static bool rtU_ErrorInterrupt_bool;

/* '<Root>/RollAngle_deg' */
static float rtU_RollAngle_deg;

/* '<Root>/StartBalanceThreshold_deg' */
static float rtU_StartBalanceThreshold_deg;

/* '<Root>/StopBalanceThreshold_deg' */
static float rtU_StopBalanceThreshold_deg;

/* '<Root>/IMUCalComplete_bool' */
static bool rtU_IMUCalComplete_bool;

/* '<Root>/CT_CurrentState_enum' */
static PrimaryState rtY_CT_CurrentState_enum;

/* '<Root>/CT_MotorEnable_bool' */
static bool rtY_CT_MotorEnable_bool;

/* '<Root>/CT_GLEDState_enum' */
static LEDState rtY_CT_GLEDState_enum;

/* '<Root>/CT_RLEDState_enum' */
static LEDState rtY_CT_RLEDState_enum;

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(RT_MODEL *const rtM);
void rt_OneStep(RT_MODEL *const rtM)
{
  static bool OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(rtM, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  PrimaryStateMachine_step(rtM, rtU_VS_StateRequest_enum,
    rtU_ErrorInterrupt_bool, rtU_RollAngle_deg, rtU_StartBalanceThreshold_deg,
    rtU_StopBalanceThreshold_deg, rtU_IMUCalComplete_bool,
    &rtY_CT_CurrentState_enum, &rtY_CT_MotorEnable_bool, &rtY_CT_GLEDState_enum,
    &rtY_CT_RLEDState_enum);

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example main function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific. This example
 * illustrates how you do this relative to initializing the model.
 */
int main(int argc, const char *argv[])
{
  RT_MODEL *const rtM = rtMPtr;

  /* Unused arguments */
  (void)(argc);
  (void)(argv);

  /* Pack model data into RTM */
  rtM->dwork = &rtDW;

  /* Initialize model */
  PrimaryStateMachine_initialize(rtM, &rtU_VS_StateRequest_enum,
    &rtY_CT_CurrentState_enum);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 0.005 seconds (base rate of the model) here.
   * The call syntax for rt_OneStep is
   *
   *  rt_OneStep(rtM);
   */
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(rtM) == (NULL)) {
    /*  Perform application tasks here */
  }

  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
