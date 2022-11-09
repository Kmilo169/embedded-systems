/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	F_LED1=0,
	T_LED1,
	F_LED2,
	T_LED2,
	F_LED3,
	T_LED3,
	F_LED4,
	T_LED4,
	TTOTAL
}TIMERS;

typedef enum{
	LED_STANDBY=0,
	LED_OFF,
	LED_ON
}ST_LEDS;

typedef enum{
	Comm_STANBY=0,
	Comm_PROCESS
}COMMs;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
volatile uint32_t of=0,timers[TTOTAL];
ST_LEDS st_led1=LED_STANDBY,st_led2=LED_STANDBY,st_led3=LED_STANDBY,st_led4=LED_STANDBY;
uint8_t pts[6]={0x16,0x06,0x00,0x00,0x1B,0x19};
uint16_t tr=0;
COMMs com1=Comm_STANBY;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void control_Comm();
void Comm_1();
void control_LEDs();
void LED1();
void LED2();
void LED3();
void LED4();
/* @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
*/
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
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
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  control_LEDs();
	  control_Comm();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LEDG_Pin|LEDR_Pin|LEDB_Pin|LEDY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PULAMA_Pin PULAZU_Pin PULRED_Pin PULVER_Pin */
  GPIO_InitStruct.Pin = PULAMA_Pin|PULAZU_Pin|PULRED_Pin|PULVER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDG_Pin LEDR_Pin LEDB_Pin LEDY_Pin */
  GPIO_InitStruct.Pin = LEDG_Pin|LEDR_Pin|LEDB_Pin|LEDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void LED1()
{
	switch (st_led1)
	{
		case LED_STANDBY:
			HAL_GPIO_WritePin(LEDY_GPIO_Port,LEDY_Pin,0);
			if(1 == HAL_GPIO_ReadPin(PULAMA_GPIO_Port,PULAMA_Pin))
			{
				st_led1=LED_OFF;
				timers[F_LED1]=0;
				timers[T_LED1]=5000;
			}
		break;
		case LED_OFF:
			if(timers[F_LED1]==0)
			{
				HAL_GPIO_WritePin(LEDY_GPIO_Port,LEDY_Pin,0);
				st_led1=LED_ON;
				timers[F_LED1]=500;
			}
			if(timers[T_LED1]==0) st_led1=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULAMA_GPIO_Port,PULAMA_Pin))
			{
				tr=timers[T_LED1];
				com1=Comm_PROCESS;
			}
		break;
		case LED_ON:
			if(timers[F_LED1]==0)
			{
				HAL_GPIO_WritePin(LEDY_GPIO_Port,LEDY_Pin,1);
				st_led1=LED_OFF;
				timers[F_LED1]=500;
			}
			if(timers[T_LED1]==0) st_led1=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULAMA_GPIO_Port,PULAMA_Pin))
			{
				tr=timers[T_LED1];
				com1=Comm_PROCESS;
			}
		break;
	}
}

void LED2()
{
	switch (st_led2)
	{
		case LED_STANDBY:
			HAL_GPIO_WritePin(LEDB_GPIO_Port,LEDB_Pin,0);
			if(1 == HAL_GPIO_ReadPin(PULAZU_GPIO_Port,PULAZU_Pin))
			{
				st_led2=LED_OFF;
				timers[F_LED2]=0;
				timers[T_LED2]=10000;
			}
		break;
		case LED_OFF:
			if(timers[F_LED2]==0)
			{
				HAL_GPIO_WritePin(LEDB_GPIO_Port,LEDB_Pin,0);
				st_led2=LED_ON;
				timers[F_LED2]=250;
			}
			if(timers[T_LED2]==0) st_led2=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULAZU_GPIO_Port,PULAZU_Pin))
			{
				tr=timers[T_LED2];
				com1=Comm_PROCESS;
			}
		break;
		case LED_ON:
			if(timers[F_LED2]==0)
			{
				HAL_GPIO_WritePin(LEDB_GPIO_Port,LEDB_Pin,1);
				st_led2=LED_OFF;
				timers[F_LED2]=250;
			}
			if(timers[T_LED2]==0) st_led2=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULAZU_GPIO_Port,PULAZU_Pin))
			{
				tr=timers[T_LED2];
				com1=Comm_PROCESS;
			}
		break;
	}
}

void LED3()
{
	switch (st_led3)
	{
		case LED_STANDBY:
			HAL_GPIO_WritePin(LEDR_GPIO_Port,LEDR_Pin,0);
			if(1 == HAL_GPIO_ReadPin(PULRED_GPIO_Port,PULRED_Pin))
			{
				st_led3=LED_OFF;
				timers[F_LED3]=0;
				timers[T_LED3]=15000;
			}
		break;
		case LED_OFF:
			if(timers[F_LED3]==0)
			{
				HAL_GPIO_WritePin(LEDR_GPIO_Port,LEDR_Pin,0);
				st_led3=LED_ON;
				timers[F_LED3]=100;
			}
			if(timers[T_LED3]==0) st_led3=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULRED_GPIO_Port,PULRED_Pin))
			{
				tr=timers[T_LED3];
				com1=Comm_PROCESS;
			}
		break;
		case LED_ON:
			if(timers[F_LED3]==0)
			{
				HAL_GPIO_WritePin(LEDR_GPIO_Port,LEDR_Pin,1);
				st_led3=LED_OFF;
				timers[F_LED3]=100;
			}
			if(timers[T_LED3]==0) st_led3=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULRED_GPIO_Port,PULRED_Pin))
			{
				tr=timers[T_LED3];
				com1=Comm_PROCESS;
			}
		break;
	}
}

void LED4()
{
	switch (st_led4)
	{
		case LED_STANDBY:
			HAL_GPIO_WritePin(LEDG_GPIO_Port,LEDG_Pin,0);
			if(1 == HAL_GPIO_ReadPin(PULVER_GPIO_Port,PULVER_Pin))
			{
				st_led4=LED_OFF;
				timers[F_LED4]=0;
				timers[T_LED4]=20000;
			}
		break;
		case LED_OFF:
			if(timers[F_LED4]==0)
			{
				HAL_GPIO_WritePin(LEDG_GPIO_Port,LEDG_Pin,0);
				st_led4=LED_ON;
				timers[F_LED4]=50;
			}
			if(timers[T_LED4]==0) st_led4=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULVER_GPIO_Port,PULVER_Pin))
			{
				tr=timers[T_LED4];
				com1=Comm_PROCESS;
			}
		break;
		case LED_ON:
			if(timers[F_LED4]==0)
			{
				HAL_GPIO_WritePin(LEDG_GPIO_Port,LEDG_Pin,1);
				st_led4=LED_OFF;
				timers[F_LED4]=50;
			}
			if(timers[T_LED4]==0) st_led4=LED_STANDBY;
			if(1 == HAL_GPIO_ReadPin(PULVER_GPIO_Port,PULVER_Pin))
			{
				tr=timers[T_LED4];
				com1=Comm_PROCESS;
			}
		break;
	}
}

void control_LEDs()
{
	LED1();
	LED2();
	LED3();
	LED4();
}

void control_Comm()
{
	Comm_1();
}

void Comm_1()
{
	switch(com1)
	{
		case Comm_STANBY:

		break;
		case Comm_PROCESS:
			pts[2]=((tr)>>8)&0xFF;
			pts[3]=(tr)&0xFF;
			CDC_Transmit_FS(pts, pts[1]);
			com1=Comm_STANBY;
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t k=0;
	for(k=0;k<TTOTAL;k++)
	{
		if(timers[k]!=0) timers[k]--;
	}
}

void fxUSB(uint8_t* buf,uint32_t T)
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
