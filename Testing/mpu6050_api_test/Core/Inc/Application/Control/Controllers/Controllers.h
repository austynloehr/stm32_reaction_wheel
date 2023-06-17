/*
 * Controllers.h
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_CONTROLLERS_CONTROLLERS_H_
#define INC_APPLICATION_CONTROL_CONTROLLERS_CONTROLLERS_H_

#include "Processing.h"

/* Controller Libraries */
#include "CT_Logging.h"

/* Start global typedefs */
typedef struct CT_Bus{
	CT_Log_Bus_t CT_Log_Bus;
} CT_Bus_t;
/* End global typedefs */

/* Start global function prototypes */
CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_CONTROL_CONTROLLERS_CONTROLLERS_H_ */
