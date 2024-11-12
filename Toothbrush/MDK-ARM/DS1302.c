#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "delay.h"


uint8_t  read_time[7];
struct TIMEData TimeData;
struct TIMERAM TimeRAM;
//配置双向I/O端口为输出态
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

//配置双向I/O端口为输入态
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
 * 向DS1302发送一个字节
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param data 要传入的字节
 * @param  
 */


void DS1302_write_onebyte(uint8_t data)
{
		uint8_t count=0;
		SCLK_L;     //拉低时钟线
		DS1302_DATAOUT_init(); //设置I/O引脚为输出模式，即向DS1302写数据
 
	 for(count=0;count<8;count++)
		{	
			SCLK_L;    //拉低时钟线
			if(data&0x01)//判断最低位是否为1，若为1，则将I/O数据引脚发出高电平，反之，低电平
			{				
			  DATA_H;
			}
			else{
				DATA_L;
			}//准备好数据再发送
			SCLK_H;//拉高时钟线
			data>>=1;//右移
		}
}
/**
 * 从指定地址写入字节
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param data 要写入的字节
 * @param address 指定地址
 */

void DS1302_wirte_rig(uint8_t address,uint8_t data)
{
	uint8_t temp1=address;
	uint8_t temp2=data;
	CE_L;SCLK_L;delay_us(1);//使能位CE,时钟线SCLK都置低电平
	CE_H;delay_us(3);//写入数据时，CE引脚要配置高电平
	DS1302_write_onebyte(temp1);//写地址
	DS1302_write_onebyte(temp2);//写数据
	CE_L;SCLK_L;delay_us(3);  //写完数据，CE引脚要配置低电平
}
/**
 * 从指定地址读取一个字节
 * 
 * 
 * DS1302_write_onebyte(uint8_t data)
 * @param address 指定地址
 */

uint8_t DS1302_read_rig(uint8_t address)
{
	uint8_t temp3=address;
	uint8_t count=0;
	uint8_t return_data=0x00;
	CE_L;SCLK_L;delay_us(3);//使电平持续一段时间
	CE_H;delay_us(3);       //读取数据时，CE引脚要配置高电平
	DS1302_write_onebyte(temp3);//写入要读取数据的地址
	DS1302_DATAINPUT_init();//配置I/O为输入
	delay_us(3);
	for(count=0;count<8;count++)
		{
		delay_us(3);//使电平持续一段时间
		return_data>>=1;//右移一位，
		SCLK_H;delay_us(5);//使高电平持续一段时间
		SCLK_L;delay_us(30);//延时14us后再读取电压，更准确
		if(HAL_GPIO_ReadPin(DS1302_DATA_PORT,DS1302_DATA_PIN))//如果读到某一位为高电平，则该位置一
		{return_data=return_data|0x80;}//将该位置1
		
		}
	delay_us(2);
	CE_L;DATA_L;//读完数据，使能位CE引脚要配置低电平
	return return_data;
}

//DS1302初始化，设置当前时间

void DS1302_Init(void)
{
	DS1302_wirte_rig(0x8e,0x00);//关闭写保护
	DS1302_wirte_rig(0x80,0x00);//seconds ：00秒
	DS1302_wirte_rig(0x82,0x04);//minutes : 04分
	DS1302_wirte_rig(0x84,0x21);//hours   ：21时
	DS1302_wirte_rig(0x86,0x08);//date    ：8日
	DS1302_wirte_rig(0x88,0x11);//months  ：11月
	DS1302_wirte_rig(0x8a,0x05);//days    ：星期五
	DS1302_wirte_rig(0x8c,0x24);//year    ：2024年
	DS1302_wirte_rig(0x8e,0x80);//开启写保护
}
//从DS1302读取实时时间（BCD码）
void DS1302_read_time(void)
{
	read_time[0]=DS1302_read_rig(0x81);//读秒
	read_time[1]=DS1302_read_rig(0x83);//读分
	read_time[2]=DS1302_read_rig(0x85);//读时
	read_time[3]=DS1302_read_rig(0x87);//读日
	read_time[4]=DS1302_read_rig(0x89);//读月
	read_time[5]=DS1302_read_rig(0x8B);//读星期
	read_time[6]=DS1302_read_rig(0x8D);//读年
}

//将实时时间由BCD码转为十进制

void DS1302_read_realTime(void)
{
	DS1302_read_time();  //BCD码转为十进制
	TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
	TimeData.minute=((read_time[1]>>4))*10+(read_time[1]&0x0f);
	TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
	TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
	TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
	TimeData.week=read_time[5];
	TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;	
}

//向RAM中写东西

void DS1302_wirteRAM(void)
{
	DS1302_wirte_rig(0x8e,0x00);//关闭写保护
	DS1302_wirte_rig(0xC0,TimeRAM.hour_kai);//开时
	DS1302_wirte_rig(0xC2,TimeRAM.minute_kai);//开分
	DS1302_wirte_rig(0xC4,TimeRAM.hour_guan);//关时 
	DS1302_wirte_rig(0xC6,TimeRAM.minute_guan);//关分
	DS1302_wirte_rig(0xC8,TimeRAM.kai);//关分
	DS1302_wirte_rig(0xCA,TimeRAM.guan);//关分
	DS1302_wirte_rig(0x8e,0x80);//开启写保护
}
void DS1302_readRAM(void)
{
	TimeRAM.hour_kai=DS1302_read_rig(0xC1);//读秒
	TimeRAM.minute_kai=DS1302_read_rig(0xC3);//读分
	TimeRAM.hour_guan=DS1302_read_rig(0xC5);//读时
	TimeRAM.minute_guan=DS1302_read_rig(0xC7);//??	
	TimeRAM.kai=DS1302_read_rig(0xC9);//读日
	TimeRAM.guan=DS1302_read_rig(0xCB);//读日
}
