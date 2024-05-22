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
#include "BNO055/bno055.h"
#include "CANFrame/CANFrame.h"
#include "PowerPublisher/PowerPublisher.h"
#include "EngineControl/EngineControl.h"
#include "Encoder/Encoder.h"
#include "MotorController/SpeedController.h"
#include "IMUPublisher/IMUPublisher.h"
#include "DataProcess/DataProcess.h"
//#include "CAN_OSI.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
/* Definitions for tDefault */
osThreadId_t tDefaultHandle;
const osThreadAttr_t tDefault_attributes = { .name = "tDefault", .stack_size =
		200 * 4, .priority = (osPriority_t) osPriorityRealtime4, };
/* USER CODE BEGIN PV */

osThreadId_t TestTaskHandle;
const osThreadAttr_t TestTask_attributes = { .name = "TestTask", .stack_size =
		200 * 4, .priority = (osPriority_t) osPriorityNormal2, };

GPIO_HandlerStruct Blinker = { .Port = GPIOC, .Pin = GPIO_PIN_13 };
GPIO_HandlerStruct BrakePin = { .Port = Engine_Brake_GPIO_Port, .Pin =
Engine_Brake_Pin };
GPIO_HandlerStruct GasEnPin = { .Port = Engine_Enable_GPIO_Port, .Pin =
Engine_Enable_Pin };
GPIO_HandlerStruct DirPin = { .Port = Engine_Dir_GPIO_Port, .Pin =
Engine_Dir_Pin };

I2C_OS_HandlerStruct mainI2C;
INA219_t INA219_main;
BNO055_HandlerStruct imuBNO055;

CAN_OS_HandlerStruct CANmain;
CANFrame_HandlerStruct CANFrame_Handler;

Encoder_HandlerStruct Encoder;
Engine_HandlerStruct Engine;
SpeedControler_HandlerStruct SpeedController;

PowerPublisher_HandlerStruct PowerPub;
//float Speed = 0;
IMUPublisher_HandlerStruct IMUPub;
uint8_t Break = 0;

float curSpeed = 0;
//float Speed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CAN_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */
void TestTask(void *argument);

void CAN_TxCallback(CAN_HandleTypeDef *hcan);
void CAN_RxFifo0Callback(CAN_HandleTypeDef *hcan);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
//  HAL_GPIO_WritePin(Engine_Brake_GPIO_Port, Engine_Brake_Pin, 0);
//  HAL_GPIO_WritePin(Engine_Brake_GPIO_Port, Engine_Brake_Pin, 1);
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
//  MX_FREERTOS_Init();
	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	SyncPrintf_Init();

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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void) {

	/* USER CODE BEGIN CAN_Init 0 */

	/* USER CODE END CAN_Init 0 */

	/* USER CODE BEGIN CAN_Init 1 */

	/* USER CODE END CAN_Init 1 */
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 4;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = DISABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = ENABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK) {
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
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_Encoder_InitTypeDef sConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 0;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 0;
	if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

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
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 * Configure DMA for memory to memory transfers
 *   hdma_memtomem_dma1_channel1
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
	hdma_memtomem_dma1_channel1.Instance = DMA1_Channel1;
	hdma_memtomem_dma1_channel1.Init.Direction = DMA_MEMORY_TO_MEMORY;
	hdma_memtomem_dma1_channel1.Init.PeriphInc = DMA_PINC_ENABLE;
	hdma_memtomem_dma1_channel1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_memtomem_dma1_channel1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_memtomem_dma1_channel1.Init.Mode = DMA_NORMAL;
	hdma_memtomem_dma1_channel1.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_memtomem_dma1_channel1) != HAL_OK) {
		Error_Handler();
	}

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
	/* DMA1_Channel7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			Engine_Dir_Pin | Engine_Enable_Pin | Engine_Brake_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LED_B_Pin | LED_G_Pin | LED_R_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : Engine_Dir_Pin Engine_Enable_Pin Engine_Brake_Pin */
	GPIO_InitStruct.Pin = Engine_Dir_Pin | Engine_Enable_Pin | Engine_Brake_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : BTN_1_Pin */
	GPIO_InitStruct.Pin = BTN_1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BTN_1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : BTN2_Pin Encoder_Z_Pin */
	GPIO_InitStruct.Pin = BTN2_Pin | Encoder_Z_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_B_Pin LED_G_Pin LED_R_Pin */
	GPIO_InitStruct.Pin = LED_B_Pin | LED_G_Pin | LED_R_Pin;
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

void TestTask(void *argument) {
	while (1) {
		for (int i = 1000; i <= Engine_TIM_MaxPulse; i += 100) {
			Engine_SetSpeed(&Engine, i);
			osDelay(1000);
		}
		for (int i = Engine_TIM_MaxPulse; i >= 1000; i -= 100) {
			Engine_SetSpeed(&Engine, i);
			osDelay(1000);
		}
	}
}

static void CANFrame_RcvDoneHandler(CANFrame_RxHeaderTypedef *CANFrame_RxHeader,
		uint8_t *str) {
	uint8_t strLen = CANFrame_RxHeader->DataLen;
	uint8_t ID;
	char Data[50];
	float Speed;
	int res = 0;
	if (str[0] != '#' || str[strLen - 2] != '\r' || str[strLen - 1] != '\n') {
		return;
	}
	if (str[strLen - 3] == '!') {
		SpeedController_Start(&SpeedController, 0);
		Engine_SetSpeed(&Engine, 0);
		return;
	}

	if (DataPack_Split((char*) str, &ID, Data) != 0) {
		return;
	}

	switch (ID) {
	case 1:
		res = DataPack_GetSpeed(Data, &Speed);
		if (res != 0)
			return;
//		Engine_SetSpeed(&Engine, Speed * Engine_TIM_MaxPulse / 100);
		if (Speed == 0) {
			SpeedController_SetSpeed(&SpeedController, 0);
			SpeedController_Start(&SpeedController, 0);
			Engine_Brake(&Engine, ENGINE_BRAKE_SET);
		}
		if (Speed == SpeedController.SetPoint)
			return;
		if (Speed > 0) {
			SpeedController_SetSpeed(&SpeedController, Speed);
			if (SpeedController.isEnable == 0)
				SpeedController_Start(&SpeedController, 1);
		} else {
//			SpeedController_SetSpeed(&SpeedController, 0);
			SpeedController_Start(&SpeedController, 0);
			Engine_SetSpeed(&Engine, Speed * 4 * Engine_TIM_MaxPulse / 100);
		}
		HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
		break;
	default:
		break;
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN 5 */

	I2C_OS_Init(&mainI2C, &hi2c1);
	CAN_OS_Init(&CANmain, &hcan);
	CAN_OS_RegisterCB(&CANmain, CAN_OS_ActivateTxCB_ID, CAN_TxCallback);
	CAN_OS_RegisterCB(&CANmain, CAN_OS_ActivateRxFifo0_ID, CAN_RxFifo0Callback);

	INA219_Init(&INA219_main, &mainI2C, INA219_ADDRESS_Default);
	CANFrame_Init(&CANFrame_Handler, &CANmain, CANFRAME_ENGINE_CONTROL_ID,
	CAN_FILTER_FIFO0);
	CANFrame_RegCB(&CANFrame_Handler, CANFRAME_RCVCPLT_CB_ID,
			CANFrame_RcvDoneHandler);
//	PowerPublisher_Init(&PowerPub, &INA219_main, &CANFrame_Handler, 50);
//	BNO055_Init(&imuBNO055, &mainI2C, BNO055_I2C_ADDR_HI);

	Encoder_Init(&Encoder, &htim3, 0.3, 10);
	Engine_Init(&Engine, -7200, 7200);
	Engine_ConfigTimer(&Engine, &htim2, TIM_CHANNEL_2);
	Engine_ConfigControlPin(&Engine, BrakePin, DirPin, GasEnPin);

	SpeedController_Init(&SpeedController, &Encoder, &Engine, 50);
	HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
//	IMUPublisher_Init(&IMUPub, &imuBNO055, &CANFrame_Handler, 10);
//	IMUPublisher_EnablePub(&IMUPub, IMUPUBLISHER_PUB_EULER);\
//	IMUPublisher_Start(&IMUPub, 1);
	Encoder.EnCntTraveled = 1;
	Encoder.PulsePerMeter = 2927 * 2;
	SpeedController.PulsePerMeter = 2927 * 2;
	Encoder_Start(&Encoder, 1);
	SpeedController_SetTunings(&SpeedController, 180.0, 1.0, 35.0);
	SpeedController_Start(&SpeedController, 1);
	CAN_OS_Start(&CANmain);
//	PowerPublisher_Start(&PowerPub, 1);
//	SpeedController_SetSpeed(&SpeedController, 0.3);

	SyncPrintf("####### Engine I AM AWAKE ##### \r\n");
//	SpeedController_SetSpeed(&SpeedController, 5);
//	Engine_SetSpeed(&Engine, 100);
//	TestTaskHandle = osThreadNew(TestTask, NULL, &TestTask_attributes);
	osThreadSetPriority(tDefaultHandle, osPriorityBelowNormal);
//	Speed = 10;
	/* Infinite loop */
	for (;;) {
		GPIO_TogglePin(Blinker);
//		SpeedController_SetSpeed(&SpeedController, Speed);
		curSpeed = Encoder_GetSpeed_MPS(&Encoder);
//		Engine_SetSpeed(&Engine, 0);
//		Engine_SetSpeed(&Engine, -80 * Engine_TIM_MaxPulse / 100);
//		SpeedController_SetSpeed(&SpeedController, Speed);
//		Speed = -Speed;
//		Speed = Encoder_GetSpeed_MPS(&Encoder);

//		if (Break == 0) {
//			Engine_Brake(&Engine, ENGINE_BRAKE_CLEAR);
//		}
//		else {
//			Engine_Brake(&Engine, ENGINE_BRAKE_SET);
//		}
//		HAL_GPIO_WritePin(GasEnPin, , PinState)
//		GPIO_WritePin(GasEnPin, 1);
//		GPIO_WritePin(BrakePin, 1);
//		GPIO_WritePin(DirPin, 1);
//		GPIO_WritePin(GasEnPin, 0);
//		GPIO_WritePin(BrakePin, 0);
//		GPIO_WritePin(DirPin, 0);

		osDelay(500);
	}
	/* USER CODE END 5 */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM4) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
