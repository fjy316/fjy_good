#include "oled.h"
#include "oledfont.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "Interface.h"
#include "string.h"
#include "math.h"
	
extern uint8_t Grame[8][128] ;
extern  float pitch,roll,yaw; 		    //y-?
extern  short aacx,aacy,aacz;			//???????'??
extern  short gyrox,gyroy,gyroz;		//G???'??
extern uint32_t RES_value;	
extern float Voltage_value;
extern int  Percentage_tx;

void Angle_Interface(void)
{
		memset(Grame, 0, 8*128);//清空数组里的元素
		OLED_Show_CH_Fast(0,0,85,12,0);//角
		OLED_Show_CH_Fast(24,0,87,12,0);//度		
		
    rocher_position(0,0,50,(roll+90)*48/180,(pitch-90)*48/180);
		OLED_ShowString_Fast(53,0,"R_x:",12,0,3);
	  OLED_ShowdecimalNumber_Fast(93,0,roll,2,1,12,0);
		OLED_ShowString_Fast(53,12,"P_y:",12,0,3);
	  OLED_ShowdecimalNumber_Fast(93,12,pitch,2,1,12,0);
		OLED_ShowString_Fast(53,24,"Yaw:",12,0,3);
	  OLED_ShowdecimalNumber_Fast(93,24,yaw,2,1,12,0);
		OLED_ShowNumber_Fast(86,45,0,1,12,0);
		Slide_point(53,0,70,6,180,-yaw+90);
    OLED_Refresh(0);		//刷新	 
}
void Gyro_Interface(void)
{
		memset(Grame, 0, 8*128);//清空数组里的元素
//    rocher_position(0,0,50,(roll+90)*48/180,(pitch-90)*48/180);
		OLED_Show_CH_Fast(0,0,64,16,0);//角
		OLED_Show_CH_Fast(16,0,62,16,0);//速
		OLED_Show_CH_Fast(32,0,63,16,0);//度		
		OLED_ShowString_Fast(48,0,"X:",16,0,3);
	  OLED_ShowNumber_Fast(80,0,gyrox,4,16,0);
		
		OLED_Show_CH_Fast(0,16,64,16,0);//角
		OLED_Show_CH_Fast(16,16,62,16,0);//速
		OLED_Show_CH_Fast(32,16,63,16,0);//度		
		OLED_ShowString_Fast(48,16,"Y:",16,0,3);
	  OLED_ShowNumber_Fast(80,16,gyroy,4,16,0);
		
		OLED_Show_CH_Fast(0,32,64,16,0);//角
		OLED_Show_CH_Fast(16,32,62,16,0);//速
		OLED_Show_CH_Fast(32,32,63,16,0);//度		
		OLED_ShowString_Fast(48,32,"Z:",16,0,3);
	  OLED_ShowNumber_Fast(80,32,gyroz,4,16,0);
		
    OLED_Refresh(0);		//刷新		
}
void Acc_Interface(void)
{
		memset(Grame, 0, 8*128);//清空数组里的元素
		
		OLED_Show_CH_Fast(0,0,61,16,0);//加
		OLED_Show_CH_Fast(16,0,62,16,0);//速
		OLED_Show_CH_Fast(32,0,63,16,0);//度		
		OLED_ShowString_Fast(48,0,"X:",16,0,3);
	  OLED_ShowNumber_Fast(80,0,aacx,5,16,0);

		OLED_Show_CH_Fast(0,16,61,16,0);//加
		OLED_Show_CH_Fast(16,16,62,16,0);//速
		OLED_Show_CH_Fast(32,16,63,16,0);//度		
		OLED_ShowString_Fast(48,16,"Y:",16,0,3);
	  OLED_ShowNumber_Fast(80,16,aacy,5,16,0);
	
		OLED_Show_CH_Fast(0,32,61,16,0);//加
		OLED_Show_CH_Fast(16,32,62,16,0);//速
		OLED_Show_CH_Fast(32,32,63,16,0);//度		
		OLED_ShowString_Fast(48,32,"Z:",16,0,3);
	  OLED_ShowNumber_Fast(80,32,aacz,5,16,0);
	
    OLED_Refresh(0);		//刷新		
}

void RES_Interface(void)
{
		memset(Grame, 0, 8*128);//清空数组里的元素
//    rocher_position(0,0,50,(roll+90)*48/180,(pitch-90)*48/180);
		OLED_Show_CH_Fast(0,0,53,16,0);//阻
		OLED_Show_CH_Fast(16,0,54,16,0);//值
		OLED_Show_CH_Fast(84,0,60,16,0);//Ω

		OLED_ShowString_Fast(32,0,":",16,0,3);
	  OLED_ShowNumber_Fast(36,0,RES_value,6,16,0);
		
		OLED_Show_CH_Fast(0,16,55,16,0);//力
		OLED_Show_CH_Fast(16,16,56,16,0);//度
		OLED_Show_CH_Fast(32,16,57,16,0);//百
		OLED_Show_CH_Fast(48,16,58,16,0);//分
		OLED_Show_CH_Fast(64,16,59,16,0);//比
		
		OLED_ShowString_Fast(80,16,":   %",16,0,3);
	  OLED_ShowNumber_Fast(88,16,Percentage_tx,3,16,0);
		
		
//		OLED_ShowString_Fast(53,24,"G_Z:",12,0,3);
//	  OLED_ShowNumber_Fast(93,24,gyroz,4,12,0);
//		OLED_ShowNumber_Fast(86,45,0,1,12,0);
//		Slide_point(53,0,70,6,180,-yaw+90);
    OLED_Refresh(0);		//刷新
	
	
}
