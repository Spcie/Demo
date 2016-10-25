#ifndef __MPU6050_H
#define __MPU6050_H

/****************************************宏定义*************************************************/
#define	SMPLRT_DIV		0x19	//陀螺仪采样率,典型值:0x07(125Hz)
#define	CONFIG			  0x1A	//低通滤波频率,典型值:0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及量程,典型值:0x18（不自检,2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速度计自检、量程,及高通滤波器频率。典型值:0x01(不自检,2G,5Hz)
////////////////////////////////////
#define	ACCEL_XOUT_H	0x3B    //加速度
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D     
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40  
///////////////////////////////////
#define	TEMP_OUT_H		0x41    //温度
#define	TEMP_OUT_L		0x42  
//////////////////////////////////
#define	GYRO_XOUT_H		0x43    //角速度
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45        
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48  
//////////////////////////////////
#define	PWR_MGMT_1		0x6B	//电源管理,典型值:0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68,只读)  可用于检测是否正常工作
#define	SlaveAddress	0xD0	//IIC总线上的MPU6050器件地址,+1为读取

/**************************************全局变量定义*********************************************/
typedef struct MPU6050Parameter{
	float AccelerationX;
	float AccelerationY;
	float AccelerationZ;
	float PalstanceX;
	float PalstanceY;
	float PalstanceZ;
	float Temperature;          //温度转化公式 Temperature = 35+ ((double) (Temperature + 13200)) / 280;
}MPU6050Parameter;

/***************************************函数声明************************************************/
int MPU6050_Init(void);
int GetData(char REG_Address);
char MPU6050_Check(void);
MPU6050Parameter ReadMPUParameter(void);
#endif


/*******************************************结束*************************************************/







