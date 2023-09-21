/*
 * CT_Logging.h
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */

#ifndef INC_CONTROL_CONTROLLERS_CT_LOGGING_H_
#define INC_CONTROL_CONTROLLERS_CT_LOGGING_H_

#include <stdint.h>
#include "PR_MPU6050.h"
#include "VS_OrientationEstimation.h"

/* Start Global Struct definitions */
typedef struct CT_Log_Bus {
	uint8_t *pData;
	uint16_t Size;
} CT_Log_Bus_t;
/* End Global Struct definitions */

/* Start Global Function Prototypes */
CT_Log_Bus_t CT_Logging(IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus);
/* End Global Function Prototypes */

#endif /* INC_CONTROL_CONTROLLERS_CT_LOGGING_H_ */
