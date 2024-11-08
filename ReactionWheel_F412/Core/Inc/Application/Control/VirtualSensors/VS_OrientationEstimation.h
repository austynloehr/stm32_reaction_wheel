/*
 * VS_OrientationEstimation.h
 *
 *  Created on: Jul. 20, 2023
 *      Author: aloehr
 */

#ifndef INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_
#define INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_

#include "PR_MPU6050.h"
#include "main.h"

/* Start Global Struct definitions */

/* End Global Struct definitions */

/* Start Global Struct definitions */
typedef struct VS_Orientation_Bus {
	float RollAngle_deg;
} VS_Orientation_Bus_t;

/* Start Function Prototypes */
VS_Orientation_Bus_t EstimateOrientation(IP_MPU6050_Bus_t IP_MPU6050_Bus, float dt);
/* End Function Prototypes */

#endif /* INC_APPLICATION_CONTROL_VIRTUALSENSORS_VS_ORIENTATIONESTIMATION_H_ */
