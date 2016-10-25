#ifndef __NOKIA5110_H
#define __NOKIA5110_H
/**********************************头文件声明*********************************************/
#include "sys.h"
#include "delay.h"

/************************************宏定义***********************************************/
#define Nokia5110_CE_1  GPIOG->BSRR = 1<<4           //取消片选
#define Nokia5110_CE_0  GPIOG->BRR = 1<<4            //片选有效
#define Nokia5110_RET_1 GPIOG->BSRR = 1<<6
#define Nokia5110_RET_0 GPIOG->BRR = 1<<6
#define Nokia5110_DC_1  GPIOG->BSRR = 1<<2
#define Nokia5110_DC_0  GPIOG->BRR = 1<<2
#define Nokia5110_CK_1  GPIOG->BSRR = 1<<7
#define Nokia5110_CK_0  GPIOG->BRR = 1<<7
#define Nokia5110_DIN_1 GPIOG->BSRR = 1<<8
#define Nokia5110_DIN_0 GPIOG->BRR = 1<<8

#define BIT(x) 0x01<<x
/***********************************函数声明**********************************************/


int Nokia_Init(void);
int Nokia_write_byte(unsigned char dat,unsigned char mode);
int Nokia_clear(void);
int Nokia_SetCoordiate(unsigned char X,unsigned char Y);
int Nokia_Write_Char(unsigned char c);
int Nokia_WriteEnglishString(unsigned char X,unsigned char Y,char *s);
int Nokia_DrawPiont(unsigned char X,unsigned Y);
//void Nokia_Line(unsigned char StartX, unsigned char StartY, unsigned char EndX, unsigned char EndY);
#endif
