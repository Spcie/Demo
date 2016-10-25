#ifndef __HMC5883_H
#define __HMC5883_H
/****************************************宏定义*************************************************/
#define	SlaveAddress    0x3C	  //HMC5883在IIC总线中的地址
#define ANGLE_X_H       0x03   //HMC5883内部角度数据首地址  地址范围0x03~0x08
#define ANGLE_X_L       0X04
#define ANGLE_Y_H       0X05
#define ANGLE_Y_L       0X06
#define ANGLE_Z_H       0X07
#define ANGLE_Z_L       0X08
/**************************************全局变量定义*********************************************/
typedef struct HMC5883Parameter
{
	int X;
	int Y;
	int Z;
}HMC5883Parameter;
/***************************************函数声明************************************************/
int HNC5883_Init(void);
HMC5883Parameter HMC5883_ReadAll(void);         
int HMC5883GetData(char REG_Address);
/****************************************结束**************************************************/
#endif


