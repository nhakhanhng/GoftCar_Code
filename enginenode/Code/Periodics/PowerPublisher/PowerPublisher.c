/*
 * PowerPublisher.c
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#include "PowerPublisher.h"
#include "string.h"
const osThreadAttr_t PowerPubTask_attributes = {
  .name = "PowerPub",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void PowerPublisher_Task(void* arg)
{
	PowerPublisher_HandlerStruct* publisher = (PowerPublisher_HandlerStruct*) arg;
	CANFrame_TxHeaderTypedef txHeader;
	txHeader.MessageType = CANFRAME_MSGTYPE_DATA_FRAME;
	txHeader.TargetNode = CANFRAME_MASTER_ID;
//	txHeader.TargetNode = CANFRAME_POWER_ID;

	char Name[] = "PWRENG";
	int Power, Current, Voltage;
	char txbuff[55];
	uint32_t startTime;
	int delayTime;
	while(1)
	{
		if(!publisher->isEnable)
		{
			osThreadSuspend(publisher->Thread);
		}
		startTime = osKernelGetTickCount();
		Power = INA219_ReadPower(publisher->INA219);
		Current = INA219_ReadCurrent(publisher->INA219);
		Voltage = INA219_ReadBusVoltage(publisher->INA219);
		txHeader.DataLen = sprintf(txbuff, "#12=%s;%d;%d;%d\r\n",Name, Power,Voltage, Current);
//		SyncPrintf("Len %d: %s",txHeader.DataLen, txbuff);
		CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);

		delayTime = publisher->Interval - (osKernelGetTickCount() - startTime);
		delayTime = delayTime < 0? 0 : delayTime;
		osDelay(delayTime);
	}
}


int PowerPublisher_Init(PowerPublisher_HandlerStruct* Publisher, INA219_t* ina219,CANFrame_HandlerStruct* CANFrame, uint32_t Interval)
{
	memset(Publisher, 0, sizeof(PowerPublisher_HandlerStruct));
	Publisher->INA219 = ina219;
	Publisher->Interval = Interval;
	Publisher->CANFrame = CANFrame;
	Publisher->Thread = osThreadNew(PowerPublisher_Task, Publisher, &PowerPubTask_attributes);
	return 0;
}

int PowerPublisher_Start(PowerPublisher_HandlerStruct* Publisher, uint8_t State)
{
	Publisher->isEnable = State;
	if(State)
	{
		osThreadResume(Publisher->Thread);
	}
	return 0;
}
