#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "gpio.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "Read_attitude.h"

extern  float pitch,roll,yaw; 		    //y-?
extern  short aacx,aacy,aacz;			//???????'??
extern  short gyrox,gyroy,gyroz;		//G???'??

//void Read_attitude(float Pitch,float Roll,float Yaw,short Gyrox,short Gyroy,short Gyroz)//AACX换成GYRoy
void Read_attitude(void)//AACX换成GYRoy
	
{

	mpu_dmp_get_data(&pitch,&roll,&yaw);//角度
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//角加速度
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	
//	Pitch = -pitch; //Pitch增加向下倾斜(俯)，减小则向上倾(仰)  --正值说明处于向下倾斜状态，反之则向上
//	Roll  = -roll ; //Roll增加往左倾斜，减小则往右倾斜         --正值说明处于左倾斜状态，反之则右倾斜
//	Yaw   = -yaw  ; //Yaw增加往右旋转，减小则往左旋转          --正值说明处于左转状态，反之则右转状态
//  //Gyrox，Gyroy，Gyroz值越大 ，加速度越大，变化越大
//	Gyrox = gyrox ; //大于0--向上仰，小于0--向下俯 ，初始状态值为-48左右
//	Gyroy = gyroy ; //大于0--向右倾斜，小于0--向左倾斜，初始值为-13左右
//	Gyroz = gyroz ; //大于0--向左转动，小于0--向右转动，初始值为0左右
	
	
}
