#include "sys.h"
#include "delay.h"
#include "mpu6050.h"
/************************************************************************************************
GY-521陀螺仪模块驱动
占用资源：PA3，PA4 用作模拟IIC通信
接线方式：PA3-SCL  PA4-SDA
注意：ADO接地或悬空
IIC总线最快速度为400KHz STM32的硬件IIC常出现BUG，因此常用IO口模拟IIC
适用单片机 STM32
By Spice
2014.8.30
************************************************************************************************/
/****************************************宏定义*************************************************/
#define SCL        PAout(3)
#define SDA        PAout(4)
#define READ_SDA   PAin(4)
/***************************************函数声明************************************************/
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
函数名：IIC_Init
描述：用于IIC的IO口配置
输入：无
输出：0
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
函数名：IIC_Start
描述：产生一个IIC开始的信号
输入：无
输出：无
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
函数名：IIC_Stop
描述：产生一个停止信号
输入：无
输出：无
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
函数名：IIC_Ack
描述：产生一个ACK
输入：ack (0:ACK 1:NAK)
输出：无
************************************************************************************************/
void IIC_SendAck(char ack)
{	
	SDA = ack;
	SCL = 1;
	delay_us(5);
	SCL = 0;
}   
/************************************************************************************************
函数名：IIC_WaitAck
描述：等待一个ACK
输入：无
输出：1有ACK,0无ACK
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
函数名：IIC_SendByte
描述：向IIC总线发送一个字节
输入：dat 发送的字节
输出：无
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
	 IIC_RecvAck();                      //等待应答 
}

/************************************************************************************************
函数名：IIC_RecvByte
描述：从IIC总线读取一个字节
输入：无
输出dat  读到的字节
************************************************************************************************/
char IIC_RecvByte(void)
{
	unsigned char i,dat=0;
	SDA = 1;                        //拉高数据线准备读取数据
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
函数名：IIC_Write
描述：写器件寄存器
输入：REG_Address 寄存器地址 REG_data 数据
输出：0
************************************************************************************************/
int IIC_Write(unsigned char REG_Address,unsigned char REG_data)		     
{
  IIC_Start();                                //起始信号
	IIC_SendByte(SlaveAddress);                 //mpu6050的器件地址
	IIC_SendByte(REG_Address);                  //寄存器地址
	IIC_SendByte(REG_data);                     //数据
	IIC_Stop();                                 //停止信号
	return 0;
}
/************************************************************************************************
函数名：IIC_Read
描述：读器件寄存器
输入：REG_Address 寄存器地址
输出：REG_data 数据
************************************************************************************************/
 char IIC_Read(unsigned char REG_Address)
{   
	unsigned char REG_data;     	
  IIC_Start();                              //起始信号
	IIC_SendByte(SlaveAddress);               //mpu的器件地址  写信号
	IIC_SendByte(REG_Address);                //寄存器地址
	IIC_Start();                              //起始信号
	IIC_SendByte(SlaveAddress+1);             //mpu的器件地址 读取+1   读信号
	REG_data = IIC_RecvByte();                //读出寄存器数据
	IIC_SendAck(1);                           //发送ACK   
	IIC_Stop();                               //停止信号
	return REG_data;

}						      

/************************************************************************************************
函数名：MPU6050_Init
描述：GY-521初始化
输入：无
输出：0
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
函数名：GetData
描述：读取一个数据
输入：REG_Address 寄存器高为地址  低位地址为REG_Address+1
输出：读到的数据
************************************************************************************************/
int GetData(char REG_Address)
{
	char H,L;
  H=IIC_Read(REG_Address);
	L=IIC_Read(REG_Address+1);
	return (H<<8)|L;  
}
/************************************************************************************************
函数名：MPU6050_Check
描述： 对WHO AM I 寄存器的检测 检测MPU6050是否正常
输入：无
输出：1:检测失败 0：检测成功
************************************************************************************************/
char MPU6050_Check(void)
{
	char temp;
	//IIC_Read(WHO_AM_I);
	IIC_Start();
	IIC_SendByte(SlaveAddress);               //mpu6050的器件地址
	IIC_SendByte(WHO_AM_I);
	IIC_Start();
	IIC_SendByte(SlaveAddress+1);             //mpu6050的器件地址 读取+1
	temp = IIC_RecvByte();//发送ACK   
	IIC_Stop();
	if(temp == 0x68)  {return 0;}
	return 1;
}
/************************************************************************************************
函数名：ReadMPUParameter
描述：读取GY-521的数据
输入：无
输出；GY-521数据
************************************************************************************************/
MPU6050Parameter ReadMPUParameter(void)
{
	MPU6050Parameter Parameter;
	Parameter.AccelerationX = GetData(ACCEL_XOUT_H);          //X轴加速度
	Parameter.AccelerationY = GetData(ACCEL_YOUT_H);          //Y轴加速度
	Parameter.AccelerationZ = GetData(ACCEL_ZOUT_H);          //Z轴加速度
	Parameter.PalstanceX = GetData(GYRO_XOUT_H);              //X轴角速度
	Parameter.PalstanceY = GetData(GYRO_YOUT_H);              //Y轴角速度
	Parameter.PalstanceZ = GetData(GYRO_ZOUT_H);              //Z轴角速度
	Parameter.Temperature =  GetData(TEMP_OUT_H);             //温度
	return Parameter;
}

/*******************************************结束*************************************************/



