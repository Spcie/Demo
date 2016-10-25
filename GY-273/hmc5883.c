#include "sys.h"
#include "delay.h"
#include "hmc5883.h"
/************************************************************************************************
GY-273 HMC5883 电子罗盘驱动
占用资源 PA3 PA4 软件模拟IIC通信
连接：PA3-SCL PA4-SDA DRDY悬空
By Spice
2014.8.31
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
函数名：IIC_RecvAck
描述：等待一个ACK
输入：无
输出：
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
	IIC_SendAck(1);                           //发送NOACK   
	IIC_Stop();                               //停止信号
	return REG_data;

}						      

/************************************************************************************************
函数名：HNC5883_Init
描述：GY-273初始化
输入：无
输出：0
************************************************************************************************/
int HNC5883_Init(void)
{
	IIC_Init();
	IIC_Write(0x02,0x00);
	return 0;
}
/************************************************************************************************
函数名：HMC5883_Read
描述：一次读出寄存器数据
输入：REG_Address 寄存器地址
输出：REG_data 数据
************************************************************************************************/
HMC5883Parameter HMC5883_ReadAll(void)
{   
	HMC5883Parameter Parameter;
	unsigned char REG_data[6];     	
  int i;
	IIC_Start();                              //起始信号
	IIC_SendByte(SlaveAddress);               //HMC5883的器件地址  写信号
	IIC_SendByte(0X03);                       //数据寄存器首地址
	IIC_Start();                              //起始信号
	IIC_SendByte(SlaveAddress+1);             //HMC5883的器件地址 读取+1   读信号
	for(i=0;i<6;i++)                          //连续读出所有数据
	{
		REG_data[i] = IIC_RecvByte();
		if(i == 5) {IIC_SendAck(1);}            //最后一个地址发送一个NOACK
		else       {IIC_SendAck(0);}            //发送一个ACK
	}
	IIC_Stop();                               //停止信号
	Parameter.X = REG_data[0]<<8|REG_data[1];
	Parameter.Y = REG_data[2]<<8|REG_data[3];
	Parameter.Z = REG_data[4]<<8|REG_data[5];
	return Parameter;
}						      
/************************************************************************************************
函数名：HMC5883GetData
描述：读取寄存器的数据
输入：REG_Address 寄存器高为地址  低位地址为REG_Address+1
输出：读到的数据
************************************************************************************************/
int HMC5883GetData(char REG_Address)
{
	char H,L;
  H=IIC_Read(REG_Address);
	L=IIC_Read(REG_Address+1);
	return (H<<8)|L;  
}
/****************************************结束**************************************************/




