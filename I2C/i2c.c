#include "sys.h"
#include "delay.h"
/************************************************************************************************
IO口模拟IIC通信协议驱动
占用资源 PA3 PA4 软件模拟IIC通信
By Spice
2014.8.31
************************************************************************************************/
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
函数名：IIC_SendAck
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
	IIC_SendAck(1);                           //发送 NOACK  若不发送NOACK而发送ACK 则会继续读下个寄存器 
	IIC_Stop();                               //停止信号
	return REG_data;

}						      
/****************************************结束**************************************************/


