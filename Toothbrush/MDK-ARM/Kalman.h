#ifndef _KALMAN_H
#define _KALMAN_H

void Kalman_Cal_Roll(float acc,float gyro);
float Kalman_Filter_x(float Accel,float Gyro)		;
float Complementary_Filter_x(float angle_m, float gyro_m);
float Complementary_Filter_y(float angle_m, float gyro_m);

#endif
