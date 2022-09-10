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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define F_CLK  72000000UL
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
volatile uint8_t medida = 0;
volatile uint32_t p1 = 0;
volatile uint32_t p2 = 0;
volatile uint32_t pasos = 0;
volatile uint16_t of = 0;
volatile uint32_t fs = 0;
char i=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void initlcd();
void lcd_clear();
void lcd_put_cur(char row, char col);
void lcd_send_string (char *str);
void lcd_send_char_hod(unsigned long long xx, char hod);
void lcd_print_fhz(unsigned long long xx, char hz);
void lcd_print_perc(unsigned long long xx);
void clean_row(char row);
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
  /* USER CODE BEGIN 2 */
  initlcd();
  lcd_clear();
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(1==(HAL_GPIO_ReadPin(USER_GPIO_Port,USER_Pin)))
	  {
		  i++;
	  }
	  fs=(uint32_t)(F_CLK/pasos);
	  lcd_print_fhz(fs, i%3);
	  lcd_print_perc(fs);

	  /*
	   *lcd_send_string("HOLA");
	  	  HAL_Delay(1000);
	   */

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
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EN_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Pin */
  GPIO_InitStruct.Pin = USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_Pin RS_Pin D4_Pin D5_Pin
                           D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = EN_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void fx(unsigned char cmd,char rs)
{
	unsigned char t1,i,t2;
		HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin, rs);
		t1=(cmd>>4)&0x0F;
			for(i=0;i<4;i++){
				t2=t1&0x01;
				switch(i){
				case 0:
					HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, t2);
				break;
				case 1:
					HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, t2);
				break;
				case 2:
					HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin,t2);
				break;
				case 3:
					HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, t2);
				break;
				}
				t1=t1>>1;
			}
			HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin, 1);
				HAL_Delay(2);
			HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin, 0);
				HAL_Delay(1);
				t1=cmd&0x0F;
				i=0;
				for(i;i<4;i++){
						t2=(t1)&0x01;
						switch(i){
						case 0:
						HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, t2);
						       break;
						case 1:
								HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, t2);
								break;
						case 2:
								HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin,t2);
								break;
						case 3:
								HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, t2);
								break;
						}
						t1=t1>>1;
					}
					HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin, 1);
						HAL_Delay(2);
					HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin, 0);
						HAL_Delay(1);
}

void initlcd()
{
	HAL_Delay(40);
	fx(0x28,0);
	HAL_Delay(2);
	fx(0x01,0);
	HAL_Delay(2);
	fx(0x02,0);
	HAL_Delay(2);
	fx(0x0C,0);
	HAL_Delay(2);
	fx(0x06,0);
	HAL_Delay(2);
	fx(0x80,0);
	HAL_Delay(2);
}

void lcd_clear()
{
	fx(0x01,0);
	HAL_Delay(2);
}

void lcd_put_cur(char row, char col)
{
	if(row==0)
	{
		col|=0x80;
	}
	else
	{
		col|=0xC0;
	}
    fx(col,0);
}

void lcd_send_string(char *str)
{
	while (*str) fx(*str++,1);
}

void lcd_send_char_hod(unsigned long long xx, char hod)
{
	if(hod==0)
	{
		char temp=0,i=0;

			for(i;i<8;i++)
			{
				temp=(xx>>(28-(4*i)))&0xF;
				if(temp<10)
				{
					temp=temp+48;
				}
				else
				{
					temp=temp+55;
				}
				fx(temp,1);
			}
	}else{
		short int a[10],i=0;

			while(xx>0)
			{
				a[i]=xx%10;
				xx=xx/10;
				i++;
			}
			for(i=i-1;i>=0;i--)
			{
				fx(a[i]+48,1);
			}
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
    if(medida == 0)
    {
        p1=TIM2->CCR1;
        of= 0;
        medida=1;
    }
    else if(medida==1)
    {
        p2 = TIM2->CCR1;
        pasos=(p2 + (of * 65536)) - p1;
        medida=0;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    of++;
}

void lcd_print_fhz(unsigned long long xx,char hz)
{
	clean_row(0);
	lcd_put_cur(0, 0);
	lcd_send_string("F:");
	switch(hz)
	{
		case 0:
			lcd_send_char_hod(xx, 1);
			lcd_send_string(" Hz");
		break;
		case 1:
			if(xx<1000)
			{
				lcd_send_char_hod(xx, 1);
				lcd_send_string("x10^-3 kHz");
			}else{
				xx=xx/1000;
				lcd_send_char_hod(xx, 1);
				lcd_send_string(" kHz");
			}
		break;
		case 2:
			if(xx<1000000)
			{
				if(xx<1000)
				{
					lcd_send_char_hod(xx, 1);
					lcd_send_string("x10^-6 MHz");
				}else{
					xx=xx/1000;
					lcd_send_char_hod(xx, 1);
					lcd_send_string("x10^-3 MHz");
				}
			}else{
				xx=xx/1000000;
				lcd_send_char_hod(xx, 1);
				lcd_send_string(" MHz");
			}
		break;
		default:
			lcd_send_char_hod(xx, 1);
			lcd_send_string(" Hz");
		break;
	}
}

void lcd_print_perc(unsigned long long xx)
{
	short int nc=0,percent=0;
	char contador=0;

	clean_row(1);
	lcd_put_cur(1, 0);

	if(xx<=36000000)
	{
		if(xx<1000000)
		{
			if(xx<10000)
			{
				if(xx<100)
				{
					nc=xx/10;
				}else{
					nc=xx/1000;
					percent=xx/100;
				}
			}else{
				nc=xx/100000;
				percent=xx/10000;
			}
		}else{
			nc=xx/3600000;
			percent=xx/360000;
		}
	}
	for(contador;contador<nc;contador++)
	{
		fx(0xFF,1);
	}
	lcd_put_cur(1, 10);
	lcd_send_char_hod(percent, 1);
	fx(0x25,1);
}

void clean_row(char row)
{
	char casillas=0;
	lcd_put_cur(row, 0);
	while(casillas<16)
	{
		fx(0x80,1);
		casillas++;
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
