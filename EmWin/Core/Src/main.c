/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "crc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI93xx.h"	//引用LCD的头文件
#include "GUI.h"

#include "delay.h"
#include "touch.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//宏定义SRAM的映射地址以及SRAM的大小
#define EXT_SRAM_ADDR  	    ((uint32_t)0x68000000)
#define EXT_SRAM_SIZE				(1 * 1024 * 1024)
uint32_t bsp_TestExtSRAM(void);	//这个是SRAM的测试函数
/* 绝对定位方式访问 SDRAM,这种方式必须定义成全局变量 */
uint8_t testValue __attribute__((at(EXT_SRAM_ADDR)));

//定时器变量
extern __IO int32_t OS_TimeMS;


//重定向printf到串口1
#if 1
#include <stdio.h>

int fputc(int ch, FILE *stream)
{
    /* 堵塞判断串口是否发送完成 */
    while((USART1->SR & 0X40) == 0);

    /* 串口发送完成，将该字符发送 */
    USART1->DR = (uint8_t) ch;

    return ch;
}
#endif

//定时器中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{
	//TIM3
	if(htim->Instance == TIM3)
	{
		OS_TimeMS++;
	}
	//TIM4
	if(htim->Instance == TIM4)
	{
		GUI_TOUCH_Exec();
	}
}

//GUI触摸屏测试函数
void Mytouch_MainTask(void)
{
	GUI_PID_STATE TouchState;
	int	xPhys;
	int yPhys;
	GUI_Init();
	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_CURSOR_Show();
	GUI_CURSOR_Select(&GUI_CursorCrossL);
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();
	GUI_DispString("");
	while(1)
	{
		GUI_TOUCH_GetState(&TouchState);
		xPhys = GUI_TOUCH_GetxPhys();
		yPhys = GUI_TOUCH_GetyPhys();
		GUI_SetColor(GUI_BLUE);
		GUI_DispStringAt("Analog input:\n",0,40);
		GUI_GotoY(GUI_GetDispPosY()+2);
		GUI_DispString("x:");
		GUI_DispDec(xPhys,4);
		GUI_DispString(",y:");
		GUI_DispDec(yPhys,4);
		GUI_SetColor(GUI_RED);
		GUI_GotoY(GUI_GetDispPosY()+4);
		GUI_DispString("\nPosition:\n");
		GUI_GotoY(GUI_GetDispPosY()+2);
		GUI_DispString("x:");
		GUI_DispDec(TouchState.x,4);
		GUI_DispString(",y:");
		GUI_DispDec(TouchState.y,4);
		delay_ms(50);
	}
}

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
  MX_CRC_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);//启用这两个定时器的中断
	
	TFTLCD_Init();	//初始化LCD,这个必须放在FSMC
	tp_dev.init();				//触摸屏初始化
	
	Mytouch_MainTask();
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*
*********************************************************************************************************
*	函 数 名: bsp_TestExtSRAM
*	功能说明: 扫描测试外部SRAM的全部单元。
*	形    参: 无
*	返 回 值: 0 表示测试通过； 大于0表示错误单元的个数。
*********************************************************************************************************
*/
uint32_t bsp_TestExtSRAM(void)
{
	uint32_t i;
	uint32_t *pSRAM;
	uint8_t *pBytes;
	uint32_t err;
	const uint8_t ByteBuf[4] = {0x55, 0xA5, 0x5A, 0xAA};
	
	/* 写SRAM */
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		*pSRAM++ = i;
	}
	
	/* 读SRAM */
	err = 0;
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE / 4; i++)
	{
		if (*pSRAM++ != i)
		{
			err++;
		}
	}
	printf("SDRAM check round 1 error = %d\n", err);
	if (err > 0)
	{
		return (4 * err);
	}

	#if 1
	/* 对SRAM 的数据求反并写入 */
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i < EXT_SRAM_SIZE/4; i++)
	{
		*pSRAM = ~*pSRAM;
		pSRAM++;
	}

	/* 再次比较SRAM的数据 */
	err = 0;
	pSRAM = (uint32_t *)EXT_SRAM_ADDR;
	for (i = 0; i<EXT_SRAM_SIZE/4;i++)
	{
		if (*pSRAM++ != (~i))
		{
			err++;
		}
	}

	printf("SDRAM check round 2 error = %d\n", err);
	if (err>0)
	{
	return (4 * err);
	}
	#endif

	/* 测试按字节方式访问, 目的是验证 FSMC_NBL0 、 FSMC_NBL1 口线 */
	pBytes = (uint8_t *)EXT_SRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		*pBytes++ = ByteBuf[i];
	}

	/* 比较SRAM的数据 */
	err = 0;
	pBytes = (uint8_t *)EXT_SRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		if (*pBytes++ != ByteBuf[i])
		{
			err++;
		}
	}
	printf("SDRAM check round 3 error = %d\n", err);
	if (err > 0)
	{
		return err;
	}
	return 0;
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
