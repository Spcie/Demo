#ifndef __MPU6050_H
#define __MPU6050_H

/****************************************�궨��*************************************************/
#define	SMPLRT_DIV		0x19	//�����ǲ�����,����ֵ:0x07(125Hz)
#define	CONFIG			  0x1A	//��ͨ�˲�Ƶ��,����ֵ:0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰����,����ֵ:0x18�����Լ�,2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ٶȼ��Լ졢����,����ͨ�˲���Ƶ�ʡ�����ֵ:0x01(���Լ�,2G,5Hz)
////////////////////////////////////
#define	ACCEL_XOUT_H	0x3B    //���ٶ�
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D     
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40  
///////////////////////////////////
#define	TEMP_OUT_H		0x41    //�¶�
#define	TEMP_OUT_L		0x42  
//////////////////////////////////
#define	GYRO_XOUT_H		0x43    //���ٶ�
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45        
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48  
//////////////////////////////////
#define	PWR_MGMT_1		0x6B	//��Դ����,����ֵ:0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68,ֻ��)  �����ڼ���Ƿ���������
#define	SlaveAddress	0xD0	//IIC�����ϵ�MPU6050������ַ,+1Ϊ��ȡ

/**************************************ȫ�ֱ�������*********************************************/
typedef struct MPU6050Parameter{
	float AccelerationX;
	float AccelerationY;
	float AccelerationZ;
	float PalstanceX;
	float PalstanceY;
	float PalstanceZ;
	float Temperature;          //�¶�ת����ʽ Temperature = 35+ ((double) (Temperature + 13200)) / 280;
}MPU6050Parameter;

/***************************************��������************************************************/
int MPU6050_Init(void);
int GetData(char REG_Address);
char MPU6050_Check(void);
MPU6050Parameter ReadMPUParameter(void);
#endif


/*******************************************����*************************************************/







