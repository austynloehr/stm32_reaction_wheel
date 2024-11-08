/*
 * VS_OrientationEstimation.c
 *
 *  Created on: Jul. 20, 2023
 *      Author: aloehr
 */

#include "VS_OrientationEstimation.h"
#include "Config_DSP.h"
#include <math.h>

/* Start Global Variables */

/* Start Global Variables */

/* Start local enum definitions */
typedef struct KalmanFilterState {
	arm_matrix_instance_f32 F;
	arm_matrix_instance_f32 X;
	arm_matrix_instance_f32 Q;
	arm_matrix_instance_f32 P;
	arm_matrix_instance_f32 R;
	arm_matrix_instance_f32 H;
	arm_matrix_instance_f32 y;
	arm_matrix_instance_f32 K;
} KalmanFilterState_t;
/* End local enum definitions */

/* Start Static Function Prototypes */
static float CalcThetaAccel(float Ax, float Ay);
static KalmanFilterState_t* KalmanInit(float dt);
static void KalmanPredict(KalmanFilterState_t* pKf);
/* End Static Function Prototypes */

/* Start Global Function Definitions */
VS_Orientation_Bus_t EstimateOrientation(IP_MPU6050_Bus_t IP_MPU6050_Bus, float dt){
	VS_Orientation_Bus_t VS_Orientation_Bus;
	static bool OneShot = true;

	if (OneShot){
		static KalmanFilterState_t* pKf = KalmanInit(dt);
		OneShot = false;
	}

	KalmanPredict(pKf);
	float state = (pKf->X.pData)[0];
	float ThetaAccel_deg = CalcThetaAccel(IP_MPU6050_Bus.AxFilt_mps2, IP_MPU6050_Bus.AyFilt_mps2);
	VS_Orientation_Bus.RollAngle_deg = ThetaAccel_deg;

	return VS_Orientation_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static float CalcThetaAccel(float Ax, float Ay){
	// This function calculates roll angle (theta) from accelerometer measurements

	float theta_rad = atan2f(Ax, Ay);
	float theta_deg = theta_rad * 180 / M_PI;

	return theta_deg;
}
static KalmanFilterState_t* KalmanInit(float dt){
	static KalmanFilterState_t kf;
	KalmanFilterState_t* pKf = &kf;

	float Q_std = 10;
	float Wz_std = 8;
	float Theta_std = 8;

	float P_Theta = 2;
	float P_ThetaDot = 10;
	float P_ThetaDotDot = 10;


	float F_data[9] = {1,		dt,		.5 * powf(dt,2),
						0, 		1, 		dt,
						0, 		1, 		1};
	arm_mat_init_f32(&kf.F, (uint16_t) 3, (uint16_t) 3, (float32_t*) F_data);

	float X_data[3] = {45,
						0,
						0};
	arm_mat_init_f32(&kf.X, (uint16_t) 3, (uint16_t) 1, (float32_t*) X_data);

	float Q_data[9] = {powf(dt,4)/4 * powf(Q_std,2), 	powf(dt,3)/2 * powf(Q_std,2), 	powf(dt,2)/2 * powf(Q_std,2),
						powf(dt,3)/2 * powf(Q_std,2), 	powf(dt,2) * powf(Q_std,2), 		dt * powf(Q_std,2),
						powf(dt,2)/2 * powf(Q_std,2), 		dt * powf(Q_std,2), 			1 * powf(Q_std,2)};
	arm_mat_init_f32(&kf.Q, (uint16_t) 3, (uint16_t) 3, (float32_t*) Q_data);

	float P_data[9] = {P_Theta, 		0, 				0,
						0, 			P_ThetaDot, 		0,
						0, 				0, 			P_ThetaDotDot};
	arm_mat_init_f32(&kf.P, (uint16_t) 3, (uint16_t) 3, (float32_t*) P_data);

	float R_data[4] = {powf(Theta_std,2), 		0,
								0, 			powf(Wz_std,2)};
	arm_mat_init_f32(&kf.R, (uint16_t) 2, (uint16_t) 2, (float32_t*) R_data);

	float H_data[6] = {1, 	0, 	0,
						0, 	1, 	0};
	arm_mat_init_f32(&kf.H, (uint16_t) 2, (uint16_t) 3, (float32_t*) H_data);

	float y_data[2] = {0, 0};
	arm_mat_init_f32(&kf.y, (uint16_t) 2, (uint16_t) 1, (float32_t*) y_data);

	float k_data[6] = {0,	0,
						0, 	0,
						0, 	0};
	arm_mat_init_f32(&kf.K, (uint16_t) 3, (uint16_t) 2, (float32_t*) k_data);

	return pKf;
}

static void KalmanPredict(KalmanFilterState_t* pKf){
	arm_mat_mult_f32(&(pKf->F), &(pKf->X), &(pKf->X));
}
