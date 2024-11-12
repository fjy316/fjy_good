#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "delay.h"


uint8_t  read_time[7];
struct TIMEData TimeData;
struct TIMERAM TimeRAM;
//����˫��I/O�˿�Ϊ���̬
void DS1302_DATAOUT_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(DS1302_DATA_PORT, DS1302_DATA_PIN, GPIO_PIN_RESET);		
	 
  GPIO_InitStruct.Pin = DS1302_DATA_PIN;//DATA
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS1302_DATA_PORT, &GPIO_InitStruct);
}

//����˫��I/O�˿�Ϊ����̬
void DS1302_DATAINPUT_init(void)
{	
		GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = DS1302_DATA_PIN;//DATA
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	
  HAL_GPIO_Init(DS1302_DATA_PORT, &GPIO_InitStruct);
}

/**
 * ��DS1302����һ���ֽ�
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param data Ҫ������ֽ�
 * @param  
 */


void DS1302_write_onebyte(uint8_t data)
{
		uint8_t count=0;
		SCLK_L;     //����ʱ����
		DS1302_DATAOUT_init(); //����I/O����Ϊ���ģʽ������DS1302д����
 
	 for(count=0;count<8;count++)
		{	
			SCLK_L;    //����ʱ����
			if(data&0x01)//�ж����λ�Ƿ�Ϊ1����Ϊ1����I/O�������ŷ����ߵ�ƽ����֮���͵�ƽ
			{				
			  DATA_H;
			}
			else{
				DATA_L;
			}//׼���������ٷ���
			SCLK_H;//����ʱ����
			data>>=1;//����
		}
}
/**
 * ��ָ����ַд���ֽ�
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param data Ҫд����ֽ�
 * @param address ָ����ַ
 */

void DS1302_wirte_rig(uint8_t address,uint8_t data)
{
	uint8_t temp1=address;
	uint8_t temp2=data;
	CE_L;SCLK_L;delay_us(1);//ʹ��λCE,ʱ����SCLK���õ͵�ƽ
	CE_H;delay_us(3);//д������ʱ��CE����Ҫ���øߵ�ƽ
	DS1302_write_onebyte(temp1);//д��ַ
	DS1302_write_onebyte(temp2);//д����
	CE_L;SCLK_L;delay_us(3);  //д�����ݣ�CE����Ҫ���õ͵�ƽ
}
/**
 * ��ָ����ַ��ȡһ���ֽ�
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param address ָ����ַ
 */

uint8_t DS1302_read_rig(uint8_t address)
{
	uint8_t temp3=address;
	uint8_t count=0;
	uint8_t return_data=0x00;
	CE_L;SCLK_L;delay_us(3);//ʹ��ƽ����һ��ʱ��
	CE_H;delay_us(3);       //��ȡ����ʱ��CE����Ҫ���øߵ�ƽ
	DS1302_write_onebyte(temp3);//д��Ҫ��ȡ���ݵĵ�ַ
	DS1302_DATAINPUT_init();//����I/OΪ����
	delay_us(3);
	for(count=0;count<8;count++)
		{
		delay_us(3);//ʹ��ƽ����һ��ʱ��
		return_data>>=1;//����һλ��
		SCLK_H;delay_us(5);//ʹ�ߵ�ƽ����һ��ʱ��
		SCLK_L;delay_us(30);//��ʱ14us���ٶ�ȡ��ѹ����׼ȷ
		if(HAL_GPIO_ReadPin(DS1302_DATA_PORT,DS1302_DATA_PIN))//�������ĳһλΪ�ߵ�ƽ�����λ��һ
		{return_data=return_data|0x80;}//����λ��1
		
		}
	delay_us(2);
	CE_L;DATA_L;//�������ݣ�ʹ��λCE����Ҫ���õ͵�ƽ
	return return_data;
}

//DS1302��ʼ�������õ�ǰʱ��

void DS1302_Init(void)
{
	DS1302_wirte_rig(0x8e,0x00);//�ر�д����
	DS1302_wirte_rig(0x80,0x00);//seconds ��00��
	DS1302_wirte_rig(0x82,0x04);//minutes : 04��
	DS1302_wirte_rig(0x84,0x21);//hours   ��21ʱ
	DS1302_wirte_rig(0x86,0x08);//date    ��8��
	DS1302_wirte_rig(0x88,0x11);//months  ��11��
	DS1302_wirte_rig(0x8a,0x05);//days    ��������
	DS1302_wirte_rig(0x8c,0x24);//year    ��2024��
	DS1302_wirte_rig(0x8e,0x80);//����д����
}
//��DS1302��ȡʵʱʱ�䣨BCD�룩
void DS1302_read_time(void)
{
	read_time[0]=DS1302_read_rig(0x81);//����
	read_time[1]=DS1302_read_rig(0x83);//����
	read_time[2]=DS1302_read_rig(0x85);//��ʱ
	read_time[3]=DS1302_read_rig(0x87);//����
	read_time[4]=DS1302_read_rig(0x89);//����
	read_time[5]=DS1302_read_rig(0x8B);//������
	read_time[6]=DS1302_read_rig(0x8D);//����
}

//��ʵʱʱ����BCD��תΪʮ����

void DS1302_read_realTime(void)
{
	DS1302_read_time();  //BCD��תΪʮ����
	TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
	TimeData.minute=((read_time[1]>>4))*10+(read_time[1]&0x0f);
	TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
	TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
	TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
	TimeData.week=read_time[5];
	TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;	
}

//��RAM��д����

void DS1302_wirteRAM(void)
{
	DS1302_wirte_rig(0x8e,0x00);//�ر�д����
	DS1302_wirte_rig(0xC0,TimeRAM.hour_kai);//��ʱ
	DS1302_wirte_rig(0xC2,TimeRAM.minute_kai);//����
	DS1302_wirte_rig(0xC4,TimeRAM.hour_guan);//��ʱ 
	DS1302_wirte_rig(0xC6,TimeRAM.minute_guan);//�ط�
	DS1302_wirte_rig(0xC8,TimeRAM.kai);//�ط�
	DS1302_wirte_rig(0xCA,TimeRAM.guan);//�ط�
	DS1302_wirte_rig(0x8e,0x80);//����д����
}
void DS1302_readRAM(void)
{
	TimeRAM.hour_kai=DS1302_read_rig(0xC1);//����
	TimeRAM.minute_kai=DS1302_read_rig(0xC3);//����
	TimeRAM.hour_guan=DS1302_read_rig(0xC5);//��ʱ
	TimeRAM.minute_guan=DS1302_read_rig(0xC7);//??	
	TimeRAM.kai=DS1302_read_rig(0xC9);//����
	TimeRAM.guan=DS1302_read_rig(0xCB);//����
}
