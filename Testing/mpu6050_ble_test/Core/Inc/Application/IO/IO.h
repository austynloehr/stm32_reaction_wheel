/*
 * IO.h
 *
 *  Created on: Jul. 10, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_IO_IO_H_
#define INC_APPLICATION_IO_IO_H_

#include "DataBuses.h"
#include "main.h"

/* Start global function prototypes */
HI_Bus_t IO_HardwareInputs(Config_Bus_t Config_Bus);
void IO_HardwareOutputs(Config_Bus_t Config_Bus, OP_Bus_t OP_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_IO_IO_H_ */
