/*
 * GPSPublisher.h
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#ifndef GPSPublisher_GPSPublisher_H_
#define GPSPublisher_GPSPublisher_H_

#include <CANFrame/CANFrame.h>
#include "main.h"
#include "GPSHandler/GPSHandler.h"
typedef struct GPSPublisher_HandlerStruct{
	CANFrame_HandlerStruct* CANFrame;
	char name[10];
	GPS_HandlerStruct* gps;
	osThreadId_t Thread;
	uint8_t isEnable;
	uint32_t Interval;
}GPSPublisher_HandlerStruct;

int GPSPublisher_Init(GPSPublisher_HandlerStruct* Publisher, GPS_HandlerStruct* ina219,
						CANFrame_HandlerStruct* CANFrame, uint32_t Interval);

int GPSPublisher_Start(GPSPublisher_HandlerStruct* Publisher, uint8_t State);


#endif /* GPSPublisher_GPSPublisher_H_ */
