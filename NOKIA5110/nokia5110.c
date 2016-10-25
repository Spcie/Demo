#include "nokia5110.h"
/******************************************************************************************
NOKIA5110 ����  ʹ������ģ��SPIͨ�� 
ռ����Դ ��PG2-DC PG4-CE PG6-RET PG8-DIN PG7-CLK 
BLΪ�����Դ����ѡ��ر�
���ߺ��� Nokia_Line ֻ�ܻ���ֱ��ˮƽ����

******************************************************************************************/
/*************************************�ֿ�************************************************/
const unsigned char font6x8[92][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },  
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },  
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },  
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },  
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },  
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },  
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },  
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },  
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },  
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 },
	};   

/******************************************************************************************
��������Nokia_Init
���ܣ� Nokia5110����IO������   Nokia5110����
���룺��
�����0
******************************************************************************************/
int Nokia_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_4 | GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	Nokia5110_RET_0;
	delay_us(1);
	Nokia5110_RET_1;                                         //����һ��ʹLCD��λ�ĵ�����                            
	 
	Nokia5110_CE_0;                                          //ȡ��Ƭѡ
	delay_us(1);
	Nokia5110_CE_1;                                          //ʹ��Ƭѡ
	delay_us(1);
	Nokia_write_byte(0x21,0);                                //ʹ����չ����LCDģʽ
	Nokia_write_byte(0xc8,0);                                //����Һ��ƫ�õ�ѹ
	Nokia_write_byte(0x06,0);                                //�¶�У��
	Nokia_write_byte(0x13,0);                                //1��48
	Nokia_write_byte(0x20,0);                                //ʹ�û������� V=0 ˮƽѰַ
	Nokia_clear();                                           //����
	Nokia_write_byte(0x0c,0);                                //������ʾģʽ��������ʾ
	
	Nokia5110_CE_0;                                          //ȡ��Ƭѡ
	
	return 0;
}
/******************************************************************************************
��������nokia_write_byte
���ܣ���nokia5110дһ���ֽ�
���룺dat: ���͵��ֽ� mode��0 д����  1д����
�����0
******************************************************************************************/
int Nokia_write_byte(unsigned char dat,unsigned char mode)
{
	int i;
	Nokia5110_CE_0;                                          //Ƭѡ��Ч
	if(mode == 0) Nokia5110_DC_0;                            //д����
	if(mode == 1) Nokia5110_DC_1;                            //д����
	  for(i=0;i<8;i++)				  //����8�ֽ�
  {
    if(dat&0x80)
      Nokia5110_DIN_1;
    else
      Nokia5110_DIN_0;
		
    Nokia5110_CK_0;
    dat = dat << 1;
    Nokia5110_CK_1;                //һ��ʱ������
  }
	//SPI2_ReadWriteByte(dat);                                 //��������
	Nokia5110_CE_1;                                          //ȡ��Ƭѡ
	return 0;
}
/******************************************************************************************
��������Nokia_clear
���ܣ� Nokia5110����
���룺��
�����0
******************************************************************************************/
int Nokia_clear(void)
{
	unsigned int i;
	Nokia_write_byte(0x0C,0);
	Nokia_write_byte(0x80,0);
	for(i=0;i<504;i++)
	{
		Nokia_write_byte(0,1);
	}
	return 0;
}
/******************************************************************************************
��������Nokia_SetCoordiate
���ܣ���Ļ������GRAM��ַת��
���룺�أ������ꡡ�٣�������
�����0
******************************************************************************************/
int Nokia_SetCoordiate(unsigned char X,unsigned char Y)
{
	Nokia_write_byte(0x40|Y,0);
	Nokia_write_byte(0x80|X,0);
	return 0;
}
/******************************************************************************************
�������� Nokia_Write_Char
���� ����ʾһ���ַ�
���� ��  ��ʾ���ַ�
��� ��0
******************************************************************************************/
int Nokia_Write_Char(unsigned char c)
{
	unsigned char line;
	c-=32;
	for(line=0;line<6;line++)
	{
		Nokia_write_byte(font6x8[c][line],1);
	}
	return 0;
}
/******************************************************************************************
��������Nokia_WriteEnglishString
���� ����ָ��������ʼ����ʾһ���ַ���
���룺X �������꣬Y�������� *s ���ַ���ָ��
******************************************************************************************/
int Nokia_WriteEnglishString(unsigned char X,unsigned char Y,char *s)
{
	Nokia_SetCoordiate(X,Y);
	while(*s)
	{
		Nokia_Write_Char(*s);
		s++;
	}
	return 0;
}
/******************************************************************************************
��������Nokia_DrawPiont
���ܣ� ���㺯��
���룺������
����� ��
******************************************************************************************/
int Nokia_DrawPiont(unsigned char X,unsigned Y)
{
	Nokia_SetCoordiate(X,Y);
	Nokia_write_byte(BIT(Y % 8),1);
	return 0;
}
/******************************************************************************************
��������Nokia_Line
���ܣ����ߺ���
���룺StartX StartY  ��ʼ������         EndX EndY�յ�����

******************************************************************************************/

void Nokia_Line(unsigned char StartX, unsigned char StartY, unsigned char EndX, unsigned char EndY)
{
  unsigned char TempX,TempY;
  unsigned char TempByte=0xFF;
  if(EndY<StartY)                                                        //�任���߷������з�����Y����
  {
    TempX=StartX;
	StartX=EndX;
	EndX=TempX;
	TempY=StartY;
	StartY=EndY;
	EndY=TempY;
  }
  
  if(EndX==StartX)                                                       //�����仯��x��Ⱦ͸�Ϊ��ֱ���»���
  {
    //����Ϊǰ�˴���
    if(StartY%8)                                                         //���ܱ�8����˵���п��ֽ�
	{
	  Nokia_SetCoordiate(StartX,StartY/8);
	  //�жϳ����Ƿ�
	  if((EndY-StartY)>(7-StartY%8))                                       //˵�����ȹ���
	  {
	    TempByte>>=(StartY%8);
		TempByte<<=(StartY%8);
		Nokia_write_byte(TempByte,1);
		StartY+=(7-StartY%8);                                                //����StartY�����¸�����
	  }
	  else                                                                 //���Ȳ���һ���ֽ�
	  {
	    TempByte>>=(StartY%8);
		TempByte<<=(StartY%8+7-EndY%8);
		TempByte>>=(7-EndY%8);
		Nokia_write_byte(TempByte,1);
		return;
	  }
	}
	//����Ĳ���������ʼ���궼û�п�Խ�ֽ�(�������Ѿ�������ϣ������Ĳ�����һ���ֽ�)
	//�жδ���
	while(EndY-StartY>7)
	{
	 Nokia_SetCoordiate(StartX,StartY/8);
	  Nokia_write_byte(0xFF,1);
	  StartY+=8;                     //�������
	}
	//ĩ�˴���
	TempByte=0xFF;
	TempByte<<=(7-EndY%8);
	TempByte>>=(7-EndY%8);
	Nokia_SetCoordiate(StartX,StartY/8);
	Nokia_write_byte(TempByte,1);
  }
  else if(StartY==EndY) //������б��Ϊ0�����
  {
    if(StartX>EndX)
	{
	  TempX=StartX;
	  StartX=EndX;
	  EndX=TempX;
	}
	Nokia_SetCoordiate(StartX,StartY/8);
	while(EndX+1-StartX)
	{
	  Nokia_write_byte(BIT(StartY%8),1);
	  StartX++;
	}
  }

}



/***************************************����**********************************************/


