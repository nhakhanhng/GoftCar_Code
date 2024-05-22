/*
 * PowerPublisher.h
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#ifndef POWERPUBLISHER_POWERPUBLISHER_H_
#define POWERPUBLISHER_POWERPUBLISHER_H_

#include <CANFrame/CANFrame.h>
#include "main.h"
#include "INA219/INA219.h"
typedef struct PowerPublisher_HandlerStruct{
	CANFrame_HandlerStruct* CANFrame;
	INA219_t* INA219;
	char name[10];

	osThreadId_t Thread;
	uint8_t isEnable;
	uint32_t Interval;
}PowerPublisher_HandlerStruct;

int PowerPublisher_Init(PowerPublisher_HandlerStruct* Publisher, INA219_t* ina219,
						CANFrame_HandlerStruct* CANFrame, uint32_t Interval);

int PowerPublisher_Start(PowerPublisher_HandlerStruct* Publisher, uint8_t State);


#endif /* POWERPUBLISHER_POWERPUBLISHER_H_ */
