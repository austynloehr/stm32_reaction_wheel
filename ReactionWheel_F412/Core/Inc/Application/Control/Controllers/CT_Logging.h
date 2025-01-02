/*
 * CT_Logging.h
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */

#ifndef INC_CONTROL_CONTROLLERS_CT_LOGGING_H_
#define INC_CONTROL_CONTROLLERS_CT_LOGGING_H_

#include "main.h"
#include "IO_VESC.h"
#include "PR_MPU6050.h"
#include "VS_OrientationEstimation.h"
#include "VS_StateRequest.h"
#include "CT_PrimaryStateMachine.h"
#include "CT_BalanceController.h"

/* Start Global Struct definitions */
typedef struct CT_Log_Bus {
	uint8_t *pData;
	uint16_t Size;
} CT_Log_Bus_t;
/* End Global Struct definitions */

/* Start Global Function Prototypes */
CT_Log_Bus_t CT_Logging(IP_MPU6050_Bus_t IP_MPU6050_Bus, HI_VESC_Bus_t HI_VESC_Bus, VS_Orientation_Bus_t VS_Orientation_Bus, VS_StateRequest_Bus_t VS_StateRequest_Bus, CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine_Bus, CT_Balance_Bus_t CT_Balance_Bus);
/* End Global Function Prototypes */

#endif /* INC_CONTROL_CONTROLLERS_CT_LOGGING_H_ */
