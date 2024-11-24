#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "Read_attitude.h"
#include "Percentage_res.h"
#include "stdio.h"
#include "string.h"
#include "mpu6050.h"
#include "tim.h"

/**********��ӡ�������������ݽṹ��************/
struct PRINTF_Data
{
	uint8_t day,hour,min,sec; 	//�գ�ʱ���֣���
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;			//���ٶ�
	short gyrox,gyroy,gyroz;	//���ٶ�
	float Vol_val;      //��ѹ
	
  float real_aacx,real_aacy,real_aacz;//��λΪ�������ٶ�g
  float real_gyrox,real_gyroy,real_gyroz;//��λΪ ��/s	
};
struct PRINTF_Data Print;

//��һ�δ�ӡ��־λ
uint8_t Fisrt_print_flag = 0;
//��ӡ����Ч���ݱ�־λ������ڴ����꣬��ô������ڴ淶Χ�ڵ�����Ϊ��Ч���ݣ�
//���δ��������ô�ڶ�ȡ��Print.day = 0xFF,Print.hour = 0xFF,Print.min = 0xFF,Print.sec = 0xFF,֮ǰ������Ϊ��Ч����
uint8_t Finish_print_flag = 0;
//��ӡһ�ν����󣬽�����Ĵ�ӡ��������
uint8_t Conclusion_print_Cnt = 0;
//�ڴ�������־λ
uint8_t Memory_Full_flag = 0;
//��ȡ�����ĵ������,float�����ں������㷽��
float  Read_RES_value = 0;
//��ȡ�����������ı�������λΪ g
float  Read_weight_g = 0;
//��ȡ�����ı��� ����λΪ N
float  Read_Force_N= 0;
/*********************************************/

/*******�Ƕȼ�����********/
float prev_angle[2] = {0}; //�洢��һ�εĽǶ�ֵ
float current_angle[2]; //�洢��ǰ�Ƕ�ֵ
uint8_t angle_stable_flag = 0; //�Ƕ��ȶ���־λ
uint8_t angle_stable_flag_3sec = 0; //����Ƕ��ȶ���־λ
#define ANGLE_THRESHOLD 1.5 //�Ƕ��ż�	
/**************************/

uint16_t cnt_Write_data = 0;//ֻ��¼cnt_test�������
uint16_t cnt_printf=0;

extern uint8_t Grame[8][128] ;
extern uint8_t arrWrite[];//Ҫд�������
extern uint8_t arrRead[10];  //�洢��ȡ�������ݵ�����
extern uint8_t MID;  //����ID
extern uint16_t DID;//�豸ID
extern uint8_t one_second_flag;//��ʱ��һ���־λ
extern  float pitch,roll,yaw; 		    //y-?
extern  short aacx,aacy,aacz;			//???????'??
extern  short gyrox,gyroy,gyroz;		//G???'??
extern int  Percentage_tx;
extern float Voltage_value;
extern uint8_t mpu6050_motion_interrupt_status;
//ͨ��ָ��ǿ������ת�������float
uint8_t* pByte = (uint8_t*)&pitch;
uint8_t* pByte1 = (uint8_t*)&roll;
uint8_t* pByte2 = (uint8_t*)&yaw;
uint8_t* pByte3 = (uint8_t*)&Voltage_value;
//ÿ100ms�ж�һ�νǶ��ȶ���Ҫ��5��Ƕ��ȶ�����Ҫ(10*Time_Cnt_EnterSLEEPMode)��
uint8_t Time_Cnt_EnterSLEEPMode = 0;


//�������е����ݴ���W25Q16��
#define ARRAY_SIZE 33  //�����С
uint8_t dataArray[ARRAY_SIZE];  // Ҫ�洢����������
uint32_t currentAddress = 0;  // ��ǰ��W25Q16�Ĵ洢��ַ����ʼ��Ϊ0
//#define W25Q16_FLASH_SIZE 0x200000  //W25Q16оƬ����
//��W25Q16�ж�ȡ���ݴ�������
#define Read_ARRAY_SIZE 33  //�����С
uint8_t Read_dataArray[Read_ARRAY_SIZE];  // Ҫ�洢����������
uint32_t Read_currentAddress = 0;  // ��ǰ��W25Q16�Ĵ洢��ַ����ʼ��Ϊ0
//��W25Q16�ж�ȡ���ݴ�������
#define Printf_ARRAY_SIZE 15  //�����С
uint32_t Printf_dataArray[Read_ARRAY_SIZE];  // Ҫ�洢����������
uint32_t Read_FinalAddress = 0x0000000+33*20;  // ���ݴ洢�����һ����ַ


////���õ�����ȡ��ַ�������ֵ��Χ���ڣ������Զ�ȡ
//#define Max_Set_Read_ARRAY 33*50+0x000000

void Show_W25Q16_Data(void)
{
  memset(Grame, 0, 8*128);//����������Ԫ��
	W25Q16_ReadID(&MID, &DID);
	OLED_ShowNumber_Fast(0,0,MID,4,12,0);
	OLED_ShowNumber_Fast(0,16,DID,5,12,0);
  W25Q16_SectorErase(0x000000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//	store_data_to_W25Q16(0x000000,1234567,-10,20,0);
	W25Q16_PageProgram(0x000000, arrWrite, 4);//д��
	W25Q16_ReadData(0x000000, arrRead, 10);//��ȡ
	// չʾ���͵�����
//	for (uint8_t i = 0;i < 4;i++) {
//		OLED_ShowNumber_Fast(0+16*i,30,arrWrite[i],2,12,0);
//	}
//	// չʾ��ȡ������
//	for (uint8_t i = 0;i < 4;i++) {
//		OLED_ShowNumber_Fast(0+16*i,49,arrRead[i],2,12,0);		
//	}

	
	OLED_Refresh(0);		//ˢ��	 	
}


void Write_Data_Once_second(void)
{
	//���÷���ÿһ����
	Read_attitude();	//��ȡ�Ƕȡ����ٶȡ����ٶ�
	Percentage_tx = Percentage_res();
	
//	if (cnt_Write_data>=52)cnt_Write_data = 52;	
	//ÿһ���һ������	

//	if (one_second_flag == 1&&cnt_Write_data<50)//ÿһ���һ������

	if (one_second_flag == 1)//ÿһ���һ������
	{
		//����if (one_second_flag == 1&&cnt_Write_data<50)�ж����Ȼ������ʱ�䲻����
		DS1302_read_realTime();//��ȡʱ��ֵ,
		
/*******************�˴�����ӱ�Ĥѹ�����������ж�*************************/		
	//�жϽǶ��ȶ��Ƿ񳬹�3�룬����Ƕȱ仯�����ұ�Ĥѹ���������ﵽһ��ֵ�����¼����		
  if(CheckAngleStability() == 0&&Voltage_value<1.9)
	{
		//����Ƕȱ仯������Ҫ�����˶��жϣ�ֻ�ڽ���˯��ģʽǰ�����ж�
		MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
		mpu6050_motion_interrupt_status = 0;
		

//		aacx = 4000;
//		aacy = 6000;
//		aacz = 7000;
//		gyrox = 2220;
//		gyroy = 1220;
//		gyroz = 3220;
//    pitch	= -24.6;	
//    roll	= -14.6;	
//    yaw	  = -78.3;	
//		Voltage_value = -3.3;
		
	  dataArray[0] = 0xAA;//֡ͷ
		dataArray[1] = TimeData.day;//����
		dataArray[2] = TimeData.hour;//Сʱ
		dataArray[3] = TimeData.minute;//����
		dataArray[4] = TimeData.second;//��	
		//���ٶ�
		dataArray[5] = (aacx >> 8) & 0xff;//aacx��8λ		  
		dataArray[6] = aacx & 0xff;//aacx��8λ		
		dataArray[7] = (aacy >> 8) & 0xff;//aacy��8λ		  
		dataArray[8] = aacy & 0xff;//aacy��8λ		
		dataArray[9] = (aacz >> 8) & 0xff;//aacz��8λ		  
		dataArray[10] = aacz & 0xff;//aacz��8λ		
		//���ٶ�
		dataArray[11] = (gyrox >> 8) & 0xff;//gyrox��8λ		  
		dataArray[12] = gyrox & 0xff;//gyrox��8λ		
		dataArray[13] = (gyroy >> 8) & 0xff;//gyroy��8λ		  
		dataArray[14] = gyroy & 0xff;//gyroy��8λ		
		dataArray[15] = (gyroz >> 8) & 0xff;//gyroz��8λ		  
		dataArray[16] = gyroz & 0xff;//gyroz��8λ		
		//�Ƕ�

		dataArray[17] = pByte[3];//pitch31~24λ	  
		dataArray[18] = pByte[2];//pitch23~16λ	
		dataArray[19] = pByte[1];//pitch15~8λ		  
		dataArray[20] = pByte[0];//pitch7~0λ	

		dataArray[21] = pByte1[3];//roll 31~24λ	  
		dataArray[22] = pByte1[2];//roll 23~16λ	
		dataArray[23] = pByte1[1];//roll 15~8λ		  
		dataArray[24] = pByte1[0];//-roll 7~0λ	

		dataArray[25] = pByte2[3];//yaw 31~24λ	  
		dataArray[26] = pByte2[2];//yaw 23~16λ	
		dataArray[27] = pByte2[1];//yaw 15~8λ		  
		dataArray[28] = pByte2[0];//yaw 7~0λ	
		//ѹ��ֵ

		dataArray[29] = pByte3[3];//Voltage_value31~24λ	  
		dataArray[30] = pByte3[2];//Voltage_value23~16λ	
		dataArray[31] = pByte3[1];//Voltage_value15~8λ		  
		dataArray[32] = pByte3[0];//Voltage_value7~0λ	

//		if(cnt_Write_data<50)//�޶��洢����,ֻ�ڲ���ʱ��Ҫ
//		{
			
			//�ж��Ƿ񳬹�оƬ������
		 if (currentAddress + ARRAY_SIZE <= W25Q64_FLASH_SIZE)
		 {
				uint32_t pageAddress = currentAddress / 256 * 256;//���㵱ǰ��ַ����ҳ����ʼ��ַ
				uint32_t offset = currentAddress % 256;//�����ڵ�ǰҳ�ڵ�ƫ����
				uint32_t bytesToWriteInCurrentPage = ARRAY_SIZE;
				if (offset + ARRAY_SIZE > 256)//�����ǰҳ�治����������
				{
						bytesToWriteInCurrentPage = 256 - offset; //���㵱ǰҳ��д����ֽ���
				}
				W25Q16_PageProgram(pageAddress + offset, &dataArray[0], bytesToWriteInCurrentPage);	//��д�뵱ǰҳ��������
				if (bytesToWriteInCurrentPage < ARRAY_SIZE)
				{
					//��ʣ������д����һҳ
					 W25Q16_PageProgram(pageAddress + 256, &dataArray[bytesToWriteInCurrentPage], ARRAY_SIZE - bytesToWriteInCurrentPage);			
				}
				 currentAddress += ARRAY_SIZE;  // ���´洢��ַ
		 }
			else //�ﵽоƬ������ֹͣд��
			{
				W25Q16_WriteDisable();
			}
			memset(dataArray, 0, ARRAY_SIZE);//�����������
//			cnt_Write_data ++;
//	 }		
	}
	else//����ǶȲ��䣬��Ĥѹ��������δ�ﵽһ��ֵ������˯��ģʽ
	{
		Time_Cnt_EnterSLEEPMode++;
		if(Time_Cnt_EnterSLEEPMode == 20)//
		{
			Time_Cnt_EnterSLEEPMode = 0;
			//�ڴ�ӡ�����У��������˶��жϼ�⣬Ҳ������˯��ģʽ����ӡ���˲Ž���
			if(Finish_print_flag == 1||Fisrt_print_flag == 0)//��һ�δ�ӡǰ���Խ���,��ӡ����Ҳ����͹���
			{
			printf("Entering low power mode as angle is stable.\n");					
			HAL_TIM_Base_Stop_IT(&htim2);	//�رն�ʱ���ж�					
//						HAL_NVIC_DisableIRQ(USART1_IRQn);//�رս����ж�
			if(	currentAddress >=W25Q64_FLASH_SIZE)//����Ѿ��ڴ�����,���ٿ����˶��жϼ��
			{
				MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
			}
			else
			{
				//�ڽ���˯��ģʽǰ�����˶��жϼ��
				MPU_INT_Init();	//�˶��жϼ�� �����Ĵ�������
			}
			HAL_SuspendTick();	//ֹͣϵͳ�δ�ʱ��
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);//stm32����˯��ģʽ��˯��ģʽ�£��ᱻ�κ��жϻ���
			HAL_ResumeTick();//�ͷŵδ�ʱ��		
//			//������һ���жϽ�stm32����֮����Ҫ���˶��жϼ���������ã���Ȼ��һֱ�����ж�
//			MPU_INT_Init();	//�˶��жϼ�� �����Ĵ�������
			}
			}
	}
    one_second_flag = 0;//10��֮1���־λ����	
	}
}
void Read_data_to_printf(void)
{	
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�	������ӡ�����ж�		 	
	//�����δ��ӡ����Fisrt_print_flag==0����ӡ��һ��֮��һֱ����1
	Fisrt_print_flag = 1;
//	if(cnt_printf>=52)cnt_printf=52;
	
//	if(cnt_printf<50)
//	{
		if (Read_currentAddress + Read_ARRAY_SIZE <= W25Q64_FLASH_SIZE)//���ڴ淶Χ�ڶ�ȡ����
		{
			W25Q16_ReadData(Read_currentAddress,&Read_dataArray[0],Read_ARRAY_SIZE);//ÿ�ν���ȡ�����ݴ����СΪ33��������
			//��W25Q16�ж�ȡ������ת��ΪҪ���͵�����
			for(uint8_t i=0;i<5;i++)//ǰ�������Ϊ�����ֽ�
			{
					Printf_dataArray[i] =	Read_dataArray[i];	//��0λΪ֡ͷ
			}
			for(uint8_t i=0;i<6;i++)// 16λ�ֽڵĸ߰�λ�͵Ͱ�λ����Ҫ����λ�͵�λ��Ϊһ������
			{
					Printf_dataArray[5+i] =	(Read_dataArray[5+i*2]<<8)+(Read_dataArray[6+i*2]);
			}
			
			for(uint8_t i=0;i<4;i++)
			{
					Printf_dataArray[11+i] =	(uint32_t)(Read_dataArray[17+i*4]<<24)|(uint32_t)(Read_dataArray[18+i*4]<<16)|(uint32_t)(Read_dataArray[19+i*4]<<8)|(uint32_t)(Read_dataArray[20+i*4]);

			}
			
			//ת��
			Print.day  = Printf_dataArray[1];
			Print.hour = Printf_dataArray[2];
			Print.min  = Printf_dataArray[3];
			Print.sec  = Printf_dataArray[4];
			
			Print.aacx = Printf_dataArray[5];
			Print.aacy = Printf_dataArray[6];
			Print.aacz = Printf_dataArray[7];

			Print.gyrox = Printf_dataArray[8];
			Print.gyroy = Printf_dataArray[9];
			Print.gyroz = Printf_dataArray[10];

			Print.pitch = -(*(float*)&(Printf_dataArray[11]));//-pitch
			Print.roll  = -(*(float*)&(Printf_dataArray[12]));//-roll
			Print.yaw   = -(*(float*)&(Printf_dataArray[13]));//-yaw
					
			Print.Vol_val = (*(float*)&(Printf_dataArray[14]));

	//		Print.real_aacx = (float)Print.aacx*(2-(-2))/16384;//��λΪ��ÿ��
	//		Print.real_aacy = (float)Print.aacy*(2-(-2))/16384;
	//		Print.real_aacz = (float)Print.aacz*(2-(-2))/16384;

			Print.real_aacx = (float)Print.aacx*9.8/16384;//��λΪ��ÿ��
			Print.real_aacy = (float)Print.aacy*9.8/16384;
			Print.real_aacz = (float)Print.aacz*9.8/16384;
			Print.real_gyrox = (float)Print.gyrox/16.4;   //��λΪ��ÿ��
			Print.real_gyroy = (float)Print.gyroy/16.4;
			Print.real_gyroz = (float)Print.gyroz/16.4;
			//����ѹת��Ϊ����
			Read_RES_value = ((Print.Vol_val*4095/3.3)*10000/(4095-(Print.Vol_val*4095/3.3)))-1000;
			//������ת��Ϊ����
			if((1000.0f/Read_RES_value)>0&&(1000.0f/Read_RES_value)<0.195)
			{
				Read_weight_g = 1896*(1000.0f/Read_RES_value)-64;//��ʽ1������Ĥѹ��������˵����
			}
			else if((1000.0f/Read_RES_value)>=0.195)
			{
				Read_weight_g = 3810*(1000.0f/Read_RES_value)-443;//��ʽ2
			}
			//������ת��Ϊ��
			Read_Force_N = Read_weight_g*0.2941/250;
		}
		//�����ȡ��ַ�ﵽ��󣬻��߶���0xff���������ӡ
//    if((Print.day == 0xFF&&Print.hour == 0xFF&&Print.min == 0xFF&&Print.sec == 0xFF)||Read_currentAddress>=W25Q64_FLASH_SIZE||cnt_printf == 49)
    if((Print.day == 0xFF&&Print.hour == 0xFF&&Print.min == 0xFF&&Print.sec == 0xFF)||Read_currentAddress>=W25Q64_FLASH_SIZE)
		{
			Finish_print_flag = 1;	//���Ҫ�ظ���ӡ������յ���λ�����͵�'1'ʱ��Ҫ��0		
//			if(Read_currentAddress>=W25Q64_FLASH_SIZE||cnt_printf == 49)

			if(Read_currentAddress>=W25Q64_FLASH_SIZE)
				Memory_Full_flag = 1;//�ڴ�����
			else
				Memory_Full_flag = 0;
		}
		if(Finish_print_flag == 0)//�������Ч�����ڣ����ӡ����
		{			
			Read_currentAddress+=Read_ARRAY_SIZE;//��ַÿ�μӶ�ȡ���ݵ�����Ĵ�С

/*			
			printf("0x%2X : \n%02dth /   %02d:%02d:%02d\n\n",Printf_dataArray[0] ,Print.day,Print.hour,Print.min,Print.sec);//һ���ӡһ��							
			printf("Angle:     %.2f     %.2f     %.2f     degrees\n\n",Print.pitch,Print.roll,Print.yaw);//һ���ӡһ��							
			printf("real_Acce:     %.2f     %.2f     %.2f     m/s^2\n\n",Print.real_aacx,Print.real_aacy,Print.real_aacz);//һ���ӡһ��							
			printf("real_Angular Velocity:     %.2f     %.2f     %.2f    degrees/s\n\n",Print.real_gyrox,Print.real_gyroy,Print.real_gyroz);//һ���ӡһ��							
	//		printf("Acce:     %02d     %02d     %02d\n\n",Print.aacx,Print.aacy,Print.aacz);//һ���ӡһ��							
	//		printf("Angular Velocity:      %2d       %02d     %02d\n\n",Print.gyrox,Print.gyroy,Print.gyroz);//һ���ӡһ��							
			printf("Voltage:    %.2f    V\n\n",Print.Vol_val);//һ���ӡһ��							
*/
			printf("\n0x%2X : \n%02dth / %02d:%02d:%02d ; ",Printf_dataArray[0] ,Print.day,Print.hour,Print.min,Print.sec);//һ���ӡһ��							
			printf("Angle:     %.2f     %.2f     %.2f     degrees ; ",Print.pitch,Print.roll,Print.yaw);//һ���ӡһ��							
			printf("Acce:     %.2f     %.2f     %.2f     m/s^2 ; ",Print.real_aacx,Print.real_aacy,Print.real_aacz);//һ���ӡһ��							
			printf("Angular Velocity:     %.2f     %.2f     %.2f    degrees/s ; ",Print.real_gyrox,Print.real_gyroy,Print.real_gyroz);//һ���ӡһ��							
	//		printf("Acce:     %02d     %02d     %02d\n\n",Print.aacx,Print.aacy,Print.aacz);//һ���ӡһ��							
	//		printf("Angular Velocity:      %2d       %02d     %02d\n\n",Print.gyrox,Print.gyroy,Print.gyroz);//һ���ӡһ��							
			printf("Read_Force_N:     %.2f    N  ;",Read_Force_N);//һ���ӡһ��							

		}
		else
			{
				if(Conclusion_print_Cnt<1)//ֻ��ӡһ��
				{
					//�ж��Ƿ����
					if(Memory_Full_flag == 1)
							printf("\n\n/********* The memory is Full!!!!!!  **********/\n");
					else
							printf("\n\n/********* Memory is not Full!!!!!!  **********/\n");
					
					//������ȫ����ӡ���!
					printf("\n/*********************  The data has all been printed out!  *********************/\n\n");
					Conclusion_print_Cnt ++;					
				}
			}
			memset(Printf_dataArray, 0, Printf_ARRAY_SIZE);
			memset(Read_dataArray, 0, Read_ARRAY_SIZE);
//		cnt_printf ++;

//	}
//	cnt_printf ++;
	
}

uint8_t  CheckAngleStability(void)//��鵱ǰ�Ƕ��Ƿ��ȶ�
{
	current_angle[0] = pitch ;
	current_angle[1] = roll ;
	//�жϽǶ��Ƿ��ȶ�
	if (fabs(current_angle[0] - prev_angle[0]) < ANGLE_THRESHOLD &&
        fabs(current_angle[1] - prev_angle[1]) < ANGLE_THRESHOLD)
	{
		angle_stable_flag = 1;//�Ƕ��ȶ�
	}
	else
	{
		angle_stable_flag_3sec = 0;
		angle_stable_flag = 0;//�ǶȲ��ȶ�
		prev_angle[0] = current_angle[0];//������һ�νǶ�ֵ
		prev_angle[1] = current_angle[1];
	}
	if(angle_stable_flag_3sec == 1)
		
		return 1;
	else
	{
		prev_angle[0] = current_angle[0];//������һ�νǶ�ֵ
		prev_angle[1] = current_angle[1];		
		return 0;
	}
//	static uint8_t First_check = 1;
//	static uint8_t Cnt_angle_stable = 0;

//	current_angle[0] = pitch ;
//	current_angle[1] = roll ;
//	if(First_check == 1)//��ʾ��һ�μ��
//	{
//		First_check = 0;
//		prev_angle[0] = current_angle[0];//������һ�νǶ�ֵ
//		prev_angle[1] = current_angle[1];		
//	}
//	//�жϽǶ��Ƿ��ȶ�
//	if (fabs(current_angle[0] - prev_angle[0]) < ANGLE_THRESHOLD &&
//        fabs(current_angle[1] - prev_angle[1]) < ANGLE_THRESHOLD)
//	{
//		angle_stable_flag = 1;//�Ƕ��ȶ�
//	}
//	else
//	{
//		angle_stable_flag_3sec = 0;
//		angle_stable_flag = 0;//�ǶȲ��ȶ�
//		prev_angle[0] = current_angle[0];//������һ�νǶ�ֵ
//		prev_angle[1] = current_angle[1];
//	}
//	if(angle_stable_flag == 1)
//	{
//		Cnt_angle_stable ++;
//		if(Cnt_angle_stable>10)
//		{
//			angle_stable_flag_3sec = 1;
//			Cnt_angle_stable = 0;
//		}
////		else 
////		{
////			angle_stable_flag_3sec = 0;
////			Cnt_angle_stable=0;
////		}
//	}
//	else
//		{
//			angle_stable_flag_3sec = 0;
//			Cnt_angle_stable=0;
//		}
//	if(angle_stable_flag_3sec == 1)
////	if(angle_stable_flag == 1)
//		
//		return 1;
//	else
//	{
//		prev_angle[0] = current_angle[0];//������һ�νǶ�ֵ
//		prev_angle[1] = current_angle[1];		
//		return 0;
//	}
	
	
	
}