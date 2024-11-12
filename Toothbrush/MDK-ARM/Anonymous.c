#include "stm32f1xx_hal.h"
#include "Anonymous.h"
#include "usart.h"
#define BYTE0(dwTemp) (*(char *)(&dwTemp))//低位字节
#define BYTE1(dwTemp) (*((char *)(&dwTemp)+1))//中位字节
#define BYTE2(dwTemp) (*((char *)(&dwTemp)+2))//高位字节
#define BYTE3(dwTemp) (*((char *)(&dwTemp)+3))//最高位字节
	
uint8_t BUFF[100];//缓存区




void sent_data1(int  A,int B,int C,uint8_t D)
//发送三个2字节int类型，一个一字节uint8_t类型
{	
		uint8_t _cnt = 0;
		BUFF[_cnt++]=0xAA;//帧头
		BUFF[_cnt++]=0xFF;//目标地址
		BUFF[_cnt++]=0x03;//功能码(ID)
		BUFF[_cnt++]=7;//数据长度(字节)
	  //数据内容A(2字节)
		BUFF[_cnt++]=BYTE0(A);//低位
		BUFF[_cnt++]=BYTE1(A);//高位
	  //数据内容B(2字节)
		BUFF[_cnt++]=BYTE0(B);//低位
		BUFF[_cnt++]=BYTE1(B);//高位
	  //数据内容C(2字节)
		BUFF[_cnt++]=BYTE0(C);//低位
		BUFF[_cnt++]=BYTE1(C);//高位
	  //数据内容D(1字节)
		BUFF[_cnt++]=BYTE0(D);
	  	
		uint8_t sumcheck = 0;
		uint8_t addcheck = 0;
		
		for(uint16_t i=0;i<BUFF[3]+4;i++) //BUFF[3]为数据区长度，4为数据区前四位
		{
			sumcheck+=BUFF[i];
			addcheck+=sumcheck;
		}
		BUFF[_cnt++]=sumcheck;//和校验	
		BUFF[_cnt++]=addcheck;//附加校验		
		HAL_UART_Transmit(&huart1,BUFF,_cnt,0xffff);//串口发送
}

