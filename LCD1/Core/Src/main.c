/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned long long a=0,b=0,token1=0x16C09C19; //Token original de referencia
char cambio=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void fx(unsigned char cmd, char rs);
void initlcd();
void lcd_clear();
void lcd_put_cur(char row, char col);
void lcd_send_string (char *str);
void lcd_send_char(unsigned long long xx);
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
  /* USER CODE BEGIN 2 */
  initlcd(); // Inicializar el LCD
  lcd_clear(); //Limpiar por completo el LCD
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(cambio==0) // Condición que mantiene en el inicio
	  {
		  if(0==(HAL_GPIO_ReadPin(F_RES_GPIO_Port, F_RES_Pin))) //Lectura del estado de la fotoresistencia
			  	  	  	  	  	  	  	  	  	  	  	  	  	// Comparacion para seguir esperando sincronización o activar el conteo de 30 seg
		  {
			  lcd_put_cur(0, 3);
			  lcd_send_string("ESPERANDO");			//Impresion de espera de sincronización
			  lcd_put_cur(1, 1);					//            --
			  lcd_send_string("SINCRONIZACION");	//            --
		  }
		  else
		  {
			  lcd_clear();
			  lcd_send_string("SINCRONIZADO");     //Ejecucion de inicio del conteo de 30 seg
			  HAL_Delay(1000);
			  lcd_clear();
			  cambio=1;								// Salida del bucle de sincronización
		  }
	  }
	  else
	  {
		  if(cambio==1)		//Impresión del titulo "Token Temporal"
		  {
			  lcd_put_cur(0,1);
			  lcd_send_string("TOKEN TEMPORAL");
		  }
		  lcd_put_cur(1,4); //Posisiconamiento del token
		  lcd_send_char(token1^((cambio-1)*30000)); //Funcion para generación de un nuevo token cada 30 seg dependiendo de la cantidad de ciclos pasados
		  for(a=0;a<30;a++)			//Conteo de 30 segundos
		  {
			  for(b=0;b<495;b++)
			  {
				  HAL_Delay(1);
			  }
			  b=0;
		  }
		  a=0;		// Reinicio del reloj
		  cambio++; //Aumento de 1 en los ciclos de 30 seg.
	  }

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

  /** Initializes the CPU, AHB and APB busses clocks
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
  /** Initializes the CPU, AHB and APB busses clocks
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RS_Pin|RW_Pin|EN_Pin|D4_Pin
                          |D5_Pin|D6_Pin|D7_Pin|pulso_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RS_Pin RW_Pin EN_Pin D4_Pin
                           D5_Pin D6_Pin D7_Pin pulso_Pin */
  GPIO_InitStruct.Pin = RS_Pin|RW_Pin|EN_Pin|D4_Pin
                          |D5_Pin|D6_Pin|D7_Pin|pulso_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : F_RES_Pin induc_Pin */
  GPIO_InitStruct.Pin = F_RES_Pin|induc_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void fx(unsigned char cmd,char rs) //Funcion para enviar un comando o dato a la LCD
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

void initlcd() // Funcion de incialización del LC por un conjunto de comandos
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

void lcd_clear() //Funcion para limpiar el LCD
{
	fx(0x01,0);
	HAL_Delay(2);
}

void lcd_put_cur(char row, char col) // Funcion para posicionarse en una fila y columna especifica
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

void lcd_send_string (char *str) //Funcion para enviar una cadena de char (String) en la pantalla LCD
{
	while (*str) fx(*str++,1);
}

void lcd_send_char(unsigned long long xx) //Funcion para enviar un numero de cualquier tamaño a la LC en formato Hexadecimal
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
