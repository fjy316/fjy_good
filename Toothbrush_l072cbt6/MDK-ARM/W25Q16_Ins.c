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

/**********打印到串口助手数据结构体************/
struct PRINTF_Data
{
	uint8_t day,hour,min,sec; 	//日，时，分，秒
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;			//加速度
	short gyrox,gyroy,gyroz;	//角速度
	float Vol_val;      //电压
	
  float real_aacx,real_aacy,real_aacz;//单位为重力加速度g
  float real_gyrox,real_gyroy,real_gyroz;//单位为 °/s	
};
struct PRINTF_Data Print;

//第一次打印标志位
uint8_t Fisrt_print_flag = 0;
//打印完有效数据标志位，如果内存用完，那么在最大内存范围内的数据为有效数据，
//如果未存满，那么在读取到Print.day = 0xFF,Print.hour = 0xFF,Print.min = 0xFF,Print.sec = 0xFF,之前的数据为有效数据
uint8_t Finish_print_flag = 0;
//打印一次结束后，结束语的打印次数变量
uint8_t Conclusion_print_Cnt = 0;
//内存已满标志位
uint8_t Memory_Full_flag = 0;
//读取出来的电阻变量,float型用于后续计算方便
float  Read_RES_value = 0;
//读取出来的重量的变量，单位为 g
float  Read_weight_g = 0;
//读取的力的变量 ，单位为 N
float  Read_Force_N= 0;
/*********************************************/

/*******角度检测变量********/
float prev_angle[2] = {0}; //存储上一次的角度值
float current_angle[2]; //存储当前角度值
uint8_t angle_stable_flag = 0; //角度稳定标志位
uint8_t angle_stable_flag_3sec = 0; //五秒角度稳定标志位
#define ANGLE_THRESHOLD 1.5 //角度门槛	
/**************************/

uint16_t cnt_Write_data = 0;//只记录cnt_test秒的数据
uint16_t cnt_printf=0;

extern uint8_t Grame[8][128] ;
extern uint8_t arrWrite[];//要写入的数据
extern uint8_t arrRead[10];  //存储读取到的数据的数组
extern uint8_t MID;  //工厂ID
extern uint16_t DID;//设备ID
extern uint8_t one_second_flag;//定时器一秒标志位
extern  float pitch,roll,yaw; 		    //y-?
extern  short aacx,aacy,aacz;			//???????'??
extern  short gyrox,gyroy,gyroz;		//G???'??
extern int  Percentage_tx;
extern float Voltage_value;
extern uint8_t mpu6050_motion_interrupt_status;
//通过指针强制类型转换来拆分float
uint8_t* pByte = (uint8_t*)&pitch;
uint8_t* pByte1 = (uint8_t*)&roll;
uint8_t* pByte2 = (uint8_t*)&yaw;
uint8_t* pByte3 = (uint8_t*)&Voltage_value;
//每100ms判断一次角度稳定，要想5秒角度稳定，需要(10*Time_Cnt_EnterSLEEPMode)次
uint8_t Time_Cnt_EnterSLEEPMode = 0;


//将数组中的数据存入W25Q16中
#define ARRAY_SIZE 33  //数组大小
uint8_t dataArray[ARRAY_SIZE];  // 要存储的数据数组
uint32_t currentAddress = 0;  // 当前在W25Q16的存储地址，初始化为0
//#define W25Q16_FLASH_SIZE 0x200000  //W25Q16芯片容量
//从W25Q16中读取数据存入数组
#define Read_ARRAY_SIZE 33  //数组大小
uint8_t Read_dataArray[Read_ARRAY_SIZE];  // 要存储的数据数组
uint32_t Read_currentAddress = 0;  // 当前在W25Q16的存储地址，初始化为0
//从W25Q16中读取数据存入数组
#define Printf_ARRAY_SIZE 15  //数组大小
uint32_t Printf_dataArray[Read_ARRAY_SIZE];  // 要存储的数据数组
uint32_t Read_FinalAddress = 0x0000000+33*20;  // 数据存储的最后一个地址


////设置的最大读取地址，在这个值范围以内，都可以读取
//#define Max_Set_Read_ARRAY 33*50+0x000000

void Show_W25Q16_Data(void)
{
  memset(Grame, 0, 8*128);//清空数组里的元素
	W25Q16_ReadID(&MID, &DID);
	OLED_ShowNumber_Fast(0,0,MID,4,12,0);
	OLED_ShowNumber_Fast(0,16,DID,5,12,0);
  W25Q16_SectorErase(0x000000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//	store_data_to_W25Q16(0x000000,1234567,-10,20,0);
	W25Q16_PageProgram(0x000000, arrWrite, 4);//写入
	W25Q16_ReadData(0x000000, arrRead, 10);//读取
	// 展示发送的数据
//	for (uint8_t i = 0;i < 4;i++) {
//		OLED_ShowNumber_Fast(0+16*i,30,arrWrite[i],2,12,0);
//	}
//	// 展示读取的数据
//	for (uint8_t i = 0;i < 4;i++) {
//		OLED_ShowNumber_Fast(0+16*i,49,arrRead[i],2,12,0);		
//	}

	
	OLED_Refresh(0);		//刷新	 	
}


void Write_Data_Once_second(void)
{
	//不用放入每一秒里
	Read_attitude();	//读取角度、角速度、加速度
	Percentage_tx = Percentage_res();
	
//	if (cnt_Write_data>=52)cnt_Write_data = 52;	
	//每一秒存一次数据	

//	if (one_second_flag == 1&&cnt_Write_data<50)//每一秒存一次数据

	if (one_second_flag == 1)//每一秒存一次数据
	{
		//放在if (one_second_flag == 1&&cnt_Write_data<50)判断里，不然读出的时间不连续
		DS1302_read_realTime();//获取时间值,
		
/*******************此处还需加薄膜压力传感器的判断*************************/		
	//判断角度稳定是否超过3秒，如果角度变化，并且薄膜压力传感器达到一定值，则记录数据		
  if(CheckAngleStability() == 0&&Voltage_value<1.9)
	{
		//如果角度变化，则不需要开启运动中断，只在进入睡眠模式前开启中断
		MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
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
		
	  dataArray[0] = 0xAA;//帧头
		dataArray[1] = TimeData.day;//日期
		dataArray[2] = TimeData.hour;//小时
		dataArray[3] = TimeData.minute;//分钟
		dataArray[4] = TimeData.second;//秒	
		//加速度
		dataArray[5] = (aacx >> 8) & 0xff;//aacx高8位		  
		dataArray[6] = aacx & 0xff;//aacx低8位		
		dataArray[7] = (aacy >> 8) & 0xff;//aacy高8位		  
		dataArray[8] = aacy & 0xff;//aacy低8位		
		dataArray[9] = (aacz >> 8) & 0xff;//aacz高8位		  
		dataArray[10] = aacz & 0xff;//aacz低8位		
		//角速度
		dataArray[11] = (gyrox >> 8) & 0xff;//gyrox高8位		  
		dataArray[12] = gyrox & 0xff;//gyrox低8位		
		dataArray[13] = (gyroy >> 8) & 0xff;//gyroy高8位		  
		dataArray[14] = gyroy & 0xff;//gyroy低8位		
		dataArray[15] = (gyroz >> 8) & 0xff;//gyroz高8位		  
		dataArray[16] = gyroz & 0xff;//gyroz低8位		
		//角度

		dataArray[17] = pByte[3];//pitch31~24位	  
		dataArray[18] = pByte[2];//pitch23~16位	
		dataArray[19] = pByte[1];//pitch15~8位		  
		dataArray[20] = pByte[0];//pitch7~0位	

		dataArray[21] = pByte1[3];//roll 31~24位	  
		dataArray[22] = pByte1[2];//roll 23~16位	
		dataArray[23] = pByte1[1];//roll 15~8位		  
		dataArray[24] = pByte1[0];//-roll 7~0位	

		dataArray[25] = pByte2[3];//yaw 31~24位	  
		dataArray[26] = pByte2[2];//yaw 23~16位	
		dataArray[27] = pByte2[1];//yaw 15~8位		  
		dataArray[28] = pByte2[0];//yaw 7~0位	
		//压力值

		dataArray[29] = pByte3[3];//Voltage_value31~24位	  
		dataArray[30] = pByte3[2];//Voltage_value23~16位	
		dataArray[31] = pByte3[1];//Voltage_value15~8位		  
		dataArray[32] = pByte3[0];//Voltage_value7~0位	

//		if(cnt_Write_data<50)//限定存储容量,只在测试时需要
//		{
			
			//判断是否超过芯片总容量
		 if (currentAddress + ARRAY_SIZE <= W25Q64_FLASH_SIZE)
		 {
				uint32_t pageAddress = currentAddress / 256 * 256;//计算当前地址所在页的起始地址
				uint32_t offset = currentAddress % 256;//计算在当前页内的偏移量
				uint32_t bytesToWriteInCurrentPage = ARRAY_SIZE;
				if (offset + ARRAY_SIZE > 256)//如果当前页存不下整个数组
				{
						bytesToWriteInCurrentPage = 256 - offset; //计算当前页能写入的字节数
				}
				W25Q16_PageProgram(pageAddress + offset, &dataArray[0], bytesToWriteInCurrentPage);	//先写入当前页部分数据
				if (bytesToWriteInCurrentPage < ARRAY_SIZE)
				{
					//将剩余数据写入下一页
					 W25Q16_PageProgram(pageAddress + 256, &dataArray[bytesToWriteInCurrentPage], ARRAY_SIZE - bytesToWriteInCurrentPage);			
				}
				 currentAddress += ARRAY_SIZE;  // 更新存储地址
		 }
			else //达到芯片容量，停止写入
			{
				W25Q16_WriteDisable();
			}
			memset(dataArray, 0, ARRAY_SIZE);//清空数组数据
//			cnt_Write_data ++;
//	 }		
	}
	else//如果角度不变，或薄膜压力传感器未达到一定值，进入睡眠模式
	{
		Time_Cnt_EnterSLEEPMode++;
		if(Time_Cnt_EnterSLEEPMode == 20)//
		{
			Time_Cnt_EnterSLEEPMode = 0;
			//在打印过程中，不开启运动中断检测，也不进入睡眠模式，打印完了才进入
			if(Finish_print_flag == 1||Fisrt_print_flag == 0)//第一次打印前可以进入,打印结束也进入低功耗
			{
			printf("Entering low power mode as angle is stable.\n");					
			HAL_TIM_Base_Stop_IT(&htim2);	//关闭定时器中断					
//						HAL_NVIC_DisableIRQ(USART1_IRQn);//关闭接收中断
			if(	currentAddress >=W25Q64_FLASH_SIZE)//如果已经内存满了,不再开启运动中断检测
			{
				MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
			}
			else
			{
				//在进入睡眠模式前开启运动中断检测
				MPU_INT_Init();	//运动中断检测 ——寄存器配置
			}
			HAL_SuspendTick();	//停止系统滴答定时器
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);//stm32进入睡眠模式，睡眠模式下，会被任何中断唤醒
			HAL_ResumeTick();//释放滴答定时器		
//			//触发第一次中断将stm32唤醒之后，需要将运动中断检测重新配置，不然会一直触发中断
//			MPU_INT_Init();	//运动中断检测 ——寄存器配置
			}
			}
	}
    one_second_flag = 0;//10分之1秒标志位置零	
	}
}
void Read_data_to_printf(void)
{	
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断	，当打印完后打开中断		 	
	//如果还未打印，则Fisrt_print_flag==0，打印了一次之后，一直等于1
	Fisrt_print_flag = 1;
//	if(cnt_printf>=52)cnt_printf=52;
	
//	if(cnt_printf<50)
//	{
		if (Read_currentAddress + Read_ARRAY_SIZE <= W25Q64_FLASH_SIZE)//在内存范围内读取数据
		{
			W25Q16_ReadData(Read_currentAddress,&Read_dataArray[0],Read_ARRAY_SIZE);//每次将读取的数据存入大小为33的数组中
			//将W25Q16中读取的数据转化为要发送的数据
			for(uint8_t i=0;i<5;i++)//前五个数据为单个字节
			{
					Printf_dataArray[i] =	Read_dataArray[i];	//第0位为帧头
			}
			for(uint8_t i=0;i<6;i++)// 16位字节的高八位和低八位，需要将高位和低位合为一个数据
			{
					Printf_dataArray[5+i] =	(Read_dataArray[5+i*2]<<8)+(Read_dataArray[6+i*2]);
			}
			
			for(uint8_t i=0;i<4;i++)
			{
					Printf_dataArray[11+i] =	(uint32_t)(Read_dataArray[17+i*4]<<24)|(uint32_t)(Read_dataArray[18+i*4]<<16)|(uint32_t)(Read_dataArray[19+i*4]<<8)|(uint32_t)(Read_dataArray[20+i*4]);

			}
			
			//转换
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

	//		Print.real_aacx = (float)Print.aacx*(2-(-2))/16384;//单位为米每秒
	//		Print.real_aacy = (float)Print.aacy*(2-(-2))/16384;
	//		Print.real_aacz = (float)Print.aacz*(2-(-2))/16384;

			Print.real_aacx = (float)Print.aacx*9.8/16384;//单位为米每秒
			Print.real_aacy = (float)Print.aacy*9.8/16384;
			Print.real_aacz = (float)Print.aacz*9.8/16384;
			Print.real_gyrox = (float)Print.gyrox/16.4;   //单位为度每秒
			Print.real_gyroy = (float)Print.gyroy/16.4;
			Print.real_gyroz = (float)Print.gyroz/16.4;
			//将电压转换为电阻
			Read_RES_value = ((Print.Vol_val*4095/3.3)*10000/(4095-(Print.Vol_val*4095/3.3)))-1000;
			//将电阻转换为重量
			if((1000.0f/Read_RES_value)>0&&(1000.0f/Read_RES_value)<0.195)
			{
				Read_weight_g = 1896*(1000.0f/Read_RES_value)-64;//公式1，看薄膜压力传感器说明书
			}
			else if((1000.0f/Read_RES_value)>=0.195)
			{
				Read_weight_g = 3810*(1000.0f/Read_RES_value)-443;//公式2
			}
			//将重量转换为力
			Read_Force_N = Read_weight_g*0.2941/250;
		}
		//如果读取地址达到最大，或者读到0xff，则结束打印
//    if((Print.day == 0xFF&&Print.hour == 0xFF&&Print.min == 0xFF&&Print.sec == 0xFF)||Read_currentAddress>=W25Q64_FLASH_SIZE||cnt_printf == 49)
    if((Print.day == 0xFF&&Print.hour == 0xFF&&Print.min == 0xFF&&Print.sec == 0xFF)||Read_currentAddress>=W25Q64_FLASH_SIZE)
		{
			Finish_print_flag = 1;	//如果要重复打印，则接收到上位机发送的'1'时，要置0		
//			if(Read_currentAddress>=W25Q64_FLASH_SIZE||cnt_printf == 49)

			if(Read_currentAddress>=W25Q64_FLASH_SIZE)
				Memory_Full_flag = 1;//内存已满
			else
				Memory_Full_flag = 0;
		}
		if(Finish_print_flag == 0)//如果在有效数据内，则打印数据
		{			
			Read_currentAddress+=Read_ARRAY_SIZE;//地址每次加读取数据的数组的大小

/*			
			printf("0x%2X : \n%02dth /   %02d:%02d:%02d\n\n",Printf_dataArray[0] ,Print.day,Print.hour,Print.min,Print.sec);//一秒打印一次							
			printf("Angle:     %.2f     %.2f     %.2f     degrees\n\n",Print.pitch,Print.roll,Print.yaw);//一秒打印一次							
			printf("real_Acce:     %.2f     %.2f     %.2f     m/s^2\n\n",Print.real_aacx,Print.real_aacy,Print.real_aacz);//一秒打印一次							
			printf("real_Angular Velocity:     %.2f     %.2f     %.2f    degrees/s\n\n",Print.real_gyrox,Print.real_gyroy,Print.real_gyroz);//一秒打印一次							
	//		printf("Acce:     %02d     %02d     %02d\n\n",Print.aacx,Print.aacy,Print.aacz);//一秒打印一次							
	//		printf("Angular Velocity:      %2d       %02d     %02d\n\n",Print.gyrox,Print.gyroy,Print.gyroz);//一秒打印一次							
			printf("Voltage:    %.2f    V\n\n",Print.Vol_val);//一秒打印一次							
*/
			printf("\n0x%2X : \n%02dth / %02d:%02d:%02d ; ",Printf_dataArray[0] ,Print.day,Print.hour,Print.min,Print.sec);//一秒打印一次							
			printf("Angle:     %.2f     %.2f     %.2f     degrees ; ",Print.pitch,Print.roll,Print.yaw);//一秒打印一次							
			printf("Acce:     %.2f     %.2f     %.2f     m/s^2 ; ",Print.real_aacx,Print.real_aacy,Print.real_aacz);//一秒打印一次							
			printf("Angular Velocity:     %.2f     %.2f     %.2f    degrees/s ; ",Print.real_gyrox,Print.real_gyroy,Print.real_gyroz);//一秒打印一次							
	//		printf("Acce:     %02d     %02d     %02d\n\n",Print.aacx,Print.aacy,Print.aacz);//一秒打印一次							
	//		printf("Angular Velocity:      %2d       %02d     %02d\n\n",Print.gyrox,Print.gyroy,Print.gyroz);//一秒打印一次							
			printf("Read_Force_N:     %.2f    N  ;",Read_Force_N);//一秒打印一次							

		}
		else
			{
				if(Conclusion_print_Cnt<1)//只打印一次
				{
					//判断是否存满
					if(Memory_Full_flag == 1)
							printf("\n\n/********* The memory is Full!!!!!!  **********/\n");
					else
							printf("\n\n/********* Memory is not Full!!!!!!  **********/\n");
					
					//数据已全部打印完毕!
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

uint8_t  CheckAngleStability(void)//检查当前角度是否稳定
{
	current_angle[0] = pitch ;
	current_angle[1] = roll ;
	//判断角度是否稳定
	if (fabs(current_angle[0] - prev_angle[0]) < ANGLE_THRESHOLD &&
        fabs(current_angle[1] - prev_angle[1]) < ANGLE_THRESHOLD)
	{
		angle_stable_flag = 1;//角度稳定
	}
	else
	{
		angle_stable_flag_3sec = 0;
		angle_stable_flag = 0;//角度不稳定
		prev_angle[0] = current_angle[0];//更新上一次角度值
		prev_angle[1] = current_angle[1];
	}
	if(angle_stable_flag_3sec == 1)
		
		return 1;
	else
	{
		prev_angle[0] = current_angle[0];//更新上一次角度值
		prev_angle[1] = current_angle[1];		
		return 0;
	}
//	static uint8_t First_check = 1;
//	static uint8_t Cnt_angle_stable = 0;

//	current_angle[0] = pitch ;
//	current_angle[1] = roll ;
//	if(First_check == 1)//表示第一次检测
//	{
//		First_check = 0;
//		prev_angle[0] = current_angle[0];//更新上一次角度值
//		prev_angle[1] = current_angle[1];		
//	}
//	//判断角度是否稳定
//	if (fabs(current_angle[0] - prev_angle[0]) < ANGLE_THRESHOLD &&
//        fabs(current_angle[1] - prev_angle[1]) < ANGLE_THRESHOLD)
//	{
//		angle_stable_flag = 1;//角度稳定
//	}
//	else
//	{
//		angle_stable_flag_3sec = 0;
//		angle_stable_flag = 0;//角度不稳定
//		prev_angle[0] = current_angle[0];//更新上一次角度值
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
//		prev_angle[0] = current_angle[0];//更新上一次角度值
//		prev_angle[1] = current_angle[1];		
//		return 0;
//	}
	
	
	
}