/*
 * CT_BalanceController.h
 *
 *  Created on: Dec 3, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_
#define INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_

#include "main.h"

/* Start Struct definitions */
typedef struct CT_Balance_Bus{
	int32_t MotorSpeedReq_rpm;
	float ThrottleReq_pct;
}CT_Balance_Bus_t;
/* End Struct definitions */

/* Start global function prototypes */
CT_Balance_Bus_t CT_BalanceController(uint8_t BalanceEnable_bool, float RollAng_deg);
/* End global function prototypes */

#endif /* INC_APPLICATION_CONTROL_CONTROLLERS_CT_BALANCECONTROLLER_H_ */
