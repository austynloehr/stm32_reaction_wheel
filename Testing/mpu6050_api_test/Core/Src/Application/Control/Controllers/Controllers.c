/*
 * Controllers.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */

#include "Controllers.h"

/* Start global function definitions */
CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus){
	static CT_Bus_t CT_Bus;

	CT_Bus.CT_Log_Bus = CT_Logging(IP_Bus.IP_MPU6050_Bus);

	return CT_Bus;
}
/* End global function definitions */
