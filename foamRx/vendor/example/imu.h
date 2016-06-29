#ifndef __IMU_H
#define __IMU_H

//#include "common.h"

//#include <math.h>
#define M_PI  (float)3.1415926535.f

//��������̬��������ⲿ�������

extern float pitch, roll,yaw;
extern int gps_pitch,gps_roll;

extern int IMU_Pitch,IMU_Roll,IMU_Yaw;	  //��λ ��

extern  int IMU_Pitch_, IMU_Roll_, IMU_Yaw_;
//�����ǵ������
extern volatile float IMU_GYROx, IMU_GYROy, IMU_GYROz;	//��λ  ��ÿ��
extern int ACCerr;

extern float imuKp,imuKi;
//Mini IMU AHRS �����API
void IMU_init(void); //��ʼ��
void IMU_getYawPitchRoll(void); //������̬  ��Ҫ���ڵ���

void IMU_noHeadMode(int x,int y);
//uint32_t micros(void);	//��ȡϵͳ�ϵ���ʱ��  ��λ us

#endif

//------------------End of File----------------------------
