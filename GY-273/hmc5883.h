#ifndef __HMC5883_H
#define __HMC5883_H
/****************************************�궨��*************************************************/
#define	SlaveAddress    0x3C	  //HMC5883��IIC�����еĵ�ַ
#define ANGLE_X_H       0x03   //HMC5883�ڲ��Ƕ������׵�ַ  ��ַ��Χ0x03~0x08
#define ANGLE_X_L       0X04
#define ANGLE_Y_H       0X05
#define ANGLE_Y_L       0X06
#define ANGLE_Z_H       0X07
#define ANGLE_Z_L       0X08
/**************************************ȫ�ֱ�������*********************************************/
typedef struct HMC5883Parameter
{
	int X;
	int Y;
	int Z;
}HMC5883Parameter;
/***************************************��������************************************************/
int HNC5883_Init(void);
HMC5883Parameter HMC5883_ReadAll(void);         
int HMC5883GetData(char REG_Address);
/****************************************����**************************************************/
#endif


