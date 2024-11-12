#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "DS1302.h"
#include "oled.h"
#include "Show_DS1302_Data.h"
#include "string.h"
#include "math.h"
extern uint8_t Grame[8][128] ;

/**
 * ��ʾDS1302��ʵʱʱ������
 * 
 * void Show_DS1302_Data( uint8_t x_date,uint8_t y_date,uint8_t x_time,uint8_t y_time)
 *
 * @param x_date ���ڵĺ����ʼֵ
 * @param y_date ���ڵ������ʼֵ
 * @param x_time ʱ��ĺ����ʼֵ
 * @param y_time ʱ��������ʼֵ
 */

void Show_DS1302_Data( uint8_t x_date,uint8_t y_date,uint8_t x_time,uint8_t y_time)
{
//	  //������memset(Grame, 0, 8*128);ǰ���һ�㣬�����׳���OLED���治��
//		DS1302_read_realTime(); //��ȡʮ���Ƶ�ʵʱʱ�������	
	
		memset(Grame, 0, 8*128);//����������Ԫ��
	
	  OLED_ShowNumber_Fast(x_date,y_date,TimeData.year,4,16,0);
		OLED_ShowString_Fast(x_date+32,y_date,"/",16,0,3);
	  OLED_ShowNumber_Fast(x_date+40,y_date,TimeData.month ,2,16,0);
		OLED_ShowString_Fast(x_date+56,y_date,"/",16,0,3);
	  OLED_ShowNumber_Fast(x_date+64,y_date,TimeData.day,2,16,0);
	  if(TimeData.month<10)	OLED_ShowNumber_Fast(x_date+40,y_date,0,1,16,0);//��0������' 3'��Ϊ'03'
	  if(TimeData.day<10)	OLED_ShowNumber_Fast(x_date+64,y_date,0,1,16,0);//��0������' 3'��Ϊ'03'
	  OLED_ShowNumber_Fast(x_time,y_time,TimeData.hour ,2,16,0);
	  OLED_ShowString_Fast(x_time+16,y_time,":",16,0,3);
	  OLED_ShowNumber_Fast(x_time+24,y_time,TimeData.minute  ,2,16,0);
		OLED_ShowString_Fast(x_time+40,y_time,":",16,0,3);
	  OLED_ShowNumber_Fast(x_time+48,y_time,TimeData.second ,2,16,0);
	  if(TimeData.hour<10)	OLED_ShowNumber_Fast(x_time+0,y_time,0,1,16,0);//��0������' 3'��Ϊ'03'	
	  if(TimeData.minute<10)	OLED_ShowNumber_Fast(x_time+24,y_time,0,1,16,0);//��0������' 3'��Ϊ'03'
	  if(TimeData.second<10)	OLED_ShowNumber_Fast(x_time+48,y_time,0,1,16,0);//��0������' 3'��Ϊ'03'	
    OLED_Refresh(0);		//ˢ��	 	
}
