/*
 * PR_HC05.h
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */

#ifndef INC_PROCESSING_PR_HC05_H_
#define INC_PROCESSING_PR_HC05_H_

#include "main.h"
#include "CT_Logging.h"

/* Start Struct definitions */
typedef struct OP_HC05_Bus {
	uint8_t *pData;
	uint16_t Size;
} OP_HC05_Bus_t;
/* End Struct definitions */

/* Start Function Prototypes */
OP_HC05_Bus_t OP_HC05(CT_Log_Bus_t CT_Log_Bus);
/* End Function Prototypes */

#endif /* INC_PROCESSING_PR_HC05_H_ */
