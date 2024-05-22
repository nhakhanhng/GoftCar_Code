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
extern TIM_HandleTypeDef htim1;

void PowerPublisher_Task(void* arg)
{
	PowerPublisher_HandlerStruct* publisher = (PowerPublisher_HandlerStruct*) arg;
	CANFrame_TxHeaderTypedef txHeader;
	txHeader.MessageType = CANFRAME_MSGTYPE_DATA_FRAME;
	txHeader.TargetNode = CANFRAME_DISTANCE_ID;
	char Name[] = "PWRLGPS";
	int Power, Current, Voltage;
	char txbuff[55];
	uint32_t startTime;
	int32_t delayTime;
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
//		txHeader.DataLen = sprintf(txbuff, "856896358568963585689635856896358568963585689635\r\n");
//		SyncPrintf("Len %d: %s",txHeader.DataLen, txbuff);
//		__HAL_TIM_SET_COUNTER(&htim1,0);
//		HAL_TIM_Base_Start(&htim1);
		CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);
//		uint32_t cnt =__HAL_TIM_GET_COUNTER(&htim1);
//		SyncPrintf("SendTime %ld ", cnt);
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
