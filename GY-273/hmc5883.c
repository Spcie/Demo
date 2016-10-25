#include "sys.h"
#include "delay.h"
#include "hmc5883.h"
/************************************************************************************************
GY-273 HMC5883 ������������
ռ����Դ PA3 PA4 ���ģ��IICͨ��
���ӣ�PA3-SCL PA4-SDA DRDY����
By Spice
2014.8.31
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
��������IIC_SendAck
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
��������IIC_RecvAck
�������ȴ�һ��ACK
���룺��
�����
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
	IIC_SendAck(1);                           //����NOACK   
	IIC_Stop();                               //ֹͣ�ź�
	return REG_data;

}						      

/************************************************************************************************
��������HNC5883_Init
������GY-273��ʼ��
���룺��
�����0
************************************************************************************************/
int HNC5883_Init(void)
{
	IIC_Init();
	IIC_Write(0x02,0x00);
	return 0;
}
/************************************************************************************************
��������HMC5883_Read
������һ�ζ����Ĵ�������
���룺REG_Address �Ĵ�����ַ
�����REG_data ����
************************************************************************************************/
HMC5883Parameter HMC5883_ReadAll(void)
{   
	HMC5883Parameter Parameter;
	unsigned char REG_data[6];     	
  int i;
	IIC_Start();                              //��ʼ�ź�
	IIC_SendByte(SlaveAddress);               //HMC5883��������ַ  д�ź�
	IIC_SendByte(0X03);                       //���ݼĴ����׵�ַ
	IIC_Start();                              //��ʼ�ź�
	IIC_SendByte(SlaveAddress+1);             //HMC5883��������ַ ��ȡ+1   ���ź�
	for(i=0;i<6;i++)                          //����������������
	{
		REG_data[i] = IIC_RecvByte();
		if(i == 5) {IIC_SendAck(1);}            //���һ����ַ����һ��NOACK
		else       {IIC_SendAck(0);}            //����һ��ACK
	}
	IIC_Stop();                               //ֹͣ�ź�
	Parameter.X = REG_data[0]<<8|REG_data[1];
	Parameter.Y = REG_data[2]<<8|REG_data[3];
	Parameter.Z = REG_data[4]<<8|REG_data[5];
	return Parameter;
}						      
/************************************************************************************************
��������HMC5883GetData
��������ȡ�Ĵ���������
���룺REG_Address �Ĵ�����Ϊ��ַ  ��λ��ַΪREG_Address+1
���������������
************************************************************************************************/
int HMC5883GetData(char REG_Address)
{
	char H,L;
  H=IIC_Read(REG_Address);
	L=IIC_Read(REG_Address+1);
	return (H<<8)|L;  
}
/****************************************����**************************************************/




