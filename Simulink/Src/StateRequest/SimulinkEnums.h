/*
 * File: SimulinkEnums.h
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

#ifndef SimulinkEnums_h_
#define SimulinkEnums_h_
#include <stdint.h>

typedef uint8_t PrimaryState;

/* enum PrimaryState */
#define Error                          ((PrimaryState)0U)
#define Standby                        ((PrimaryState)1U)        /* Default value */
#define CalibrateIMU                   ((PrimaryState)2U)
#define JumpUp                         ((PrimaryState)3U)
#define Balance                        ((PrimaryState)4U)

typedef uint8_t StateReq;

/* enum StateReq */
#define ReqStandby                     ((StateReq)1U)            /* Default value */
#define ReqCalibrateIMU                ((StateReq)2U)
#define ReqBalance                     ((StateReq)3U)
#endif                                 /* SimulinkEnums_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
