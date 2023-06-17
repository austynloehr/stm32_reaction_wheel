/*
 * PR_HC05.c
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */
#include "PR_HC05.h"

OP_HC05_Bus_t OP_HC05(CT_Log_Bus_t CT_Log_Bus){
	static OP_HC05_Bus_t OP_HC05_Bus;

	OP_HC05_Bus.pData = CT_Log_Bus.pData;
	OP_HC05_Bus.Size = CT_Log_Bus.Size;

	return OP_HC05_Bus;
}
