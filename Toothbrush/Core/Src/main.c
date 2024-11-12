/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "Anonymous.h"
#include "Kalman.h"
#include "Read_attitude.h"
#include "Percentage_res.h"
#include "oled.h"
#include "oledfont.h"
#include "KEY.h"
#include "Interface.h"
#include "DS1302.h"
#include "Show_DS1302_Data.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  float pitch,roll,yaw; 		    //y-?
  short aacx,aacy,aacz;			//???????'??
  short gyrox,gyroy,gyroz;		//G???'??
	float	Accel_Angle_x ,Accel_Angle_y;
  extern float roll_kalman ;
	extern uint8_t Grame[8][128] ;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t RxBuffer[1];//??????
uint16_t P,I,D;
	float t1 = 0;
	float t2 = 0;
uint16_t ADC_Value[4];	
uint32_t RES_value;
float Voltage_value;
uint8_t Key_num=0;
uint8_t Switch_Interface=0;

int  Percentage_tx;


uint8_t MID;  //工厂ID
uint16_t DID;//设备ID
 
uint8_t arrWrite[] = { 6,1,8,5 };//要写入的数据
uint8_t arrRead[4];  //存储读取到的数据的数组
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
//???
int fputc(int c,FILE *stream)
{
	uint8_t ch[1]={c};
	HAL_UART_Transmit(&huart1,ch,1,0xFFFF);
	return c;
}


/*
 * ???DataBuff????
 * ?????????
 */

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
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  W25Q16_Init	();
//	DS1302_Init();//初始化时间日期
	DS1302_read_realTime();//读取实时时间
	MPU_Init();			//MPU6050?'?
	mpu_dmp_init();		//dmp?'?
  HAL_UART_Receive_IT(&huart1,(uint8_t *)RxBuffer,1);//
  OLED_Init();                           //OLED初始
  OLED_Clear();                         //清屏
  OLED_WR_CMD(0x2e);//停止滚动
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//      Show_DS1302_Data(0,0,0,16);		
//		OLED_ShowNum(0,0,TimeData.year,4,16,0);
//		OLED_ShowString_Fast(53,0,"R_x:",12,0,3);
//		OLED_ShowNum(0,0,TimeData.year,4,16,0);
//		OLED_ShowNum(0,0,TimeData.year,4,16,0);
	  //该行在memset(Grame, 0, 8*128);前会好一点，不容易出现OLED画面不动
		DS1302_read_realTime(); //读取十进制的实时时间和日期	
		
//		OLED_ShowNumber_Fast(50,30,0,3,16,0);
		Show_W25Q16_Data();
		Key_num = KEY_Scan();
//		if(Key_num == 1)
//		{
//			Switch_Interface++;
//			if(Switch_Interface>3)
//			{
//				Switch_Interface =0;
//			}
//		}
//		if(Switch_Interface == 0)
//			Angle_Interface();
//		else if(Switch_Interface == 1)
//			Gyro_Interface();
//		else if(Switch_Interface == 2)
//			Acc_Interface();
//		else if(Switch_Interface == 3)
//			RES_Interface();
		
		
//		Read_attitude();
//		Percentage_tx = Percentage_res();		
//		sent_data1(-roll*100,-pitch*100,-yaw*100,9);//串口发送数据到匿名上位机
      printf("%2d ,%2d  ,%2d\n",TimeData.hour ,TimeData.minute ,TimeData.second);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
