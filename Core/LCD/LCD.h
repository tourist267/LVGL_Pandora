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

//LCDƬѡλ
#define LCD_CS_L				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET)
#define LCD_CS_H				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET)
//LCD�����������λ
#define LCD_DC_CMD			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)			
#define LCD_DC_DATA			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
///LCD��λ��
#define LCD_RES_L			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)			
#define LCD_RES_H			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
//LCD�������λ �ߴ�,�͹ر�
#define LCD_PWR_OFF			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)			
#define LCD_PWR_ON			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)


//LCD�Ŀ�͸߶���
#define LCD_Width 	240
#define LCD_Height 	240

//LCD�����С���ã��޸Ĵ�ֵʱ��ע�⣡�������޸�������ֵʱ���ܻ�Ӱ�����º���	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152


//������ɫ
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








void LCD_Init(void);
void LCD_Show(void);
void LCD_Clear(u16 color);
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p);
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);
#endif
