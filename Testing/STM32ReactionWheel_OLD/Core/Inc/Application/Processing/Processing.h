/*
 * Processing.h
 *
 *  Created on: Jul. 10, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_PROCESSING_PROCESSING_H_
#define INC_APPLICATION_PROCESSING_PROCESSING_H_

#include "DataBuses.h"

/* Start global function prototypes */
IP_Bus_t PR_InputProcessing(Config_Bus_t Config_Bus, HI_Bus_t HI_Bus);
OP_Bus_t PR_OutputProcessing(CT_Bus_t CT_Bus);
/* End global function prototypes */

#endif /* INC_APPLICATION_PROCESSING_PROCESSING_H_ */
