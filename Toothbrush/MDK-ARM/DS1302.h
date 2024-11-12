#ifndef _DS1302_H
#define _DS1302_H


struct TIMEData
{
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  week;
};//创建TIMEData结构体方便存储时间日期数据
 
struct TIMERAM
{
	uint8_t  hour_kai;
	uint8_t  minute_kai;
	uint8_t  hour_guan;
	uint8_t  minute_guan;
	uint8_t  kai;
	uint8_t  guan;
};//创建TIMERAM结构体


extern struct TIMERAM TimeRAM;
extern struct TIMEData TimeData;//全局变量


/**********DATA引脚定义**********/


#define DS1302_CE_PORT  				GPIOB
#define DS1302_CE_PIN						GPIO_PIN_10
#define DS1302_DATA_PORT				GPIOB
#define DS1302_DATA_PIN   			GPIO_PIN_1
#define DS1302_SCLK_PORT   			GPIOA
#define DS1302_SCLK_PIN  				GPIO_PIN_7

/**********引脚输出宏定义**********/

#define CE_L				 HAL_GPIO_WritePin(DS1302_CE_PORT, DS1302_CE_PIN, GPIO_PIN_RESET)//CE
#define CE_H				 HAL_GPIO_WritePin(DS1302_CE_PORT, DS1302_CE_PIN, GPIO_PIN_SET)
 
#define DATA_L			 HAL_GPIO_WritePin(DS1302_DATA_PORT, DS1302_DATA_PIN, GPIO_PIN_RESET)//DATA
#define DATA_H			 HAL_GPIO_WritePin(DS1302_DATA_PORT, DS1302_DATA_PIN, GPIO_PIN_SET)
 
#define SCLK_L			 HAL_GPIO_WritePin(DS1302_SCLK_PORT, DS1302_SCLK_PIN, GPIO_PIN_RESET)//SCLK
#define SCLK_H			 HAL_GPIO_WritePin(DS1302_SCLK_PORT, DS1302_SCLK_PIN, GPIO_PIN_SET)

void DS1302_DATAOUT_init(void);
void DS1302_DATAINPUT_init(void);
void DS1302_wirte_rig(uint8_t address,uint8_t data);
uint8_t DS1302_read_rig(uint8_t address);
void DS1302_Init(void);
void DS1302_read_time(void);
void DS1302_read_realTime(void);

void DS1302_wirteRAM(void);
void DS1302_readRAM(void);

#endif


