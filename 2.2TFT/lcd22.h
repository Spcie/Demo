#ifndef __LCD22_H
#define __LCD22_H
#include "sys.h"

extern u16 PointColor;	//画笔颜色
extern u16 BackColor;  //背景色 

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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

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




