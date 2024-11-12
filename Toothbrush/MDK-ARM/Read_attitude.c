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

//void Read_attitude(float Pitch,float Roll,float Yaw,short Gyrox,short Gyroy,short Gyroz)//AACX����GYRoy
void Read_attitude(void)//AACX����GYRoy
	
{

	mpu_dmp_get_data(&pitch,&roll,&yaw);//�Ƕ�
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//�Ǽ��ٶ�
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	
//	Pitch = -pitch; //Pitch����������б(��)����С��������(��)  --��ֵ˵������������б״̬����֮������
//	Roll  = -roll ; //Roll����������б����С��������б         --��ֵ˵����������б״̬����֮������б
//	Yaw   = -yaw  ; //Yaw����������ת����С��������ת          --��ֵ˵��������ת״̬����֮����ת״̬
//  //Gyrox��Gyroy��GyrozֵԽ�� �����ٶ�Խ�󣬱仯Խ��
//	Gyrox = gyrox ; //����0--��������С��0--���¸� ����ʼ״ֵ̬Ϊ-48����
//	Gyroy = gyroy ; //����0--������б��С��0--������б����ʼֵΪ-13����
//	Gyroz = gyroz ; //����0--����ת����С��0--����ת������ʼֵΪ0����
	
	
}
