#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "W25Q16.h"
#include "W25Q16_Ins.h"
#include "string.h"
#include "math.h"

extern uint8_t Grame[8][128] ;
extern uint8_t arrWrite[];//要写入的数据
extern uint8_t arrRead[4];  //存储读取到的数据的数组
extern uint8_t MID;  //工厂ID
extern uint16_t DID;//设备ID


void Show_W25Q16_Data(void)
{
  memset(Grame, 0, 8*128);//清空数组里的元素
	W25Q16_ReadID(&MID, &DID);
	OLED_ShowNumber_Fast(0,0,MID,4,12,0);
	OLED_ShowNumber_Fast(0,16,DID,5,12,0);
//  W25Q16_SectorErase(0x000000);//扇区的地址取决于前三位,后面三位无论怎么变都是同一个扇区
//	W25Q16_PageProgram(0x000000, arrWrite, 4);//写入
	W25Q16_ReadData(0x000000, arrRead, 4);//读取
	// 展示发送的数据
	for (uint8_t i = 0;i < 4;i++) {
		OLED_ShowNumber_Fast(0+16*i,30,arrWrite[i],2,12,0);
	}
	// 展示读取的数据
	for (uint8_t i = 0;i < 4;i++) {
		OLED_ShowNumber_Fast(0+16*i,49,arrRead[i],2,12,0);		
	}

	
	OLED_Refresh(0);		//刷新	 	
}

