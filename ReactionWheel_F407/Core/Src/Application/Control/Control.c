/*
 * Control.c
 *
 *  Created on: Jul. 9, 2023
 *      Author: aloehr
 */
#include "Control.h"
#include "VS_OrientationEstimation.h"

/* Start Static function prototypes */
static VS_Bus_t CT_VirtualSensors(IP_Bus_t IP_Bus);
static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus, VS_Bus_t VS_Bus);
/* End Static function prototypes */

/* Start global function definitions */
CT_Bus_t CT_Control(IP_Bus_t IP_Bus){
	VS_Bus_t VS_Bus;
	CT_Bus_t CT_Bus;

	VS_Bus = CT_VirtualSensors(IP_Bus);
	CT_Bus = CT_Controllers(IP_Bus, VS_Bus);

	return CT_Bus;
}
/* End global function definitions */

/* Start Static function definitions */
static VS_Bus_t CT_VirtualSensors(IP_Bus_t IP_Bus){
	VS_Bus_t VS_Bus;

	VS_Bus.VS_Orientation_Bus = EstimateOrientation(IP_Bus.IP_MPU6050_Bus);

	return VS_Bus;
}

static CT_Bus_t CT_Controllers(IP_Bus_t IP_Bus, VS_Bus_t VS_Bus){
	CT_Bus_t CT_Bus;

	CT_Bus.VS_Bus = VS_Bus;
	CT_Bus.CT_Log_Bus = CT_Logging(IP_Bus.IP_MPU6050_Bus, VS_Bus.VS_Orientation_Bus);

	return CT_Bus;
}
/* End static function definitions */
