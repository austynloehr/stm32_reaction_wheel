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
typedef struct StringPacket{
	uint8_t *pData;
	uint16_t Size;
} StringPacket_t;
/* End Struct definitions */

/*Start global variable definitions */
uint8_t msg[90];  // Need to have this global or else memory location will get overwritten after function call
uint16_t msgLen = sizeof(msg) / sizeof(uint8_t);
/*End global variable definitions */

/* Start Function Prototypes */
static StringPacket_t MakeLogString(uint8_t *msg, uint16_t msgLen, IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus);
/* End Function Prototypes */

/* Start Global Function Definitions */
CT_Log_Bus_t CT_Logging(IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus){
	StringPacket_t StringPacket;
	CT_Log_Bus_t Log_Bus;

	StringPacket = MakeLogString(msg, msgLen, IP_MPU6050_Bus, VS_Orientation_Bus);

	Log_Bus.pData = StringPacket.pData;
	Log_Bus.Size = StringPacket.Size;

	return Log_Bus;
}
/* End Global Function Definitions */

/* Start Static Function Definitions */
static StringPacket_t MakeLogString(uint8_t *msg, uint16_t msgLen, IP_MPU6050_Bus_t IP_MPU6050_Bus, VS_Orientation_Bus_t VS_Orientation_Bus){
	StringPacket_t StringPacket;

	uint32_t tick = HAL_GetTick();

	memset(msg, '\0', msgLen);  // Set unused indexes to null char
//	sprintf((char*) msg, "TK:%lu,AX:%.3f,AY:%.3f,AZ:%.3f,WX:%.3f,WY:%.3f,WZ:%.3f\n\r",
//			tick,
//			IP_MPU6050_Bus.accel.XOUT_ms2, IP_MPU6050_Bus.accel.YOUT_ms2, IP_MPU6050_Bus.accel.ZOUT_ms2,
//			IP_MPU6050_Bus.gyro.XOUT_dps, IP_MPU6050_Bus.gyro.YOUT_dps, IP_MPU6050_Bus.gyro.ZOUT_dps);

	sprintf((char*) msg, "TK:%lu,aRoll:%.5f,aRollFilt:%.5f,gRoll:%.5f,compRoll:%.5f\n\r",
			tick,
			VS_Orientation_Bus.AccelOrientation.roll_deg,
			VS_Orientation_Bus.FiltAccelOrientation.roll_deg,
			VS_Orientation_Bus.GyroOrientation.roll_deg,
			VS_Orientation_Bus.CompFiltOrientation.roll_deg);

	StringPacket.pData = msg;
	StringPacket.Size = msgLen;

	return StringPacket;
}
/* Start Static Function Definitions */
