/*
 * DataBuses.h
 *
 *  Created on: Jul. 10, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_DATABUSES_H_
#define INC_APPLICATION_CONTROL_DATABUSES_H_

#include "main.h"

/* Component libraries */
/* Config */
#include "Config_MPU6050.h"
#include "Config_DSP.h"
#include "Config_VESC.h"

/* IO */
#include "IO_MPU6050.h"
#include "IO_HC05.h"
#include "IO_VESC.h"

/* Processing */
#include "PR_MPU6050.h"
#include "PR_HC05.h"
#include "PR_VESC.h"

/* Control */
#include "VS_OrientationEstimation.h"
#include "CT_Logging.h"
#include "CT_BalanceController.h"

/* Start global typedefs */
typedef struct Config_Bus{
	Config_HAL_Bus_t Config_HAL_Bus;
	Config_MPU6050_Bus_t Config_MPU6050_Bus;
}Config_Bus_t;

typedef struct HI_Bus{
	HI_MPU6050_Bus_t HI_MPU6050_Bus;
} HI_Bus_t;

typedef struct IP_Bus{
	IP_MPU6050_Bus_t IP_MPU6050_Bus;
} IP_Bus_t;

typedef struct VS_Bus{
	VS_Orientation_Bus_t VS_Orientation_Bus;
}VS_Bus_t;

typedef struct CT_Bus{
	VS_Bus_t VS_Bus;
	CT_Log_Bus_t CT_Log_Bus;
	CT_Balance_Bus_t CT_Balance_Bus;
} CT_Bus_t;

typedef struct OP_Bus{
	OP_HC05_Bus_t OP_HC05_Bus;
	OP_VESC_Bus_t OP_VESC_Bus;
}OP_Bus_t;
/* End global typedefs */

#endif /* INC_APPLICATION_CONTROL_DATABUSES_H_ */
