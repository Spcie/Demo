#ifndef __I2C_H
#define __I2C_H
/****************************************�궨��*************************************************/
#define SCL        PAout(3)
#define SDA        PAout(4)
#define READ_SDA   PAin(4)
/***************************************��������************************************************/
int IIC_Write(unsigned char REG_Address,unsigned char REG_data);
char IIC_Read(unsigned char REG_Address);
#endif
/****************************************����**************************************************/
