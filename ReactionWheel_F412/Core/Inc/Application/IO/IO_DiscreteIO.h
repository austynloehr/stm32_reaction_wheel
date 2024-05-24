/*
 * IO_DiscreteIO.h
 *
 *  Created on: May 22, 2024
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_IO_IO_DISCRETEIO_H_
#define INC_APPLICATION_IO_IO_DISCRETEIO_H_

#include "main.h"

/* Start Struct definitions */
typedef struct HI_DiscreteInput_Bus {
	bool EnableBtn_bool;
} HI_DiscreteInput_Bus_t;
/* End Struct definitions */

/* End Function Prototypes */
HI_DiscreteInput_Bus_t HI_DiscreteInput(void);
void HO_DiscreteIO(bool GLEDCmd_bool, bool RLEDCmd_bool);
/* End Function Prototypes */

#endif /* INC_APPLICATION_IO_IO_DISCRETEIO_H_ */
