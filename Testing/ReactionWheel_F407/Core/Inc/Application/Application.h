/*
 * Application.h
 *
 *  Created on: Jul 9, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_APPLICATION_H_
#define INC_APPLICATION_APPLICATION_H_

#include "main.h"
#include "DataBuses.h"
#include "IO.h"
#include "Processing.h"
#include "Control.h"

/* Start global function prototypes */
Config_Bus_t App_Config(Config_HAL_Bus_t Config_HAL_Bus);
void App_Main(Config_Bus_t Config_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_APPLICATION_H_ */
