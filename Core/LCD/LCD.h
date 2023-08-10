#ifndef __LCD_H
#define __LCD_H

//#include "stm32l4xx_hal.h"
#include "sys.h"



/*
	LCD_PWR:	PB7
	LCD_RST:	PB6
	LCD_DC:		PB4
	LCD_CS:		PD7
*/

//LCD片选位
#define LCD_CS_L				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET)
#define LCD_CS_H				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET)
//LCD数据命令控制位
#define LCD_DC_CMD			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)			
#define LCD_DC_DATA			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
///LCD复位口
#define LCD_RES_L			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)			
#define LCD_RES_H			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
//LCD背光控制位 高打开,低关闭
#define LCD_PWR_OFF			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)			
#define LCD_PWR_ON			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)


//LCD的宽和高定义
#define LCD_Width 	240
#define LCD_Height 	240

//LCD缓存大小设置，修改此值时请注意！！！！修改这两个值时可能会影响以下函数	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152


//画笔颜色
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








void LCD_Init(void);
void LCD_Show(void);
void LCD_Clear(u16 color);
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p);
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);
#endif
