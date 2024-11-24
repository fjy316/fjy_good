#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "gpio.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include "Read_attitude.h"

extern  float pitch,roll,yaw; 		    //y-?
extern  short aacx,aacy,aacz;			//��LSBΪ��λ
extern  short gyrox,gyroy,gyroz;		//G???'??

float real_aacx,real_aacy,real_aacz;//��λΪ�������ٶ�g
float real_gyrox,real_gyroy,real_gyroz;//��λΪ ��/s


//void Read_attitude(float Pitch,float Roll,float Yaw,short Gyrox,short Gyroy,short Gyroz)//AACX����GYRoy
void Read_attitude(void)//AACX����GYRoy
	
{

	mpu_dmp_get_data(&pitch,&roll,&yaw);//�Ƕ�
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//�Ǽ��ٶ�
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	//ʵ�ʼ��ٶ�a=ACC��(2g-(-2g)/2^14; ��2gΪmpu6050���õ����̷�Χ�����ڵ�LSB������Ϊ2��14�η���16384����
	//�������ٶ� g=9.8��/��^2

	real_aacx = aacx*(2-(-2))/16384;
	real_aacy = aacy*(2-(-2))/16384;
	real_aacz = aacz*(2-(-2))/16384;
	//ʵ�ʽ��ٶ�Anglerate = gyro/������ ����2000dpsΪmpu6050���õ����̷�Χ����Ӧ������Ϊ16.4LSB/��/s.
	real_gyrox = gyrox/16.4;
	real_gyroy = gyroy/16.4;
	real_gyroz = gyroz/16.4;
	
//	Pitch = -pitch; //Pitch����������б(��)����С��������(��)  --��ֵ˵������������б״̬����֮������
//	Roll  = -roll ; //Roll����������б����С��������б         --��ֵ˵����������б״̬����֮������б
//	Yaw   = -yaw  ; //Yaw����������ת����С��������ת          --��ֵ˵��������ת״̬����֮����ת״̬
//  //Gyrox��Gyroy��GyrozֵԽ�� �����ٶ�Խ�󣬱仯Խ��
//	Gyrox = gyrox ; //����0--��������С��0--���¸� ����ʼ״ֵ̬Ϊ-48����
//	Gyroy = gyroy ; //����0--������б��С��0--������б����ʼֵΪ-13����
//	Gyroz = gyroz ; //����0--����ת����С��0--����ת������ʼֵΪ0����
	
	
}
