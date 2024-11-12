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
extern uint8_t arrWrite[];//Ҫд�������
extern uint8_t arrRead[4];  //�洢��ȡ�������ݵ�����
extern uint8_t MID;  //����ID
extern uint16_t DID;//�豸ID


void Show_W25Q16_Data(void)
{
  memset(Grame, 0, 8*128);//����������Ԫ��
	W25Q16_ReadID(&MID, &DID);
	OLED_ShowNumber_Fast(0,0,MID,4,12,0);
	OLED_ShowNumber_Fast(0,16,DID,5,12,0);
//  W25Q16_SectorErase(0x000000);//�����ĵ�ַȡ����ǰ��λ,������λ������ô�䶼��ͬһ������
//	W25Q16_PageProgram(0x000000, arrWrite, 4);//д��
	W25Q16_ReadData(0x000000, arrRead, 4);//��ȡ
	// չʾ���͵�����
	for (uint8_t i = 0;i < 4;i++) {
		OLED_ShowNumber_Fast(0+16*i,30,arrWrite[i],2,12,0);
	}
	// չʾ��ȡ������
	for (uint8_t i = 0;i < 4;i++) {
		OLED_ShowNumber_Fast(0+16*i,49,arrRead[i],2,12,0);		
	}

	
	OLED_Refresh(0);		//ˢ��	 	
}

