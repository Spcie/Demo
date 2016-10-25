#include "sys.h"
#include "delay.h"
#include "mpu6050.h"
/************************************************************************************************
GY-521������ģ������
ռ����Դ��PA3��PA4 ����ģ��IICͨ��
���߷�ʽ��PA3-SCL  PA4-SDA
ע�⣺ADO�ӵػ�����
IIC��������ٶ�Ϊ400KHz STM32��Ӳ��IIC������BUG����˳���IO��ģ��IIC
���õ�Ƭ�� STM32
By Spice
2014.8.30
************************************************************************************************/
/****************************************�궨��*************************************************/
#define SCL        PAout(3)
#define SDA        PAout(4)
#define READ_SDA   PAin(4)
/***************************************��������************************************************/
int IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendAck(char ack);
void IIC_Ack(void);
void IIC_NoAck(void);
char IIC_RecvACK(void);
void IIC_SendByte(char dat);
char IIC_RecvByte(void);
int IIC_Write(unsigned char REG_Address,unsigned char REG_data);
char IIC_Read(unsigned char REG_Address);

MPU6050Parameter  Parameter6050;
/************************************************************************************************
��������IIC_Init
����������IIC��IO������
���룺��
�����0
************************************************************************************************/
int IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	return 0;
}
/************************************************************************************************
��������IIC_Start
����������һ��IIC��ʼ���ź�
���룺��
�������
************************************************************************************************/
void IIC_Start(void)
{
    SDA = 1;                  
    SCL = 1;                    
    delay_us(5);                
    SDA = 0;                    
    delay_us(5);                
    SCL = 0;                    
}
/************************************************************************************************
��������IIC_Stop
����������һ��ֹͣ�ź�
���룺��
�������
************************************************************************************************/
void IIC_Stop(void)
{
	  SDA = 0;  
	  SCL = 1;                                     
    delay_us(5);	
    SDA = 0;    	
    delay_us(5);  
      	
}
/************************************************************************************************
��������IIC_Ack
����������һ��ACK
���룺ack (0:ACK 1:NAK)
�������
************************************************************************************************/
void IIC_SendAck(char ack)
{	
	SDA = ack;
	SCL = 1;
	delay_us(5);
	SCL = 0;
}   
/************************************************************************************************
��������IIC_WaitAck
�������ȴ�һ��ACK
���룺��
�����1��ACK,0��ACK
************************************************************************************************/
char IIC_RecvAck(void) 	 
{
	u8 Note;
	SCL = 1;
	delay_us(5);  
	Note = READ_SDA;
	SCL = 0;
	delay_us(5);	 	   
	return Note;  
}

/************************************************************************************************
��������IIC_SendByte
��������IIC���߷���һ���ֽ�
���룺dat ���͵��ֽ�
�������
************************************************************************************************/
void IIC_SendByte(char dat)
{
    u8 i;    	    
    for(i=0;i<8;i++)
    {     
			SDA=((dat&0x80)>>7);
			dat<<=1; 	   
			SCL = 1;
			delay_us(5); 
			SCL = 0;
			delay_us(5);  
    }	 
	 IIC_RecvAck();                      //�ȴ�Ӧ�� 
}

/************************************************************************************************
��������IIC_RecvByte
��������IIC���߶�ȡһ���ֽ�
���룺��
����rdat  �������ֽ�
************************************************************************************************/
char IIC_RecvByte(void)
{
	unsigned char i,dat=0;
	SDA = 1;                        //����������׼����ȡ����
  for(i=0;i<8;i++ )
	{
		dat<<=1;
    SCL = 1; 
    delay_us(5);
		dat |= READ_SDA;
		SCL = 0;
    delay_us(5);

  }					 
    return dat;
}
/************************************************************************************************
��������IIC_Write
������д�����Ĵ���
���룺REG_Address �Ĵ�����ַ REG_data ����
�����0
************************************************************************************************/
int IIC_Write(unsigned char REG_Address,unsigned char REG_data)		     
{
  IIC_Start();                                //��ʼ�ź�
	IIC_SendByte(SlaveAddress);                 //mpu6050��������ַ
	IIC_SendByte(REG_Address);                  //�Ĵ�����ַ
	IIC_SendByte(REG_data);                     //����
	IIC_Stop();                                 //ֹͣ�ź�
	return 0;
}
/************************************************************************************************
��������IIC_Read
�������������Ĵ���
���룺REG_Address �Ĵ�����ַ
�����REG_data ����
************************************************************************************************/
 char IIC_Read(unsigned char REG_Address)
{   
	unsigned char REG_data;     	
  IIC_Start();                              //��ʼ�ź�
	IIC_SendByte(SlaveAddress);               //mpu��������ַ  д�ź�
	IIC_SendByte(REG_Address);                //�Ĵ�����ַ
	IIC_Start();                              //��ʼ�ź�
	IIC_SendByte(SlaveAddress+1);             //mpu��������ַ ��ȡ+1   ���ź�
	REG_data = IIC_RecvByte();                //�����Ĵ�������
	IIC_SendAck(1);                           //����ACK   
	IIC_Stop();                               //ֹͣ�ź�
	return REG_data;

}						      

/************************************************************************************************
��������MPU6050_Init
������GY-521��ʼ��
���룺��
�����0
************************************************************************************************/
int MPU6050_Init(void)
{
	IIC_Init();
	
	IIC_Write(PWR_MGMT_1, 0x00);	//??????
	IIC_Write(SMPLRT_DIV, 0x07);
	IIC_Write(CONFIG, 0x06);
	IIC_Write(GYRO_CONFIG, 0x18);
	IIC_Write(ACCEL_CONFIG, 0x01);
	return 0;
}
/************************************************************************************************
��������GetData
��������ȡһ������
���룺REG_Address �Ĵ�����Ϊ��ַ  ��λ��ַΪREG_Address+1
���������������
************************************************************************************************/
int GetData(char REG_Address)
{
	char H,L;
  H=IIC_Read(REG_Address);
	L=IIC_Read(REG_Address+1);
	return (H<<8)|L;  
}
/************************************************************************************************
��������MPU6050_Check
������ ��WHO AM I �Ĵ����ļ�� ���MPU6050�Ƿ�����
���룺��
�����1:���ʧ�� 0�����ɹ�
************************************************************************************************/
char MPU6050_Check(void)
{
	char temp;
	//IIC_Read(WHO_AM_I);
	IIC_Start();
	IIC_SendByte(SlaveAddress);               //mpu6050��������ַ
	IIC_SendByte(WHO_AM_I);
	IIC_Start();
	IIC_SendByte(SlaveAddress+1);             //mpu6050��������ַ ��ȡ+1
	temp = IIC_RecvByte();//����ACK   
	IIC_Stop();
	if(temp == 0x68)  {return 0;}
	return 1;
}
/************************************************************************************************
��������ReadMPUParameter
��������ȡGY-521������
���룺��
�����GY-521����
************************************************************************************************/
MPU6050Parameter ReadMPUParameter(void)
{
	MPU6050Parameter Parameter;
	Parameter.AccelerationX = GetData(ACCEL_XOUT_H);          //X����ٶ�
	Parameter.AccelerationY = GetData(ACCEL_YOUT_H);          //Y����ٶ�
	Parameter.AccelerationZ = GetData(ACCEL_ZOUT_H);          //Z����ٶ�
	Parameter.PalstanceX = GetData(GYRO_XOUT_H);              //X����ٶ�
	Parameter.PalstanceY = GetData(GYRO_YOUT_H);              //Y����ٶ�
	Parameter.PalstanceZ = GetData(GYRO_ZOUT_H);              //Z����ٶ�
	Parameter.Temperature =  GetData(TEMP_OUT_H);             //�¶�
	return Parameter;
}

/*******************************************����*************************************************/



