/*
 * IMUPublisher.c
 *
 *  Created on: Jun 28, 2023
 *      Author: vanti
 */

#include <IMUPublisher/IMUPublisher.h>
#include "Utility/Utility.h"
#include "string.h"
const osThreadAttr_t IMUPubTask_attributes = {
  .name = "IMUPub",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

static void IMUVector2Str(char* res, BNO055_vector_t* vect)
{
	char strX[10], strY[10], strZ[10];
	FloatToStr(vect->x, strX, 2);
	FloatToStr(vect->y, strY, 2);
	FloatToStr(vect->z, strZ, 2);
	sprintf(res, "%s;%s;%s", strX, strY, strZ);
}

void IMUPublisher_Task(void* arg)
{
	IMUPublisher_HandlerStruct* publisher = (IMUPublisher_HandlerStruct*) arg;
	CANFrame_TxHeaderTypedef txHeader;
	BNO055_vector_t result;
	txHeader.MessageType = CANFRAME_MSGTYPE_DATA_FRAME;
	txHeader.TargetNode = CANFRAME_MASTER_ID;
//	txHeader.TargetNode = CANFRAME_POWER_ID;
	BNO055_setOperationModeNDOF(publisher->IMU);
	char txbuff[54];
	char vecStr[35];
	uint32_t startTime;
	int delayTime;
	while(1)
	{
		if(!publisher->isEnable)
		{
			osThreadSuspend(publisher->Thread);
		}
		startTime = osKernelGetTickCount();

		if((publisher->isEnable & IMUPUBLISHER_PUB_EULER) != 0)
		{
			BNO055_getVectorEuler(publisher->IMU, &result);
			IMUVector2Str(vecStr, &result);
			txHeader.DataLen = sprintf(txbuff, "#14=%s\r\n",vecStr);
			SyncPrintf("Len %d: %s",txHeader.DataLen, txbuff);
			CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);
		};

		if((publisher->isEnable & IMUPBULISHER_PUB_ACCEL) != 0)
		{
			BNO055_getVectorGyroscope(publisher->IMU, &result);
			IMUVector2Str(vecStr, &result);
			txHeader.DataLen = sprintf(txbuff, "#15=%s\r\n",vecStr);
			CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);
		};

		if((publisher->isEnable & IMUPBULISHER_PUB_GYRO) != 0)
		{
			BNO055_getVectorGyroscope(publisher->IMU, &result);
			IMUVector2Str(vecStr, &result);
			txHeader.DataLen = sprintf(txbuff, "#16=%s\r\n",vecStr);
			CANFrame_Send(publisher->CANFrame, &txHeader,(uint8_t*) txbuff, 20);
		};

		delayTime = publisher->Interval - (osKernelGetTickCount() - startTime);
		delayTime = delayTime < 0? 0 : delayTime;
		osDelay(delayTime);

	}
}


int IMUPublisher_Init(IMUPublisher_HandlerStruct* Publisher, BNO055_HandlerStruct* IMU,CANFrame_HandlerStruct* CANFrame, uint32_t Interval)
{
	memset(Publisher, 0, sizeof(IMUPublisher_HandlerStruct));
	Publisher->IMU = IMU;
	Publisher->Interval = Interval;
	Publisher->CANFrame = CANFrame;
	Publisher->Thread = osThreadNew(IMUPublisher_Task, Publisher, &IMUPubTask_attributes);
	return 0;
}
int IMUPublisher_EnablePub(IMUPublisher_HandlerStruct* Publisher, uint8_t IMUDataType)
{
	SET_BIT(Publisher->PubDataFlag, IMUDataType);
	return 0;
}
int IMUPublisher_DisablePub(IMUPublisher_HandlerStruct* Publisher, uint8_t IMUDataType)
{
	CLEAR_BIT(Publisher->PubDataFlag, IMUDataType);
	return 0;
}
int IMUPublisher_Start(IMUPublisher_HandlerStruct* Publisher, uint8_t State)
{
	Publisher->isEnable = State;
	if(State)
	{
		osThreadResume(Publisher->Thread);
	}
	return 0;
}
