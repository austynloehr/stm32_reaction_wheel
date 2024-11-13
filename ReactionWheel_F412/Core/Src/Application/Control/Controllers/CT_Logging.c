/*
 * CT_Logging.c
 *
 *  Created on: Jul 8, 2023
 *      Author: aloehr
 */
#include <stdio.h>
#include <string.h>
#include "CT_Logging.h"

/* Start Struct definitions */
typedef struct LogPacket{
	uint8_t *pData;
	uint16_t Size;
} LogPacket_t;

typedef struct LogPayload{
	uint32_t tick;
	float ax_mps2;
	float ay_mps2;
	float wz_dps;
	float axFilt_mps2;
	float ayFilt_mps2;
	float rollAngle_deg;
	float rollRate_deg;
	float rollAngleCov_deg;
	float rollRateCov_deg;
	int32_t MotorSpeedReq_rpm;
	uint8_t StateReq_enum;
	uint8_t CurrentState_enum;
	uint8_t MotorEnable_bool;
} __attribute__ ((packed)) LogPayload_t;

/* End Struct definitions */

/*Start global variable definitions */
LogPayload_t payload;
uint8_t *pPayload = (uint8_t*) &payload;
/*End global variable definitions */

/* Start Function Prototypes */
static LogPacket_t MakeLogPacket(IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus, VS_StateRequest_Bus_t VS_StateRequest_Bus, CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine_Bus, CT_Balance_Bus_t CT_Balance_Bus);
/* End Function Prototypes */

/* Start Global Function Definitions */
CT_Log_Bus_t CT_Logging(IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus, VS_StateRequest_Bus_t VS_StateRequest_Bus, CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine_Bus, CT_Balance_Bus_t CT_Balance_Bus){
	LogPacket_t LogPacket;
	CT_Log_Bus_t Log_Bus;

	LogPacket = MakeLogPacket(IP_MPU6050_Bus, VS_Orientation_Bus, VS_StateRequest_Bus, CT_PrimaryStateMachine_Bus, CT_Balance_Bus);

	Log_Bus.pData = LogPacket.pData;
	Log_Bus.Size = LogPacket.Size;

	return Log_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static LogPacket_t MakeLogPacket(IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus, VS_StateRequest_Bus_t VS_StateRequest_Bus, CT_PrimaryStateMachine_Bus_t CT_PrimaryStateMachine_Bus, CT_Balance_Bus_t CT_Balance_Bus){
	LogPacket_t LogPacket;

	// Pack payload struct
	payload.tick = HAL_GetTick();
	payload.ax_mps2 = IP_MPU6050_Bus.accel.XOUT_ms2;
	payload.ay_mps2 = IP_MPU6050_Bus.accel.YOUT_ms2;
	payload.wz_dps = IP_MPU6050_Bus.gyro.ZOUT_dps;
	payload.axFilt_mps2 = IP_MPU6050_Bus.AxFilt_mps2;
	payload.ayFilt_mps2 = IP_MPU6050_Bus.AyFilt_mps2;
	payload.rollAngle_deg = VS_Orientation_Bus.RollAngle_deg;
	payload.rollRate_deg = VS_Orientation_Bus.RollRate_degps;
	payload.rollAngleCov_deg = VS_Orientation_Bus.RollAngleCov_deg;
	payload.rollRateCov_deg = VS_Orientation_Bus.RollRateCov_degps;
	payload.StateReq_enum = VS_StateRequest_Bus.StateRequest_enum;
	payload.CurrentState_enum = CT_PrimaryStateMachine_Bus.CurrentState_enum;
	payload.MotorEnable_bool = CT_PrimaryStateMachine_Bus.MotorEnable_bool;
	payload.MotorSpeedReq_rpm = CT_Balance_Bus.MotorSpeedReq_rpm;

	LogPacket.pData = pPayload;
	LogPacket.Size = sizeof(payload);

	return LogPacket;
}
/* Start Static Function Definitions */
