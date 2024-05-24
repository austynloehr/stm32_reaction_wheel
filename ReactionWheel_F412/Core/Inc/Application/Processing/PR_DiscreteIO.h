/*
 * PR_DiscreteIO.h
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_PROCESSING_PR_DISCRETEIO_H_
#define INC_APPLICATION_PROCESSING_PR_DISCRETEIO_H_

#include "main.h"
#include "SimulinkEnums.h"

/* Start Struct definitions */
typedef struct OP_DiscreteOutput_Bus {
	bool GLEDCmd_bool;
	bool RLEDCmd_bool;
} OP_DiscreteOutput_Bus_t;
/* End Struct definitions */

/* End Function Prototypes */
OP_DiscreteOutput_Bus_t OP_DiscreteOutput(LEDState GLEDStateReq_enum, LEDState RLEDStateReq_enum);
/* End Function Prototypes */

#endif /* INC_APPLICATION_PROCESSING_PR_DISCRETEIO_H_ */
