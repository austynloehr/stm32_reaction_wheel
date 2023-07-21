/*
 * Control.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */
#include "Control.h"

/* Start Static function prototypes */
static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus);
/* End Static function prototypes */

/* Start global function definitions */
CT_Bus_t CT_Control(IP_Bus_t IP_Bus){
	CT_Bus_t CT_Bus;

	CT_Bus = CT_Controllers(IP_Bus);

	return CT_Bus;
}
/* End global function definitions */

/* Start Static function definitions */
static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus){
	CT_Bus_t CT_Bus;

	CT_Bus.CT_Log_Bus = CT_Logging(IP_Bus.IP_MPU6050_Bus);

	return CT_Bus;
}
/* End static function definitions */
