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
#include "tim.h"
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
//#include "Kalman.h"
#include "Read_attitude.h"
#include "Percentage_res.h"
//#include "oled.h"
//#include "oledfont.h"
#include "KEY.h"
#include "Interface.h"
#include "DS1302.h"
#include "Show_DS1302_Data.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"
#include "DS1302_to_Timestamp.h"
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
 
uint8_t arrWrite[] = { 0,0,0,0 };//要写入的数据
uint8_t arrRead[10];  //存储读取到的数据的数组
extern uint32_t Timestamp_DS1302;//DS1302实时时间转变为时间戳的变量
extern struct TX_TIMEData TX_TimeData;
uint8_t one_second_flag=0;
uint8_t arr_time_write[4]={0};
uint8_t arr_time_read[4]={0};
uint8_t Addr_time_write = 0;//写时间进入w25q16的地址
uint8_t Addr_time_read = 0; //从w25q16读时间的地址
uint8_t Read_W25Q16_Data[1];//从串口助手读数据的标志位
uint8_t Read_W25Q16_Data_finish=0;;//读数据完成的标志位
char msg[] = "Hello, world!";
uint8_t BackData[1];
extern uint16_t cnt_printf;
extern uint32_t Read_currentAddress;
uint8_t Continue_send=0;

extern uint8_t angle_stable_flag; //角度稳定标志位
extern uint8_t angle_stable_flag_3sec; //五秒角度稳定标志位
extern uint32_t Read_FinalAddress ;  // 数据存储的最后一个地址
uint8_t mpu6050_motion_interrupt_status = 0;//mpu6050运动检测状态变量
uint8_t is_low_power_mode = 0;//用于标记是否进入低功耗模式
uint8_t CheckAngle_flag = 0;
extern uint16_t cnt_Write_data;//只记录cnt_test秒的数据
uint8_t Times_interrupt=0;//读取地址达到最大时，进入中断的次数
uint8_t Times_interrupt_max_flag=0;//读取地址达到最大时，进入中断的次数
extern uint8_t Finish_print_flag ;
extern uint8_t Conclusion_print_Cnt ;
extern uint8_t Fisrt_print_flag;
//内存已满标志位
extern uint8_t Memory_Full_flag ;
uint16_t cnt_test_l072 ;
uint8_t cnt_test_sec ;
//读取出来的重量的变量，单位为 g
extern float  Read_weight_g ;
//读取的力的变量 ，单位为 N
extern float  Read_Force_N;

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
int fputc(int c,FILE *stream)
{
	uint8_t ch[1]={c};
	HAL_UART_Transmit(&huart1,ch,1,0xFFFF);
	return c;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	  static uint16_t cnt_tim = 0,cnt_mpu6050_check=0;//定时器计数变量
		static uint8_t cnt_angle_stable = 0; 
		if (htim == (&htim2))
    {
			cnt_mpu6050_check++;			
			cnt_tim++;
			if(cnt_mpu6050_check == 100)//半秒
			{
				cnt_mpu6050_check = 0;
				//如果angle_stable_flag是1，angle_stable_flag_5sec为0，则说明之前处于不稳定状态
				if(angle_stable_flag == 1&& angle_stable_flag_3sec!=1)
				{
					cnt_angle_stable ++;
					if(cnt_angle_stable>1)
					{
						angle_stable_flag_3sec = 1;//超过5秒，标志位置一
						cnt_angle_stable = 0;
					}
					else
					{
						angle_stable_flag_3sec = 0;
					}
				}
				else if(angle_stable_flag == 1&& angle_stable_flag_3sec==1)//如果两个都是1，则说明一直处于稳定状态
				{
					angle_stable_flag_3sec = 1;
				}
				else //如果angle_stable_flag为0，则	angle_stable_flag_5sec为0，且	cnt_angle_stable清零			
					cnt_angle_stable = 0;//如果未达到平衡要求，将计数值清0

			}				
			if(cnt_tim == 100)//一秒
			{
				cnt_tim = 0;
				one_second_flag = 1;
//				W25Q16_SectorErase(0x000000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//				W25Q16_PageProgram(0x000000, arrWrite, 4);//写入
//				W25Q16_ReadData(0x000000, arrRead, 10);//读取
//					HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
			}
			
    }		
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1 ) 
  
		{	
		 //打开中断
		 HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);
		 //判断接收的数据
		 //串口发送‘0’用于打印数据，发送‘1’用于清0数据。						
		 if(Read_W25Q16_Data[0] == '0')
		 {
			 Continue_send = 1;
//			 MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断	，当打印完后打开中断		 
//			 memset(Read_W25Q16_Data, 0, 1);//清空数组数据
		 }
     else if(Read_W25Q16_Data[0] == '1')
		 {
			 //当打印完一次后，才可以清0,打印过程中不触发中断
			 if(Finish_print_flag == 1)
							MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
			 Conclusion_print_Cnt = 0;//置零，以便下次打印可以发送结束语
			 //串口发送‘1’，将变量清零，串口再发送‘0’，重新打印。
			 Finish_print_flag = 0;//清零，用于判断下次打印的打印情况
			 cnt_printf = 0;       //测试打印的次数，清零，方便下次打印的次数限制
			 Read_currentAddress = 0;//清零，从首地址读取数据
			 Continue_send = 0;			 //清零，方便接收’0‘时才再次打印。
//			 memset(Read_W25Q16_Data, 0, 1);//清空数组数据
		 }
		 else 
		 {//收到其他数据，关闭中断检测，不然发送‘0’和‘1’时，如果接受失败，会一直触发中断，也不会出现打印一半进入睡眠
			  MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
//				memset(Read_W25Q16_Data, 0, 1);//清空数组数据
		 }
		 HAL_UART_Transmit(&huart1,(uint8_t *)Read_W25Q16_Data,1,100);
			
			
			
			//        HAL_UART_Receive_IT(&huart1, (uint8_t *)Read_W25Q16_Data, sizeof (Read_W25Q16_Data)); //???????,???????????,???????????????
//						MPU_INT_Init();	//运动中断检测 ——寄存器配置
    }
		
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == GPIO_PIN_0)
    {
			//如果mpu6050进入中断，将运动状态标志位置一
			mpu6050_motion_interrupt_status = 1;
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
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	
  W25Q16_Init	();
//	DS1302_Init();//初始化时间日期
//	DS1302_read_realTime();//读取实时时间
	MPU_Init();			//MPU6050?'?
	mpu_dmp_init();		//dmp?'?
  MPU_INT_Init();	//运动中断检测 ——寄存器配置

	HAL_TIM_Base_Start_IT(&htim2);//1ms定时器中断
  HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);//
//  HAL_UART_Receive_DMA(&huart1,(uint8_t *)Read_W25Q16_Data,1);//

//	   HAL_UART_Receive_IT(&huart1 ,(uint8_t *)BackData,1);
	
//  OLED_Init();                           //OLED初始
//  OLED_Clear();                         //清屏
//  OLED_WR_CMD(0x2e);//停止滚动
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		CheckAngle_flag = CheckAngleStability();
			 if(Finish_print_flag == 0&&Fisrt_print_flag == 1)//如果第一次未打印，或之后还未打印完，关闭中断检测
			 {
					MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
					mpu6050_motion_interrupt_status = 0;							 
			 }
			 
			 //如果内存未存满，此时还可以进入睡眠模式，当将数据打印了出来后，定时器要打开，不然不会再进入角度判断。
				//适合测试用
//			 else if(Read_currentAddress >=Max_Set_Read_ARRAY&&Read_currentAddress < W25Q16_FLASH_SIZE)
//			 {
//				 HAL_TIM_Base_Start_IT(&htim2);//初始化		 				 				 
//			 }
			 //打印完毕，如果内存存满或者没有存满，都关闭定时器，防止没存满时，打印一次完毕之后，如果摇动mpu6050，会再次记录数据
				//也不会再次进入睡眠模式。
			 //但是如果断电复位会清零，如果薄膜压力传感器没达到一定值，则不会记录
			 else if(Finish_print_flag == 1)
			 {				 
				 HAL_TIM_Base_Stop_IT(&htim2);	//关闭定时器中断
			 }

			 
			 if(mpu6050_motion_interrupt_status == 1) 
			{
//				  if(Times_interrupt_max_flag == 0)
//				      Times_interrupt++;
					printf("MPU6050 motion interrupt detected!\n");
//				HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);			
//					printf("%2d\n",Read_W25Q16_Data[0]);//一秒打印一次	
				 HAL_TIM_Base_Start_IT(&htim2);//初始化		 
				 mpu6050_motion_interrupt_status = 0;
			 }
			 
			 
//		if(CheckAngleStability() == 1)
//		{			
////			mpu6050_motion_interrupt_status = 0;  
//			MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x28);//低功耗模式  循环模式
//			MPU_Write_Byte(MPU_PWR_MGMT2_REG,0x87);		//设置唤醒频率
//		}
//				DS1302_read_realTime();//获取时间值
//HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
//			printf("%2d ,%2d  ,%2d\n",0 ,1 ,2);//一秒打印一次	

//		HAL_UART_Receive(&huart1,(uint8_t *)Read_W25Q16_Data,1,10);
			 
			 
#if  0
{
			W25Q16_ChipErase();//全部擦除内存
			Memory_Full_flag = 0;//擦除内存，内存已满标志位清0
//			W25Q16_SectorErase(0x000000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//			W25Q16_SectorErase(0x001000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//			W25Q16_SectorErase(0x002000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//			W25Q16_SectorErase(0x003000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
}
#elif 1
{
			Write_Data_Once_second();
		 //串口发送‘0’用于打印数据，发送‘1’用于清0数据。
//		 if(Read_W25Q16_Data[0] == '0')
//		 {
//			 Continue_send = 1;
//			 MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断	，当打印完后打开中断		 
//			 memset(Read_W25Q16_Data, 0, 1);//清空数组数据
//		 }
//     else if(Read_W25Q16_Data[0] == '1')
//		 {
//			 //当打印完一次后，才可以清0,打印过程中不触发中断
//			 if(Finish_print_flag == 1)
//							MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
//			 Conclusion_print_Cnt = 0;//置零，以便下次打印可以发送结束语
//			 //串口发送‘1’，将变量清零，串口再发送‘0’，重新打印。
//			 Finish_print_flag = 0;//清零，用于判断下次打印的打印情况
//			 cnt_printf = 0;       //测试打印的次数，清零，方便下次打印的次数限制
//			 Read_currentAddress = 0;//清零，从首地址读取数据
//			 Continue_send = 0;			 //清零，方便接收’0‘时才再次打印。
//			 memset(Read_W25Q16_Data, 0, 1);//清空数组数据
//		 }
//		 else 
//		 {//收到其他数据，关闭中断检测，不然发送‘0’和‘1’时，如果接受失败，会一直触发中断，也不会出现打印一半进入睡眠
//			  MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
//				memset(Read_W25Q16_Data, 0, 1);//清空数组数据
//		 }
      if(Continue_send == 1)//持续发送数据
			{
				//加这一行可以使打印时不会在数据包与数据包之间打印出MPU6050 motion interrupt detected!
				MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断				
				Times_interrupt_max_flag = 0;//等到开始打印，才将标志位置0.
			  Read_data_to_printf();			
			}

}

#endif		 



//		Read_Timestamp();
//		Read_Tx_Time(Timestamp_DS1302);
//		OLED_ShowNumber_Fast(50,30,0,3,16,0);
//		Show_W25Q16_Data();
//		Key_num = KEY_Scan();
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
//			//将电压转换为电阻
//			RES_value = ((Voltage_value*4095/3.3)*10000/(4095-(Voltage_value*4095/3.3)))-1000;
//			//将电阻转换为重量
//			if((1000.0f/RES_value)>0&&(1000.0f/RES_value)<0.195)
//			{
//				Read_weight_g = 1896*(1000.0f/RES_value)-64;//公式1，看薄膜压力传感器说明书
//			}
//			else if((1000.0f/RES_value)>0.195)
//			{
//				Read_weight_g = 3810*(1000.0f/RES_value)-443;//公式2
//			}
//			//将重量转换为力
//			Read_Force_N = Read_weight_g*0.2941/250;		
//		sent_data1(-roll*100,-pitch*100,-yaw*100,9);//串口发送数据到匿名上位机
//      printf("%2d ,%2d  ,%2d\n",TX_TimeData.hour ,TX_TimeData.minute ,TX_TimeData.second);

//      printf("%2d ,%2d  ,%2d\n",TimeData.hour ,TimeData.minute ,TimeData.second);
//			HAL_Delay (1000);
		
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
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
