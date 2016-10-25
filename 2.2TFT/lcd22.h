#ifndef __LCD22_H
#define __LCD22_H
#include "sys.h"

extern u16 PointColor;	//������ɫ
extern u16 BackColor;  //����ɫ 

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

int LCD22_Init(void);
void LCD22_Clear(unsigned Color);
void LCD22_DrawPoint(unsigned x,unsigned y,u16 Color);
void LCD22_Fill(unsigned xsta,unsigned ysta,unsigned xend,unsigned yend,unsigned color);
void LCD22_DrawBigPoint(unsigned x,unsigned y,unsigned int POINT_COLOR);
void LCD22_DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2,unsigned int POINT_COLOR);
void LCD22_ClearLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2,unsigned int POINT_COLOR);
void LCD22_DrawRectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2,unsigned int POINT_COLOR);
void LCD22_DrawCircle(unsigned x0,unsigned y0,unsigned char r,unsigned int POINT_COLOR);
void LCD22_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD22_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void number(unsigned int x,unsigned int y,const unsigned char number,unsigned int POINT_COLOR,unsigned int BACK_COLOR);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
#endif




