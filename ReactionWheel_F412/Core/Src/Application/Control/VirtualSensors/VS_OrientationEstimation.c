/*
 * VS_OrientationEstimation.c
 *
 *  Created on: Jul. 20, 2023
 *      Author: aloehr
 */

#include "VS_OrientationEstimation.h"
#include "Config_DSP.h"
#include <math.h>

#define DT_0 .002 // Initial dt value

#define Q_STD 10
#define WZ_STD 8
#define THETA_STD 8

#define P_THETA 7
#define P_THETADOT 4
#define P_THETADOTDOT 4

/* Start local enum definitions */
typedef struct KalmanFilterState {
	arm_matrix_instance_f32 F;
	arm_matrix_instance_f32 X;
	arm_matrix_instance_f32 Q;
	arm_matrix_instance_f32 P;
	arm_matrix_instance_f32 R;
	arm_matrix_instance_f32 H;
	arm_matrix_instance_f32 Z;
	arm_matrix_instance_f32 y;
	arm_matrix_instance_f32 K;
} KalmanFilterState_t;
/* End local enum definitions */

/* Start Static Function Prototypes */
static float CalcThetaAccel(float Ax, float Ay);
static KalmanFilterState_t* KalmanInit(void);
static void KalmanPredict(KalmanFilterState_t* pKf);
static void KalmanMeasure(KalmanFilterState_t* pKf, float Ax, float Ay, float Wz);
static void KalmanUpdate(KalmanFilterState_t* pKf);
/* End Static Function Prototypes */

/* Start Global Function Definitions */
VS_Orientation_Bus_t EstimateOrientation(IP_MPU6050_Bus_t IP_MPU6050_Bus, float dt){
	VS_Orientation_Bus_t VS_Orientation_Bus;
	static KalmanFilterState_t* pKf;
	static bool OneShot = true;

	if (OneShot) {
		pKf = KalmanInit();
		OneShot = false;
	}

	KalmanPredict(pKf);
	KalmanMeasure(pKf, IP_MPU6050_Bus.AxFilt_mps2, IP_MPU6050_Bus.AyFilt_mps2, IP_MPU6050_Bus.gyro.ZOUT_dps);
	KalmanUpdate(pKf);

	VS_Orientation_Bus.RollAngle_deg = (pKf->X.pData)[0];
	VS_Orientation_Bus.RollRate_degps = (pKf->X.pData)[1];
	VS_Orientation_Bus.RollAngleCov_deg = (pKf->P.pData)[0];
	VS_Orientation_Bus.RollRateCov_degps = (pKf->P.pData)[4];

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
static KalmanFilterState_t* KalmanInit(void){
	// Initialize all filter state matrices
	// Data arrays are static to prevent being overridden when out of scope

	static KalmanFilterState_t kf;
	static KalmanFilterState_t* pKf = &kf;

	static float F_data[9] = {1,		DT_0,		.5 * powf(DT_0,2),
								0, 		1, 		DT_0,
								0, 		0, 		1};

	static float X_data[3] = {0,
								0,
								0};

	static float Q_data[9] = {powf(DT_0,4)/4 * powf(Q_STD,2), 	powf(DT_0,3)/2 * powf(Q_STD,2), 	powf(DT_0,2)/2 * powf(Q_STD,2),
						powf(DT_0,3)/2 * powf(Q_STD,2), 	powf(DT_0,2) * powf(Q_STD,2), 		DT_0 * powf(Q_STD,2),
						powf(DT_0,2)/2 * powf(Q_STD,2), 		DT_0 * powf(Q_STD,2), 			1 * powf(Q_STD,2)};

	static float P_data[9] = {P_THETA, 		0, 				0,
						0, 			P_THETADOT, 		0,
						0, 				0, 			P_THETADOTDOT};

	static float R_data[4] = {powf(THETA_STD,2), 		0,
								0, 			powf(WZ_STD,2)};

	static float H_data[6] = {1, 	0, 	0,
						0, 	1, 	0};

	static float Z_data[2];
	static float y_data[2];
	static float K_data[6];

	arm_mat_init_f32(&kf.F, (uint16_t) 3, (uint16_t) 3, (float32_t*) F_data);
	arm_mat_init_f32(&kf.X, (uint16_t) 3, (uint16_t) 1, (float32_t*) X_data);
	arm_mat_init_f32(&kf.Q, (uint16_t) 3, (uint16_t) 3, (float32_t*) Q_data);
	arm_mat_init_f32(&kf.P, (uint16_t) 3, (uint16_t) 3, (float32_t*) P_data);
	arm_mat_init_f32(&kf.R, (uint16_t) 2, (uint16_t) 2, (float32_t*) R_data);
	arm_mat_init_f32(&kf.H, (uint16_t) 2, (uint16_t) 3, (float32_t*) H_data);
	arm_mat_init_f32(&kf.Z, (uint16_t) 2, (uint16_t) 1, (float32_t*) Z_data);
	arm_mat_init_f32(&kf.y, (uint16_t) 2, (uint16_t) 1, (float32_t*) y_data);
	arm_mat_init_f32(&kf.K, (uint16_t) 3, (uint16_t) 2, (float32_t*) K_data);

	return pKf;
}

static void KalmanPredict(KalmanFilterState_t* pKf){
	// Copy current state
	arm_matrix_instance_f32 F = pKf->F;
	arm_matrix_instance_f32 X = pKf->X;
	arm_matrix_instance_f32 P = pKf->P;
	arm_matrix_instance_f32 Q = pKf->Q;

	// Initialize matrices for calculations
	arm_matrix_instance_f32 F_t;
	arm_matrix_instance_f32 FP;
	arm_matrix_instance_f32 FPF_t;

	float F_t_data[9];
	float FP_data[9];
	float FPF_t_data[9];

	arm_mat_init_f32(&F_t, (uint16_t) 3, (uint16_t) 3, (float32_t*) F_t_data);
	arm_mat_init_f32(&FP, (uint16_t) 3, (uint16_t) 3, (float32_t*) FP_data);
	arm_mat_init_f32(&FPF_t, (uint16_t) 3, (uint16_t) 3, (float32_t*) FPF_t_data);

	// X = fx
	arm_mat_mult_f32(&F, &X, &(pKf->X)); // Update state

	// P = FPF(t) +  Q
	arm_mat_trans_f32(&F, &F_t);
	arm_mat_mult_f32(&F, &P, &FP);
	arm_mat_mult_f32(&FP, &F_t, &FPF_t);
	arm_mat_add_f32(&FPF_t, &Q, &(pKf->P)); // Update covariance
}

static void KalmanMeasure(KalmanFilterState_t* pKf, float Ax, float Ay, float Wz){
	pKf->Z.pData[0] = CalcThetaAccel(Ax, Ay);
	pKf->Z.pData[1] = Wz;
}

static void KalmanUpdate(KalmanFilterState_t* pKf){
	arm_matrix_instance_f32 X = pKf->X;
	arm_matrix_instance_f32 P = pKf->P;
	arm_matrix_instance_f32 R = pKf->R;
	arm_matrix_instance_f32 H = pKf->H;
	arm_matrix_instance_f32 Z = pKf->Z;
	arm_matrix_instance_f32 K = pKf->K;
	arm_matrix_instance_f32 y = pKf->y;

	arm_matrix_instance_f32 Hx;
	arm_matrix_instance_f32 H_t;
	arm_matrix_instance_f32 PH_t;
	arm_matrix_instance_f32 HPH_t;
	arm_matrix_instance_f32 HPH_t_R;
	arm_matrix_instance_f32 HPH_t_R_inv;
	arm_matrix_instance_f32 Ky;
	arm_matrix_instance_f32 I;
	arm_matrix_instance_f32 KH;
	arm_matrix_instance_f32 I_KH;
	arm_matrix_instance_f32 I_KH_t;
	arm_matrix_instance_f32 I_KHP;
	arm_matrix_instance_f32 I_KHPI_KH_t;
	arm_matrix_instance_f32 KR;
	arm_matrix_instance_f32 K_t;
	arm_matrix_instance_f32 KRK_t;

	float Hx_data[2];
	float H_t_data[6];
	float PH_t_data[6];
	float HPH_t_data[4];
	float HPH_t_R_data[4];
	float HPH_t_R_inv_data[4];
	float Ky_data[3];
	float I_data[9] = {1,0,0,
						0,1,0,
						0,0,1};
	float KH_data[9];
	float I_KH_data[9];
	float I_KH_t_data[9];
	float I_KHP_data[9];
	float I_KHPI_KH_t_data[9];
	float KR_data[6];
	float K_t_data[6];
	float KRK_t_data[6];

	arm_mat_init_f32(&Hx, (uint16_t) 2, (uint16_t) 1, (float32_t*) Hx_data);
	arm_mat_init_f32(&H_t, (uint16_t) 3, (uint16_t) 2, (float32_t*) H_t_data);
	arm_mat_init_f32(&PH_t, (uint16_t) 3, (uint16_t) 2, (float32_t*) PH_t_data);
	arm_mat_init_f32(&HPH_t, (uint16_t) 2, (uint16_t) 2, (float32_t*) HPH_t_data);
	arm_mat_init_f32(&HPH_t_R, (uint16_t) 2, (uint16_t) 2, (float32_t*) HPH_t_R_data);
	arm_mat_init_f32(&HPH_t_R_inv, (uint16_t) 2, (uint16_t) 2, (float32_t*) HPH_t_R_inv_data);
	arm_mat_init_f32(&Ky, (uint16_t) 3, (uint16_t) 1, (float32_t*) Ky_data);
	arm_mat_init_f32(&I, (uint16_t) 3, (uint16_t) 3, (float32_t*) I_data);
	arm_mat_init_f32(&KH, (uint16_t) 3, (uint16_t) 3, (float32_t*) KH_data);
	arm_mat_init_f32(&I_KH, (uint16_t) 3, (uint16_t) 3, (float32_t*) I_KH_data);
	arm_mat_init_f32(&I_KH_t, (uint16_t) 3, (uint16_t) 3, (float32_t*) I_KH_t_data);
	arm_mat_init_f32(&I_KHP, (uint16_t) 3, (uint16_t) 3, (float32_t*) I_KHP_data);
	arm_mat_init_f32(&I_KHPI_KH_t, (uint16_t) 3, (uint16_t) 3, (float32_t*) I_KHPI_KH_t_data);
	arm_mat_init_f32(&KR, (uint16_t) 3, (uint16_t) 2, (float32_t*) KR_data);
	arm_mat_init_f32(&K_t, (uint16_t) 2, (uint16_t) 3, (float32_t*) K_t_data);
	arm_mat_init_f32(&KRK_t, (uint16_t) 3, (uint16_t) 3, (float32_t*) KRK_t_data);

	// y = z - Hx
	arm_mat_mult_f32(&H, &X, &Hx);
	arm_mat_sub_f32(&Z, &Hx, &(pKf->y));
	y = pKf->y; // Copy

	// K = PH(t) (HPH(t) + R)^-1
	arm_mat_trans_f32(&H, &H_t);
	arm_mat_mult_f32(&P, &H_t, &PH_t);
	arm_mat_mult_f32(&H, &PH_t, &HPH_t);
	arm_mat_add_f32(&HPH_t, &R, &HPH_t_R);
	arm_mat_inverse_f32(&HPH_t_R, &HPH_t_R_inv);
	arm_mat_mult_f32(&PH_t, &HPH_t_R_inv, &(pKf->K));
	K = pKf->K; // Copy

	// X = x + Ky
	arm_mat_mult_f32(&K, &y, &Ky);
	arm_mat_add_f32(&X, &Ky, &(pKf->X));

	//(I-KH) P (I-KH)(t) + KRK(t)
	arm_mat_mult_f32(&K, &H, &KH);
	arm_mat_sub_f32(&I, &KH, &I_KH);
	arm_mat_trans_f32(&I_KH, &I_KH_t);
	arm_mat_mult_f32(&I_KH, &P, &I_KHP);
	arm_mat_mult_f32(&I_KHP, &I_KH_t, &I_KHPI_KH_t);
	arm_mat_mult_f32(&K, &R, &KR);
	arm_mat_trans_f32(&K, &K_t);
	arm_mat_mult_f32(&KR, &K_t, &KRK_t);
	arm_mat_add_f32(&I_KHPI_KH_t, &KRK_t, &(pKf->P));
}
