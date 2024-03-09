/*
 * VS_OrientationEstimation.h
 *
 *  Created on: Jul. 20, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_
#define INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_

#include "PR_MPU6050.h"

/* Start Global Struct definitions */
typedef struct VS_OrientationData {
	float roll_deg;
	float pitch_deg;
	float yaw_deg;
} VS_OrientationData_t;

typedef struct VS_Orientation_Bus {
	VS_OrientationData_t AccelOrientation;
	VS_OrientationData_t GyroOrientation;
	VS_OrientationData_t FiltAccelOrientation;
	VS_OrientationData_t CompFiltOrientation;
} VS_Orientation_Bus_t;

/* Start Function Prototypes */
VS_Orientation_Bus_t EstimateOrientation(IP_MPU6050_Bus_t IP_MPU6050_Bus);
/* End Function Prototypes */

#endif /* INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_ */
