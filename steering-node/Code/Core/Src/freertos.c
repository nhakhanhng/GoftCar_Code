/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN_OSI.h"
#include "SteeringDriver/WheelControl/WheelControl.h"
#include "DataProcess/DataProcess.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NODE_ID 				STEERING
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern CAN_HandleTypeDef hcan;
static CANBufferHandleStruct Rx_Buffer;
static FlagFrameHandle Flag;
//static CANConfigIDTxtypedef pIDtype;
extern Wheel_HandlerStruct Steering;
FlagRecNotification FlagRec;
uint8_t mess[62] = { 0 };
char Print[100] = { 0 };

osThreadId_t mMainTaskHandle;
const osThreadAttr_t mMainTask_attributes = { .name = "mMainTask", .stack_size =
		256 * 4, .priority = (osPriority_t) osPriorityNormal, };
/* Definitions for mCanRecv */
osThreadId_t mCanRecvHandle;
const osThreadAttr_t mCanRecv_attributes = { .name = "mCanRecv", .stack_size =
		256 * 4, .priority = (osPriority_t) osPriorityLow, };
/* Definitions for myDataProcessin */
osThreadId_t myDataProcessinHandle;
const osThreadAttr_t myDataProcessin_attributes = { .name = "myDataProcessin",
		.stack_size = 128 * 4, .priority = (osPriority_t) osPriorityHigh, };

//extern SpeedControler_HandlerStruct SpeedController;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void MainTask(void *argument);
void CanRecv(void *argument);
void DataProcessing(void *argument);

//void MX_FREERTOS_Init(void);

/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void CAN_ProcessMsg(CANBufferHandleStruct *AppBuffer, uint8_t *str) {
	uint8_t strLen = AppBuffer->NodeHandle[AppBuffer->RecvID].PacketLength - 2;
	uint8_t ID;
	char Data[50];
	float Angle;
	int res = 0;
	char strAngle[10];
	if (str[0] != '#' || str[strLen - 1] != '\n') {
		return;
	}

	if (DataPack_Split((char*) str, &ID, Data) != 0) {
		return;
	}

	switch (ID) {
	case 2:
		res = DataPack_GetSteerAngle(Data, &Angle);
		if (res != 0)
			return;
		FloatToStr(Angle, strAngle, 3);
//		SyncPrintf("Ang %s \r\n", strAngle);
		Wheel_SetAngle_ABS(&Steering, Angle);
		break;
	default:
		break;
	}
}

void MX_FREERTOS_Init(void) {
	mCanRecvHandle = osThreadNew(CanRecv, NULL, &mCanRecv_attributes);
	myDataProcessinHandle = osThreadNew(DataProcessing, NULL,
			&myDataProcessin_attributes);
}

void CanRecv(void *argument) {
	/* USER CODE BEGIN CanRecv */
	// FlagRecNotification FlagRec;
	/* Infinite loop */
	for (;;) {
		CAN_Receive_Application(&Rx_Buffer, mess, &Flag, &FlagRec);
		osDelay(1);
	}
	/* USER CODE END CanRecv */
}

void DataProcessing(void *argument) {
	/* USER CODE BEGIN DataProcessing */
	/* Infinite loop */
	for (;;) {
		if (FlagRec == REC_SUCCESS) {
//			uint32_t len = sprintf(Print, "Node 2 Rcv: %s\r\n", mess);
//           HAL_UART_Transmit(&huart1,(uint8_t*)Print,len,HAL_MAX_DELAY);
			if (Rx_Buffer.RecvID == NODE_ID)
				CAN_ProcessMsg(&Rx_Buffer, mess);
		}
		osDelay(1);
	}
	/* USER CODE END DataProcessing */
}

/* USER CODE END Application */

