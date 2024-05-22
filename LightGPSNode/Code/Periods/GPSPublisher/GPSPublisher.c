/*
 * GPSPublisher.c
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#include <GPSPublisher/GPSPublisher.h>
#include "string.h"
#include "Utility/Utility.h"
const osThreadAttr_t GPSPubTask_attributes = {
  .name = "GPSPub",
  .stack_size = 700 * 4,
  .priority = (osPriority_t) osPriorityNormal4,
};

void GPSPublisher_Task(void* arg)
{
	GPSPublisher_HandlerStruct* publisher = (GPSPublisher_HandlerStruct*) arg;
	CANFrame_TxHeaderTypedef txHeader;
	txHeader.MessageType = CANFRAME_MSGTYPE_DATA_FRAME;
	txHeader.TargetNode = CANFRAME_DISTANCE_ID;
	GPS_DataStruct gpsData;
	int Status;
	char strLong[10], strLat[10];
	char txbuff[55];
	uint32_t startTime;
	uint32_t delayTime;
	while(1)
	{
		if(!publisher->isEnable)
		{
			GPS_Start(publisher->gps, 0);
			osThreadSuspend(publisher->Thread);
		}
		startTime = osKernelGetTickCount();
		Status = GPS_GetData(publisher->gps, &gpsData);
		FloatToStr(gpsData.Long, strLong, 6);
		FloatToStr(gpsData.Lat, strLat, 6);


		txHeader.DataLen = sprintf(txbuff, "#11=%s;%s\r\n",strLong, strLat);
		CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);

		delayTime = publisher->Interval - (osKernelGetTickCount() - startTime);

		delayTime = delayTime < 0 ? 0 : delayTime;
		SyncPrintf("Len %d: %s ", txHeader.DataLen, txbuff);
//		SyncPrintf("Delay %ld  Status %d \r\n", delayTime, Status);

		osDelay(delayTime);
	}
}


int GPSPublisher_Init(GPSPublisher_HandlerStruct* Publisher, GPS_HandlerStruct* gps,CANFrame_HandlerStruct* CANFrame, uint32_t Interval)
{
	memset(Publisher, 0, sizeof(GPSPublisher_HandlerStruct));
	Publisher->gps = gps;
	Publisher->Interval = Interval;
	Publisher->CANFrame = CANFrame;
	Publisher->Thread = osThreadNew(GPSPublisher_Task, Publisher, &GPSPubTask_attributes);
	if(Publisher->Thread == NULL)
	{
		SyncPrintf("Task Create Failed");
		Error_Handler();
	}
	return osOK;
}

int GPSPublisher_Start(GPSPublisher_HandlerStruct* Publisher, uint8_t State)
{
	Publisher->isEnable = State;
	if(State)
	{
		GPS_Start(Publisher->gps, 1);
		osThreadResume(Publisher->Thread);
	}
	return 0;
}
