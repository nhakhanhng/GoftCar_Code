/*
 * IMUPublisher.h
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#ifndef IMUPublisher_IMUPublisher_H_
#define IMUPublisher_IMUPublisher_H_

#include <CANFrame/CANFrame.h>
#include "main.h"
#include "BNO055/bno055.h"

#define IMUPUBLISHER_PUB_EULER		0x01
#define IMUPBULISHER_PUB_ACCEL		0x02
#define IMUPBULISHER_PUB_GYRO		0x04

typedef struct IMUPublisher_HandlerStruct{
	CANFrame_HandlerStruct* CANFrame;
	BNO055_HandlerStruct* IMU;
	char name[10];

	osThreadId_t Thread;
	uint8_t isEnable;
	uint32_t Interval;

	uint8_t PubDataFlag;
}IMUPublisher_HandlerStruct;

int IMUPublisher_Init(IMUPublisher_HandlerStruct* Publisher, BNO055_HandlerStruct* IMU,
						CANFrame_HandlerStruct* CANFrame, uint32_t Interval);

int IMUPublisher_Start(IMUPublisher_HandlerStruct* Publisher, uint8_t State);
int IMUPublisher_EnablePub(IMUPublisher_HandlerStruct* Publisher, uint8_t IMUDataType);
int IMUPublisher_DisablePub(IMUPublisher_HandlerStruct* Publisher, uint8_t IMUDataType);

#endif /* IMUPublisher_IMUPublisher_H_ */
