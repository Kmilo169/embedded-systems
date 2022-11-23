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
	F_LEDB=0,
	F_LEDA,
	T_STANDBY,
	T_CALIB,
	T_VALORES,
	T_MOTORS,
	T_RUN,
	T_Comm_RUN,
	T_BATTERY,
	TTOTAL
}TIMS;

typedef enum{
	LED_STANDBY=0,
	LED_OFF,
	LED_ON
}LEDS;

typedef enum{
	Comm_CLOSE=0,
	Comm_STANDBY,
	Comm_CALIB,
	Comm_MOTORS,
	Comm_RUN,
	Comm_RUN_F,
	Comm_VALORES
}COMMs;

typedef enum{
	STANDBY=0,
	MOTORS=1,
	CALIB,
	RUN
}CAR;

typedef enum{
	MOTOR_STANDBY=0,
	MOTOR_F
}MTRS;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
volatile uint32_t timers[TTOTAL],maxv[8]={0},minv[8]={4095,4095,4095,4095,4095,4095,4095,4095};
uint32_t adcs[9]={0};
int ppp=0;
uint8_t pts[50]={0},k=0,accion=0,m=0,j=0;
LEDS ledA=LED_STANDBY,ledB=LED_STANDBY;
CAR hamilton=STANDBY;
MTRS m1=MOTOR_STANDBY, m2=MOTOR_STANDBY;
COMMs com1=Comm_CLOSE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
/*@retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY*/
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
void initpack();
void velocista();
void controller_LEDS(uint8_t f1,uint8_t f2);
void LED1(uint8_t f);
void LED2(uint8_t f);
void controller_PWM(uint16_t m1,uint16_t m2);
void MOTOR1(uint16_t m);
void MOTOR2(uint16_t m);
void controller_comm();
void calibvalues(uint8_t k);
void seguirlinea();
void configpack(uint8_t* p);
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
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  initpack();
  HAL_TIM_Base_Start_IT(&htim2); // Inicialización del TIM que interrumpe cada 1 ms

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //PWM motor 1
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); //PWM motor 2

  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,0); // MOTOR en 0
  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0); // MOTOR en 0
  HAL_ADC_Start_DMA(&hadc1, adcs, 9);
  timers[T_STANDBY]=50;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_GPIO_TogglePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin);
	  //HAL_GPIO_TogglePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin);

	  //HAL_Delay(100);
	  velocista();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      /*   while(TIM4->CCR3<999)
	 	 {
	 		 TIM4->CCR3=TIM4->CCR3+10;
	 		 HAL_GPIO_TogglePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin);
	 		 HAL_Delay(100);
	 	 }
	 	 while(TIM4->CCR3>1)
	 	 {
	 	 	 TIM4->CCR3=TIM4->CCR3-10;
	 	 	 HAL_GPIO_TogglePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin);
	 	 	 HAL_Delay(100);
	 	 }
	 	 while(TIM4->CCR4<999)
	 	 {
	 		 TIM4->CCR4=TIM4->CCR4+10;
	 		 HAL_GPIO_TogglePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin);
	 		 HAL_Delay(100);
	 	 }
	 	 while(TIM4->CCR4>1)
	 	 {
	 	 	TIM4->CCR4=TIM4->CCR4-10;
	 	 	HAL_GPIO_TogglePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin);
	 	 	HAL_Delay(100);
	 	 }//*/
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 9;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 9;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 8400;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LEDAZULP_Pin|LEDAZUL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port, LEDBLANCO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LEDAZULP_Pin LEDAZUL_Pin */
  GPIO_InitStruct.Pin = LEDAZULP_Pin|LEDAZUL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LEDBLANCO_Pin */
  GPIO_InitStruct.Pin = LEDBLANCO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LEDBLANCO_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void initpack()
{
	pts[0]=0x16;
	pts[2]=0x69;
}

void velocista()
{
	if(timers[T_BATTERY]==0)
	{
		timers[T_BATTERY]=900;
		pts[3]=0x27;
		pts[4]=0x96;
		configpack(pts);
	}
	controller_comm();
	switch(hamilton)
	{
	 case STANDBY:
		 controller_LEDS(20,0);
		 controller_PWM(0,0);
		 if(j==0)
		 {
			 if(timers[T_STANDBY]==0)
			 {
			 	 timers[T_STANDBY]=1000;
			 	 com1=Comm_STANDBY;
			 }
		 }
	 break;
	 case MOTORS:
		 controller_LEDS(5,10);
		 switch(m)
		 {
		 case 0:
			 controller_PWM(0, 1000);
		 break;
		 case 1:
			 controller_PWM(1000, 0);
		 break;
		 case 2:
			 controller_PWM(timers[T_MOTORS]/3,timers[T_MOTORS]/3);
		 break;
		 case 3:
			 controller_PWM(timers[T_MOTORS]/3,1000-(timers[T_MOTORS]/3));
		 break;
		 }
		 if(timers[T_MOTORS]==0)
		 {
			 switch(m)
			 {
			  case 0:
				  timers[T_MOTORS]=3000;
				  m=1;
			  break;
			  case 1:
				  timers[T_MOTORS]=3000;
				  m=2;
			  break;
			  case 2:
				  timers[T_MOTORS]=3000;
				  m=3;
			  break;
			  case 3:
				  hamilton=STANDBY;
				  com1=Comm_MOTORS;
			  break;
			 }
		 }
	 break;
	 case CALIB:
		 if(timers[T_VALORES]==0)
		 {
			 com1=Comm_VALORES;
			 timers[T_VALORES]=10;
		 }
		 controller_LEDS(2,20);
		 controller_PWM(0,0);
		 calibvalues(k);
		 k++;
		 if(k>=9) k=1;
		 if(timers[T_CALIB]==0)
		 {
			hamilton=STANDBY;
			com1=Comm_CALIB;
		 }
	 break;
	 case RUN:
		 controller_LEDS(77,1);
		 seguirlinea();
		 if(timers[T_Comm_RUN]==0)
		 {
			timers[T_Comm_RUN]=100;
			com1=Comm_RUN;
		 }
		 if(timers[T_RUN]==0)
		 {
			 hamilton=STANDBY;
			 com1=Comm_RUN_F;
		 }
	 break;
	}//*/
}

void controller_comm()
{
	switch(com1)
	{
		case Comm_CLOSE:
			accion=0;
		break;
		case Comm_STANDBY:
			accion=1;
			com1=Comm_CLOSE;
		break;
		case Comm_CALIB:
			accion=2;
			com1=Comm_CLOSE;
		break;
		case Comm_MOTORS:
			accion=3;
			com1=Comm_CLOSE;
		break;
		case Comm_RUN:
			accion=4;
			com1=Comm_CLOSE;
		break;
		case Comm_RUN_F:
			accion=5;
			com1=Comm_CLOSE;
		break;
		case Comm_VALORES:
			accion=6;
			com1=Comm_CLOSE;
		break;
	}
	if(accion==0)
	{}else{
		switch(accion)
		{
			case 1:
				pts[3]=0x22;
				pts[4]=0x00;
			break;
			case 2:
				pts[3]=0x33;
				pts[4]=0x96;
			break;
			case 3:
				pts[3]=0x44;
				pts[4]=0x00;
			break;
			case 4:
				pts[3]=0x55;
				pts[4]=0x00;
			break;
			case 5:
				pts[3]=0x66;
				pts[4]=0x00;
			break;
			case 6:
				pts[3]=0x77;
				pts[4]=0x96;
			break;
		}
		configpack(pts);
	}
}

void configpack(uint8_t* p)
{
	if(p[4]==0x00)
	{
		p[5]=0x99;
		p[6]=0x19;
		p[1]=0x07;
	}else{
		switch(p[3])
		{
		case 0x33:
			p[5]=0xAA;
			p[6]=(maxv[0]>>8)&0xFF;
			p[7]=(maxv[0])&0xFF;
			p[8]=(maxv[1]>>8)&0xFF;
			p[9]=(maxv[1])&0xFF;
			p[10]=(maxv[2]>>8)&0xFF;
			p[11]=(maxv[2])&0xFF;
			p[12]=(maxv[3]>>8)&0xFF;
			p[13]=(maxv[3])&0xFF;
			p[14]=(maxv[4]>>8)&0xFF;
			p[15]=(maxv[4])&0xFF;
			p[16]=(maxv[5]>>8)&0xFF;
			p[17]=(maxv[5])&0xFF;
			p[18]=(maxv[6]>>8)&0xFF;
			p[19]=(maxv[6])&0xFF;
			p[20]=(maxv[7]>>8)&0xFF;
			p[21]=(maxv[7])&0xFF;
			p[22]=0XBB;
			p[23]=(minv[0]>>8)&0xFF;
			p[24]=(minv[0])&0xFF;
			p[25]=(minv[1]>>8)&0xFF;
			p[26]=(minv[1])&0xFF;
			p[27]=(minv[2]>>8)&0xFF;
			p[28]=(minv[2])&0xFF;
			p[29]=(minv[3]>>8)&0xFF;
			p[30]=(minv[3])&0xFF;
			p[31]=(minv[4]>>8)&0xFF;
			p[32]=(minv[4])&0xFF;
			p[33]=(minv[5]>>8)&0xFF;
			p[34]=(minv[5])&0xFF;
			p[35]=(minv[6]>>8)&0xFF;
			p[36]=(minv[6])&0xFF;
			p[37]=(minv[7]>>8)&0xFF;
			p[38]=(minv[7])&0xFF;
			p[39]=0x99;
			p[40]=0x19;
			p[1]=0x29;
		break;
		case 0x77:
			p[5]=(adcs[1]>>8)&0xFF;
			p[6]=(adcs[1])&0xFF;
			p[7]=(adcs[2]>>8)&0xFF;
			p[8]=(adcs[2])&0xFF;
			p[9]=(adcs[3]>>8)&0xFF;
			p[10]=(adcs[3])&0xFF;
			p[11]=(adcs[4]>>8)&0xFF;
			p[12]=(adcs[4])&0xFF;
			p[13]=(adcs[5]>>8)&0xFF;
			p[14]=(adcs[5])&0xFF;
			p[15]=(adcs[6]>>8)&0xFF;
			p[16]=(adcs[6])&0xFF;
			p[17]=(adcs[7]>>8)&0xFF;
			p[18]=(adcs[7])&0xFF;
			p[19]=(adcs[8]>>8)&0xFF;
			p[20]=(adcs[8])&0xFF;
			p[21]=0x99;
			p[22]=0x19;
			p[1]=0x17;
		break;
		case 0x27:
			p[5]=(adcs[0]>>8)&0xFF;
			p[6]=(adcs[0])&0xFF;
			p[7]=0x99;
			p[8]=0x19;
			p[1]=0x09;
		break;
		}
	}
	CDC_Transmit_FS(p,p[1]);
}

void seguirlinea()
{
	ppp=((-adcs[2])*0.025)+((-adcs[3])*0.05)+((-adcs[4])*0.1)+((adcs[5])*0.65)+((adcs[6])*0.1)+((adcs[7])*0.05)+((adcs[8])*0.025);
	if(ppp<0)
	{
		controller_PWM(1000+(ppp/4.1),(-(ppp))/4.1);
	}else{
		if(ppp==0)
		{
			controller_PWM(1000,1000);
		}else{
			controller_PWM(ppp/4.1,1000-ppp/4.1);
		}
	}
}

void controller_LEDS(uint8_t f1,uint8_t f2)
{
	LED1(f1);
	LED2(f2);
}

void LED1(uint8_t f)
{
	if(f==0)
	{
		HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,1);
	}else if(f==77)
	{
		HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,0);
	}else{
		switch(ledB)
			{
				case LED_STANDBY:
					HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,0);
					ledB=LED_OFF;
				break;
				case LED_OFF:
					if(timers[F_LEDB]==0)
					{
						HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,0);
						ledB=LED_ON;
						timers[F_LEDB]=1000/f;
					}
				break;
				case LED_ON:
					if(timers[F_LEDB]==0)
					{
						HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,1);
						ledB=LED_OFF;
						timers[F_LEDB]=1000/f;
					}
				break;
			}
	}
}

void LED2(uint8_t f)
{
	if(f==0)
	{
		HAL_GPIO_WritePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin,1);
	}else if(f==77)
	{
		HAL_GPIO_WritePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin,0);
	}else{
		switch(ledA)
				{
					case LED_STANDBY:
						HAL_GPIO_WritePin(LEDBLANCO_GPIO_Port,LEDBLANCO_Pin,0);
						ledA=LED_OFF;
					break;
					case LED_OFF:
						if(timers[F_LEDA]==0)
						{
							HAL_GPIO_WritePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin,0);
							ledA=LED_ON;
							timers[F_LEDA]=1000/f;
						}
					break;
					case LED_ON:
						if(timers[F_LEDA]==0)
						{
							HAL_GPIO_WritePin(LEDAZUL_GPIO_Port,LEDAZUL_Pin,1);
							ledA=LED_OFF;
							timers[F_LEDA]=1000/f;
						}
					break;
				}
	}
}

void controller_PWM(uint16_t m1,uint16_t m2)
{
	MOTOR1(m1);
	MOTOR2(m2);
}

void MOTOR1(uint16_t m)
{
	if(m==0)
	{
		TIM4->CCR3=0;
	}else{
		switch(m1)
			{
				case MOTOR_STANDBY:
					m1=MOTOR_F;
				break;
				case MOTOR_F:
					if(m>1000)
					{
						TIM4->CCR3=1000;
					}else{
						TIM4->CCR3=m;
					}
				break;
			}
	}
}

void MOTOR2(uint16_t m)
{
	if(m==0)
	{
		TIM4->CCR4=0;
	}else{
		switch(m2)
			{
				case MOTOR_STANDBY:
					m2=MOTOR_F;
				break;
				case MOTOR_F:
					if(m>1000)
					{
						TIM4->CCR4=1000;
					}else{
						TIM4->CCR4=m;
					}
					break;
			}
	}
}

void calibvalues(uint8_t k)
{
	if(adcs[k]<minv[k-1])
	{
		minv[k-1]=adcs[k];
	}
	if(adcs[k]>maxv[k-1])
	{
		maxv[k-1]=adcs[k];
	}
}


void fxUSB(uint8_t* buf, uint32_t t)
{
	j=1;
	switch(buf[2])
	{
		case 0x22:
			hamilton=MOTORS;
			timers[T_MOTORS]=3000;
			m=0;
		break;
		case 0x33:
			hamilton=CALIB;
			timers[T_CALIB]=15777;
			k=1;
		break;
		case 0x44:
			hamilton=RUN;
			timers[T_RUN]=20050;
			timers[T_Comm_RUN]=100;
		break;
		case 0x55:
			hamilton=STANDBY;
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
