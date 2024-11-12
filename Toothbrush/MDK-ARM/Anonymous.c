#include "stm32f1xx_hal.h"
#include "Anonymous.h"
#include "usart.h"
#define BYTE0(dwTemp) (*(char *)(&dwTemp))//��λ�ֽ�
#define BYTE1(dwTemp) (*((char *)(&dwTemp)+1))//��λ�ֽ�
#define BYTE2(dwTemp) (*((char *)(&dwTemp)+2))//��λ�ֽ�
#define BYTE3(dwTemp) (*((char *)(&dwTemp)+3))//���λ�ֽ�
	
uint8_t BUFF[100];//������




void sent_data1(int  A,int B,int C,uint8_t D)
//��������2�ֽ�int���ͣ�һ��һ�ֽ�uint8_t����
{	
		uint8_t _cnt = 0;
		BUFF[_cnt++]=0xAA;//֡ͷ
		BUFF[_cnt++]=0xFF;//Ŀ���ַ
		BUFF[_cnt++]=0x03;//������(ID)
		BUFF[_cnt++]=7;//���ݳ���(�ֽ�)
	  //��������A(2�ֽ�)
		BUFF[_cnt++]=BYTE0(A);//��λ
		BUFF[_cnt++]=BYTE1(A);//��λ
	  //��������B(2�ֽ�)
		BUFF[_cnt++]=BYTE0(B);//��λ
		BUFF[_cnt++]=BYTE1(B);//��λ
	  //��������C(2�ֽ�)
		BUFF[_cnt++]=BYTE0(C);//��λ
		BUFF[_cnt++]=BYTE1(C);//��λ
	  //��������D(1�ֽ�)
		BUFF[_cnt++]=BYTE0(D);
	  	
		uint8_t sumcheck = 0;
		uint8_t addcheck = 0;
		
		for(uint16_t i=0;i<BUFF[3]+4;i++) //BUFF[3]Ϊ���������ȣ�4Ϊ������ǰ��λ
		{
			sumcheck+=BUFF[i];
			addcheck+=sumcheck;
		}
		BUFF[_cnt++]=sumcheck;//��У��	
		BUFF[_cnt++]=addcheck;//����У��		
		HAL_UART_Transmit(&huart1,BUFF,_cnt,0xffff);//���ڷ���
}

