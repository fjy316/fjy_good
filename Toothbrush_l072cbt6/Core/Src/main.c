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


uint8_t MID;  //����ID
uint16_t DID;//�豸ID
 
uint8_t arrWrite[] = { 0,0,0,0 };//Ҫд�������
uint8_t arrRead[10];  //�洢��ȡ�������ݵ�����
extern uint32_t Timestamp_DS1302;//DS1302ʵʱʱ��ת��Ϊʱ����ı���
extern struct TX_TIMEData TX_TimeData;
uint8_t one_second_flag=0;
uint8_t arr_time_write[4]={0};
uint8_t arr_time_read[4]={0};
uint8_t Addr_time_write = 0;//дʱ�����w25q16�ĵ�ַ
uint8_t Addr_time_read = 0; //��w25q16��ʱ��ĵ�ַ
uint8_t Read_W25Q16_Data[1];//�Ӵ������ֶ����ݵı�־λ
uint8_t Read_W25Q16_Data_finish=0;;//��������ɵı�־λ
char msg[] = "Hello, world!";
uint8_t BackData[1];
extern uint16_t cnt_printf;
extern uint32_t Read_currentAddress;
uint8_t Continue_send=0;

extern uint8_t angle_stable_flag; //�Ƕ��ȶ���־λ
extern uint8_t angle_stable_flag_3sec; //����Ƕ��ȶ���־λ
extern uint32_t Read_FinalAddress ;  // ���ݴ洢�����һ����ַ
uint8_t mpu6050_motion_interrupt_status = 0;//mpu6050�˶����״̬����
uint8_t is_low_power_mode = 0;//���ڱ���Ƿ����͹���ģʽ
uint8_t CheckAngle_flag = 0;
extern uint16_t cnt_Write_data;//ֻ��¼cnt_test�������
uint8_t Times_interrupt=0;//��ȡ��ַ�ﵽ���ʱ�������жϵĴ���
uint8_t Times_interrupt_max_flag=0;//��ȡ��ַ�ﵽ���ʱ�������жϵĴ���
extern uint8_t Finish_print_flag ;
extern uint8_t Conclusion_print_Cnt ;
extern uint8_t Fisrt_print_flag;
//�ڴ�������־λ
extern uint8_t Memory_Full_flag ;
uint16_t cnt_test_l072 ;
uint8_t cnt_test_sec ;
//��ȡ�����������ı�������λΪ g
extern float  Read_weight_g ;
//��ȡ�����ı��� ����λΪ N
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
	
	  static uint16_t cnt_tim = 0,cnt_mpu6050_check=0;//��ʱ����������
		static uint8_t cnt_angle_stable = 0; 
		if (htim == (&htim2))
    {
			cnt_mpu6050_check++;			
			cnt_tim++;
			if(cnt_mpu6050_check == 100)//����
			{
				cnt_mpu6050_check = 0;
				//���angle_stable_flag��1��angle_stable_flag_5secΪ0����˵��֮ǰ���ڲ��ȶ�״̬
				if(angle_stable_flag == 1&& angle_stable_flag_3sec!=1)
				{
					cnt_angle_stable ++;
					if(cnt_angle_stable>1)
					{
						angle_stable_flag_3sec = 1;//����5�룬��־λ��һ
						cnt_angle_stable = 0;
					}
					else
					{
						angle_stable_flag_3sec = 0;
					}
				}
				else if(angle_stable_flag == 1&& angle_stable_flag_3sec==1)//�����������1����˵��һֱ�����ȶ�״̬
				{
					angle_stable_flag_3sec = 1;
				}
				else //���angle_stable_flagΪ0����	angle_stable_flag_5secΪ0����	cnt_angle_stable����			
					cnt_angle_stable = 0;//���δ�ﵽƽ��Ҫ�󣬽�����ֵ��0

			}				
			if(cnt_tim == 100)//һ��
			{
				cnt_tim = 0;
				one_second_flag = 1;
//				W25Q16_SectorErase(0x000000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//				W25Q16_PageProgram(0x000000, arrWrite, 4);//д��
//				W25Q16_ReadData(0x000000, arrRead, 10);//��ȡ
//					HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
			}
			
    }		
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1 ) 
  
		{	
		 //���ж�
		 HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);
		 //�жϽ��յ�����
		 //���ڷ��͡�0�����ڴ�ӡ���ݣ����͡�1��������0���ݡ�						
		 if(Read_W25Q16_Data[0] == '0')
		 {
			 Continue_send = 1;
//			 MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�	������ӡ�����ж�		 
//			 memset(Read_W25Q16_Data, 0, 1);//�����������
		 }
     else if(Read_W25Q16_Data[0] == '1')
		 {
			 //����ӡ��һ�κ󣬲ſ�����0,��ӡ�����в������ж�
			 if(Finish_print_flag == 1)
							MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
			 Conclusion_print_Cnt = 0;//���㣬�Ա��´δ�ӡ���Է��ͽ�����
			 //���ڷ��͡�1�������������㣬�����ٷ��͡�0�������´�ӡ��
			 Finish_print_flag = 0;//���㣬�����ж��´δ�ӡ�Ĵ�ӡ���
			 cnt_printf = 0;       //���Դ�ӡ�Ĵ��������㣬�����´δ�ӡ�Ĵ�������
			 Read_currentAddress = 0;//���㣬���׵�ַ��ȡ����
			 Continue_send = 0;			 //���㣬������ա�0��ʱ���ٴδ�ӡ��
//			 memset(Read_W25Q16_Data, 0, 1);//�����������
		 }
		 else 
		 {//�յ��������ݣ��ر��жϼ�⣬��Ȼ���͡�0���͡�1��ʱ���������ʧ�ܣ���һֱ�����жϣ�Ҳ������ִ�ӡһ�����˯��
			  MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
//				memset(Read_W25Q16_Data, 0, 1);//�����������
		 }
		 HAL_UART_Transmit(&huart1,(uint8_t *)Read_W25Q16_Data,1,100);
			
			
			
			//        HAL_UART_Receive_IT(&huart1, (uint8_t *)Read_W25Q16_Data, sizeof (Read_W25Q16_Data)); //???????,???????????,???????????????
//						MPU_INT_Init();	//�˶��жϼ�� �����Ĵ�������
    }
		
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == GPIO_PIN_0)
    {
			//���mpu6050�����жϣ����˶�״̬��־λ��һ
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
//	DS1302_Init();//��ʼ��ʱ������
//	DS1302_read_realTime();//��ȡʵʱʱ��
	MPU_Init();			//MPU6050?'?
	mpu_dmp_init();		//dmp?'?
  MPU_INT_Init();	//�˶��жϼ�� �����Ĵ�������

	HAL_TIM_Base_Start_IT(&htim2);//1ms��ʱ���ж�
  HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);//
//  HAL_UART_Receive_DMA(&huart1,(uint8_t *)Read_W25Q16_Data,1);//

//	   HAL_UART_Receive_IT(&huart1 ,(uint8_t *)BackData,1);
	
//  OLED_Init();                           //OLED��ʼ
//  OLED_Clear();                         //����
//  OLED_WR_CMD(0x2e);//ֹͣ����
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		CheckAngle_flag = CheckAngleStability();
			 if(Finish_print_flag == 0&&Fisrt_print_flag == 1)//�����һ��δ��ӡ����֮��δ��ӡ�꣬�ر��жϼ��
			 {
					MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
					mpu6050_motion_interrupt_status = 0;							 
			 }
			 
			 //����ڴ�δ��������ʱ�����Խ���˯��ģʽ���������ݴ�ӡ�˳����󣬶�ʱ��Ҫ�򿪣���Ȼ�����ٽ���Ƕ��жϡ�
				//�ʺϲ�����
//			 else if(Read_currentAddress >=Max_Set_Read_ARRAY&&Read_currentAddress < W25Q16_FLASH_SIZE)
//			 {
//				 HAL_TIM_Base_Start_IT(&htim2);//��ʼ��		 				 				 
//			 }
			 //��ӡ��ϣ�����ڴ��������û�д��������رն�ʱ������ֹû����ʱ����ӡһ�����֮�����ҡ��mpu6050�����ٴμ�¼����
				//Ҳ�����ٴν���˯��ģʽ��
			 //��������ϵ縴λ�����㣬�����Ĥѹ��������û�ﵽһ��ֵ���򲻻��¼
			 else if(Finish_print_flag == 1)
			 {				 
				 HAL_TIM_Base_Stop_IT(&htim2);	//�رն�ʱ���ж�
			 }

			 
			 if(mpu6050_motion_interrupt_status == 1) 
			{
//				  if(Times_interrupt_max_flag == 0)
//				      Times_interrupt++;
					printf("MPU6050 motion interrupt detected!\n");
//				HAL_UART_Receive_IT(&huart1,(uint8_t *)Read_W25Q16_Data,1);			
//					printf("%2d\n",Read_W25Q16_Data[0]);//һ���ӡһ��	
				 HAL_TIM_Base_Start_IT(&htim2);//��ʼ��		 
				 mpu6050_motion_interrupt_status = 0;
			 }
			 
			 
//		if(CheckAngleStability() == 1)
//		{			
////			mpu6050_motion_interrupt_status = 0;  
//			MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x28);//�͹���ģʽ  ѭ��ģʽ
//			MPU_Write_Byte(MPU_PWR_MGMT2_REG,0x87);		//���û���Ƶ��
//		}
//				DS1302_read_realTime();//��ȡʱ��ֵ
//HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
//			printf("%2d ,%2d  ,%2d\n",0 ,1 ,2);//һ���ӡһ��	

//		HAL_UART_Receive(&huart1,(uint8_t *)Read_W25Q16_Data,1,10);
			 
			 
#if  0
{
			W25Q16_ChipErase();//ȫ�������ڴ�
			Memory_Full_flag = 0;//�����ڴ棬�ڴ�������־λ��0
//			W25Q16_SectorErase(0x000000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//			W25Q16_SectorErase(0x001000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//			W25Q16_SectorErase(0x002000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//			W25Q16_SectorErase(0x003000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
}
#elif 1
{
			Write_Data_Once_second();
		 //���ڷ��͡�0�����ڴ�ӡ���ݣ����͡�1��������0���ݡ�
//		 if(Read_W25Q16_Data[0] == '0')
//		 {
//			 Continue_send = 1;
//			 MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�	������ӡ�����ж�		 
//			 memset(Read_W25Q16_Data, 0, 1);//�����������
//		 }
//     else if(Read_W25Q16_Data[0] == '1')
//		 {
//			 //����ӡ��һ�κ󣬲ſ�����0,��ӡ�����в������ж�
//			 if(Finish_print_flag == 1)
//							MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
//			 Conclusion_print_Cnt = 0;//���㣬�Ա��´δ�ӡ���Է��ͽ�����
//			 //���ڷ��͡�1�������������㣬�����ٷ��͡�0�������´�ӡ��
//			 Finish_print_flag = 0;//���㣬�����ж��´δ�ӡ�Ĵ�ӡ���
//			 cnt_printf = 0;       //���Դ�ӡ�Ĵ��������㣬�����´δ�ӡ�Ĵ�������
//			 Read_currentAddress = 0;//���㣬���׵�ַ��ȡ����
//			 Continue_send = 0;			 //���㣬������ա�0��ʱ���ٴδ�ӡ��
//			 memset(Read_W25Q16_Data, 0, 1);//�����������
//		 }
//		 else 
//		 {//�յ��������ݣ��ر��жϼ�⣬��Ȼ���͡�0���͡�1��ʱ���������ʧ�ܣ���һֱ�����жϣ�Ҳ������ִ�ӡһ�����˯��
//			  MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
//				memset(Read_W25Q16_Data, 0, 1);//�����������
//		 }
      if(Continue_send == 1)//������������
			{
				//����һ�п���ʹ��ӡʱ���������ݰ������ݰ�֮���ӡ��MPU6050 motion interrupt detected!
				MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�				
				Times_interrupt_max_flag = 0;//�ȵ���ʼ��ӡ���Ž���־λ��0.
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
//			//����ѹת��Ϊ����
//			RES_value = ((Voltage_value*4095/3.3)*10000/(4095-(Voltage_value*4095/3.3)))-1000;
//			//������ת��Ϊ����
//			if((1000.0f/RES_value)>0&&(1000.0f/RES_value)<0.195)
//			{
//				Read_weight_g = 1896*(1000.0f/RES_value)-64;//��ʽ1������Ĥѹ��������˵����
//			}
//			else if((1000.0f/RES_value)>0.195)
//			{
//				Read_weight_g = 3810*(1000.0f/RES_value)-443;//��ʽ2
//			}
//			//������ת��Ϊ��
//			Read_Force_N = Read_weight_g*0.2941/250;		
//		sent_data1(-roll*100,-pitch*100,-yaw*100,9);//���ڷ������ݵ�������λ��
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
