/*
 * Control.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#include "Control.h"

CT_Bus_t CT_Control(IP_Bus_t IP_Bus){
	static CT_Bus_t CT_Bus;

	CT_Bus = CT_Controllers(IP_Bus);

	return CT_Bus;
}
