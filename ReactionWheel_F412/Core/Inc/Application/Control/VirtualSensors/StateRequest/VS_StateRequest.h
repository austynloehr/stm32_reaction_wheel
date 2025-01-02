/*
 * VS_StateRequest.h
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_STATEREQUEST_H_
#define INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_STATEREQUEST_H_

#include "SimulinkEnums.h"
#include "main.h"

/* Start Struct definitions */
typedef struct VS_StateRequest_Bus {
	StateReq StateRequest_enum;
} VS_StateRequest_Bus_t;
/* End Struct definitions */

/* End Function Prototypes */
VS_StateRequest_Bus_t VS_StateRequest(bool EnableBtn_bool, PrimaryState LastState_enum, float dt);
/* End Function Prototypes */

#endif /* INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_STATEREQUEST_H_ */
