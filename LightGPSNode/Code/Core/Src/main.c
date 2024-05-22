/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SyncPrintf/SyncPrintf.h"
#include "GPIO/GPIOHandler.h"
#include "UARTHandler/UART_Handler.h"
#include "I2CHandler/I2C_Handler.h"
#include "CANHandler/CANHandler.h"
#include "INA219/INA219.h"
#include "GPSHandler/GPSHandler.h"
#include "stdio.h"
#include "CANFrame/CANFrame.h"
#include "Utility/Utility.h"
#include "PowerPublisher/PowerPublisher.h"
#include "GPSPublisher/GPSPublisher.h"
#include "LEDSpeakerControl/LEDSpeakerControl.h"
#include "DataProcess/DataProcess.h"
//#include "isotp-c/isotp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ISOTP_BUFSIZE		100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;

/* Definitions for tDefault */
osThreadId_t tDefaultHandle;
const osThreadAttr_t tDefault_attributes = {
  .name = "tDefault",
  .stack_size = 500 * 4,
  .priority = (osPriority_t) osPriorityRealtime3,
};
/* USER CODE BEGIN PV */
osThreadId_t TestTaskHandle;
const osThreadAttr_t TestTask_attributes = { .name = "TestTask", .stack_size =
		200 * 4, .priority = (osPriority_t) osPriorityNormal2, };

GPIO_HandlerStruct Blinker = { .Port = GPIOC, .Pin = GPIO_PIN_13 };

GPIO_HandlerStruct LeftPin = { .Port = LED_LEFT_GPIO_Port, .Pin = LED_LEFT_Pin };
GPIO_HandlerStruct RightPin = { .Port = LED_RIGHT_GPIO_Port, .Pin =
LED_RIGHT_Pin };
GPIO_HandlerStruct FrontPin = { .Port = LED_FRONT_GPIO_Port, .Pin =
LED_FRONT_Pin };
GPIO_HandlerStruct StopPin = { .Port = LED_BEHIND_GPIO_Port, .Pin =
LED_BEHIND_Pin };
GPIO_HandlerStruct SpeakerPin =
		{ .Port = SPEAKER_GPIO_Port, .Pin = SPEAKER_Pin };

LEDSpeaker_HandleStruct LEDSpeaker = { .FrontPin = &FrontPin, .LeftPin =
		&LeftPin, .RightPin = &RightPin, .StopPin = &StopPin, .SpeakerPin =
		&SpeakerPin };

UART_OS_HandlerStruct gpsUART;
I2C_OS_HandlerStruct mainI2C;
INA219_t INA219_main;

CAN_OS_HandlerStruct CANmain;
CANFrame_HandlerStruct CANFrame_Handler;

GPS_HandlerStruct GPS;
PowerPublisher_HandlerStruct PowerPub;
GPSPublisher_HandlerStruct GPSPub;

IsoTpLink g_link;

uint8_t g_isotpRecvBuf[ISOTP_BUFSIZE];
uint8_t g_isotpSendBuf[ISOTP_BUFSIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
void TestTask(void *argument);

void CAN_TxCallback(CAN_HandleTypeDef *hcan);
void CAN_RxFifo0Callback(CAN_HandleTypeDef *hcan);

void CANFrame_RcvDoneHandler(CANFrame_RxHeaderTypedef *CANFrame_RxHeader,
		uint8_t *str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of tDefault */
  tDefaultHandle = osThreadNew(StartDefaultTask, NULL, &tDefault_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */

	SyncPrintf_Init();


//	TestTaskHandle = osThreadNew(TestTask, NULL, &TestTask_attributes);

	SyncPrintf("####### LightGPS AWAKE ##### \r\n");
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_BEHIND_Pin|LED_FRONT_Pin|SPEAKER_Pin|LED_RIGHT_Pin
                          |LED_LEFT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_B_Pin|LED_G_Pin|LED_R_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED_BEHIND_Pin LED_FRONT_Pin SPEAKER_Pin LED_RIGHT_Pin
                           LED_LEFT_Pin */
  GPIO_InitStruct.Pin = LED_BEHIND_Pin|LED_FRONT_Pin|SPEAKER_Pin|LED_RIGHT_Pin
                          |LED_LEFT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_2_Pin */
  GPIO_InitStruct.Pin = BTN_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_1_Pin */
  GPIO_InitStruct.Pin = BTN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_B_Pin LED_G_Pin LED_R_Pin */
  GPIO_InitStruct.Pin = LED_B_Pin|LED_G_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (mainI2C.hi2c == hi2c) {
		I2C_OS_MEM_RxCpltCB(&mainI2C);
	}
}
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (mainI2C.hi2c == hi2c) {
		I2C_OS_MEM_TxCpltCB(&mainI2C);
	}
}
void CAN_TxCallback(CAN_HandleTypeDef *hcan) {
	if (CANmain.hcan == hcan) {
		CAN_OS_TxCplt_CB(&CANmain);
	}
}
void CAN_RxFifo0Callback(CAN_HandleTypeDef *hcan) {
	if (CANmain.hcan == hcan) {
		CAN_OS_RxFifo0Cplt_CB(&CANmain);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (gpsUART.huart == huart) {
		UART_OS_RxCplt_CB(&gpsUART);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (gpsUART.huart == huart) {
		UART_OS_TxCplt_CB(&gpsUART);
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (gpsUART.huart == huart) {
		UART_OS_RcvToIdle_CB(&gpsUART, Size);
	}

}

void CANFrame_RcvDoneHandler(CANFrame_RxHeaderTypedef *CANFrame_RxHeader,
		uint8_t *str) {
	uint8_t strLen = CANFrame_RxHeader->DataLen;
	uint8_t ID;
	char Data[50];
	int LeftState, RightState, FrontState, StopState;
	int SpeakerState;
	int res = 4;
	if (str[0] != '#' || str[strLen - 1] != '\n') {
		return;
	}

	if (DataPack_Split((char*) str, &ID, Data) != 0) {
		return;
	}

	switch (ID) {
	case 3:
		res = DataPack_GetLEDState(Data, &LeftState, &RightState, &FrontState,
				&StopState);
		if (res != 0)
			return;
		LEDSpeaker_SetLED(&LEDSpeaker, LeftState, RightState, FrontState,
				StopState);
		break;
	case 4:
		DataPack_GetSpeakerState(Data, &SpeakerState);
		if (res != 0)
			return;
		LEDSpeaker_setSpeaker(&LEDSpeaker, SpeakerState);
		break;
	default:
		break;
	}
}
void TestTask(void *argument) {
	while (1) {
		osDelay(500);
	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the tDefault thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	I2C_OS_Init(&mainI2C, &hi2c1);
	CAN_OS_Init(&CANmain, &hcan);
	UART_OS_Init(&gpsUART, &huart2);
//	GPS_Init(&GPS, &gpsUART, 100);
	INA219_Init(&INA219_main, &mainI2C, INA219_ADDRESS_Default);
	INA219_setCalibration_32V_2A(&INA219_main);
	PowerPublisher_Init(&PowerPub, &INA219_main, &CANFrame_Handler, 20);

//	isotp_init_link(&g_link, CANFRAME_MASTER_ID, g_isotpSendBuf, ISOTP_BUFSIZE,
//			g_isotpRecvBuf,
//			ISOTP_BUFSIZE);
//	g_link.receive_arbitration_id = CANFRAME_LIGHT_GPS_ID;
	CAN_OS_RegisterCB(&CANmain, CAN_OS_ActivateTxCB_ID, CAN_TxCallback);
	CAN_OS_RegisterCB(&CANmain, CAN_OS_ActivateRxFifo0_ID, CAN_RxFifo0Callback);

	CANFrame_Init(&CANFrame_Handler, &CANmain, CANFRAME_MASTER_ID,
	CAN_RX_FIFO0);
	CANFrame_RegCB(&CANFrame_Handler, CANFRAME_RCVCPLT_CB_ID,
			CANFrame_RcvDoneHandler);
	CAN_OS_Start(&CANmain);

//	GPSPublisher_Init(&GPSPub, &GPS, &CANFrame_Handler, 10);
//	GPSPublisher_Start(&GPSPub, 1);
	PowerPublisher_Start(&PowerPub, 1);
	osThreadSetPriority(tDefaultHandle, osPriorityBelowNormal);
	/* Infinite loop */
	for (;;) {
		GPIO_TogglePin(Blinker);
		SyncPrintf(
				"Send Success %ld Send Fail %ld Rcv Success %ld Rcv Failed %ld\r\n ",
				CANFrame_Handler.SendSuccessCounter,
				CANFrame_Handler.SendFailedCounter,
				CANFrame_Handler.RcvSucessCounter,
				CANFrame_Handler.RcvFailedCounter);
		osDelay(500);
	}
  /* USER CODE END 5 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
