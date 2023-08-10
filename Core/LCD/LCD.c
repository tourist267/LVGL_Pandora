#include "LCD.h"
#include "delay.h"
#include "font.h"
extern SPI_HandleTypeDef hspi1;
/**
* 使用spi发送数据
 *
 * @param   data	数据
 * @param   size	数据大小
 *
 * @return  void
 */
static void SPI_Send(u8 *data ,u32 size)
{
	u32 i,data_num;
	
	//发送的数据量//
	data_num = size/0xffff;
	
	for(i = 0;i<=data_num;i++)
	{
		//短数据一次发一位,长数据一次发一个字节//
		if(i == data_num){HAL_SPI_Transmit(&hspi1,&data[i*0xffff],size%0xffff,1000);}
		else {HAL_SPI_Transmit(&hspi1,&data[i*0xffff],0xffff,1000);}
	}
	
}
/**
 * LCD发送一位命令
 *
 
 * @return  void
 */
static void LCD_WR_CMD(u8 cmd)
{
	LCD_DC_CMD;
	SPI_Send(&cmd,1);
}

/**
 * LCD发送一位数据
 *
 
 * @return  void
 */
static void LCD_WR_DATA(u8 data)
{
	LCD_DC_DATA;
	SPI_Send(&data,1);
}
/**
 * 发送半个字节的数据
 *
 * 
 * 
 *
 * @return  void
 */
static void LCD_WR_HalfWord(const u16 data)
{
	u8 dat[2];
	dat[0] = data>>8;
	dat[1] = data;
	
	LCD_DC_DATA;
	SPI_Send(dat,2);
}
/**
 * 设置数据写入LCD缓存区域
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_WR_CMD(0x2a);
	LCD_WR_DATA(x1 >> 8);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2 >> 8);
	LCD_WR_DATA(x2);

	LCD_WR_CMD(0x2b);
	LCD_WR_DATA(y1 >> 8);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2 >> 8);
	LCD_WR_DATA(y2);

	LCD_WR_CMD(0x2C);
}
static u8 lcd_buf[LCD_Buf_Size];
/**
 * 以一种颜色清空LCD屏
 *
 * @param   color	清屏颜色
 *
 * @return  void
 */
 void LCD_Clear(u16 color)
{
    u16 i, j;
    u8 data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_DC_DATA;

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        SPI_Send(lcd_buf, LCD_Buf_Size);
    }
}
/**
 * 用一个颜色填充整个区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end			终点坐标
 * @param   color       		填充颜色
 *
 * @return  void
 */
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color)
{
    u16 i = 0;
    u32 size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC_DATA;
         SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}
/**
 * 用颜色缓冲区填充区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end			终点坐标
 * @param   clr_buf       		颜色缓冲区
 *
 * @return  void
 */
void LCD_Fill_Buf(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16* clr_buf)
{
    u16 i = 0;
    u32 size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_Buf_Size)
    {
        size_remain = size - LCD_Buf_Size;
        size = LCD_Buf_Size;
    }

    LCD_Address_Set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = clr_buf[i] >> 8;
            lcd_buf[2 * i + 1] = clr_buf[i];
        }

        LCD_DC_DATA;
        SPI_Send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_Buf_Size)
        {
            size_remain = size_remain - LCD_Buf_Size;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}
u16	POINT_COLOR = BLACK;	//画笔颜色	默认为黑色
u16	BACK_COLOR 	= WHITE;	//背景颜色	默认为白色
/**
 * 画点函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_Point(u16 x, u16 y)
{
    LCD_Address_Set(x, y, x, y);
    LCD_WR_HalfWord(POINT_COLOR);
}

/**
 * 画点带颜色函数
 *
 * @param   x,y		画点坐标
 *
 * @return  void
 */
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_WR_HalfWord(color);
}

/**
 * @brief	画线函数(直线、斜线)
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    u32 i = 0;

    if(y1 == y2)
    {
        /*快速画水平线*/
        LCD_Address_Set(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_DC_DATA;
        SPI_Send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0) {incx = 1;} 
    else if(delta_x == 0){incx = 0;}
    else{incx = -1;delta_x = -delta_x;}

    if(delta_y > 0)incy = 1;

    else if(delta_y == 0)incy = 0;

    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++)
    {
        LCD_Draw_Point(row, col);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief	画一个矩形
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1,GREEN);
    LCD_DrawLine(x1, y1, x1, y2,GREEN);
    LCD_DrawLine(x1, y2, x2, y2,GREEN);
    LCD_DrawLine(x2, y1, x2, y2,GREEN);
}

/**
 * @brief	画一个圆
 *
 * @param   x0,y0	圆心坐标
 * @param   r       圆半径
 *
 * @return  void
 */
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b)
    {
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 + b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 - a);
        LCD_Draw_Point(x0 - a, y0 - b);
        LCD_Draw_Point(x0 + b, y0 + a);
        LCD_Draw_Point(x0 + a, y0 - b);
        LCD_Draw_Point(x0 + a, y0 + b);
        LCD_Draw_Point(x0 - b, y0 + a);
        a++;

        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        LCD_Draw_Point(x0 + a, y0 + b);
    }
}

/**
 * @brief	显示一个ASCII码字符
 *
 * @param   x,y		显示起始坐标
 * @param   chr		需要显示的字符
 * @param   size	字体大小(支持16/24/32号字体)
 *
 * @return  void
 */
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size)
{
    u8 temp, t1, t;
    u8 csize;		//得到字体一个字符对应点阵集所占的字节数
    u16 colortemp;
    u8 sta;

    chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

    LCD_Address_Set(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == 16) || (size == 32) )	//16和32号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)temp = asc2_1608[chr][t];	//调用1608字体
            else if(size == 32)temp = asc2_3216[chr][t];	//调用3216字体
            else return;			//没有的字库

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_WR_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

	else if  (size == 12)	//12号字体
	{
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_WR_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
	
    else if(size == 24)		//24号字体
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if(t % 2 == 0)sta = 8;
            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) colortemp = POINT_COLOR;
                else colortemp = BACK_COLOR;

                LCD_WR_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief	m^n函数
 *
 * @param   m,n		输入参数
 *
 * @return  m^n次方
 */
static u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;

    while(n--)result *= m;

    return result;
}

/**
 * @brief	显示数字,高位为0不显示
 *
 * @param   x,y		起点坐标
 * @param   num		需要显示的数字,数字范围(0~4294967295)
 * @param   len		需要显示的位数
 * @param   size	字体大小
 *
 * @return  void
 */
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size);
                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
    }
}



/**
 * @brief	显示数字,高位为0,可以控制显示为0还是不显示
 *
 * @param   x,y		起点坐标
 * @param   num		需要显示的数字,数字范围(0~999999999)
 * @param   len		需要显示的位数
 * @param   size	字体大小
 * @param   mode	1:高位显示0		0:高位不显示
 *
 * @return  void
 */
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
    u8 t, temp;
    u8 enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                if(mode)LCD_ShowChar(x + (size / 2)*t, y, '0', size);
                else
                    LCD_ShowChar(x + (size / 2)*t, y, ' ', size);

                continue;
            }

            else enshow = 1;
        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size);
    }
}


/**
 * @brief	显示字符串
 *
 * @param   x,y		起点坐标
 * @param   width	字符显示区域宽度
 * @param   height	字符显示区域高度
 * @param   size	字体大小
 * @param   p		字符串起始地址
 *
 * @return  void
 */
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, char *p)
{
    u8 x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)break; //退出

        LCD_ShowChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
}


/**
 * @brief	显示图片
 *
 * @remark	Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
 *
 * @param   x,y		起点坐标
 * @param   width	图片宽度
 * @param   height	图片高度
 * @param   p		图片缓存数据起始地址
 *
 * @return  void
 */
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p)
{
    if(x + width > LCD_Width || y + height > LCD_Height)
    {
        return;
    }

    LCD_Address_Set(x, y, x + width - 1, y + height - 1);

    LCD_DC_DATA;

    SPI_Send((u8 *)p, width * height * 2);
}
//LCD初始化
void LCD_Init(void)
{
	LCD_CS_L;
	LCD_PWR_OFF;
	
	LCD_RES_L;
	delay_ms(120);
	LCD_RES_H;
	delay_ms(120);
	
	delay_ms(120);
	/* Sleep Out */
	LCD_WR_CMD(0x11);
	/* wait for power stability */
	delay_ms(120);

	/* Memory Data Access Control */
	LCD_WR_CMD(0x36);
	LCD_WR_DATA(0x00);

	/* RGB 5-6-5-bit  */
	LCD_WR_CMD(0x3A);
	LCD_WR_DATA(0x65);

	/* Porch Setting */
	LCD_WR_CMD(0xB2);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x33);

	/*  Gate Control */
	LCD_WR_CMD(0xB7);
	LCD_WR_DATA(0x72);

	/* VCOM Setting */
	LCD_WR_CMD(0xBB);
	LCD_WR_DATA(0x3D);   //Vcom=1.625V

	/* LCM Control */
	LCD_WR_CMD(0xC0);
	LCD_WR_DATA(0x2C);

	/* VDV and VRH Command Enable */
	LCD_WR_CMD(0xC2);
	LCD_WR_DATA(0x01);

	/* VRH Set */
	LCD_WR_CMD(0xC3);
	LCD_WR_DATA(0x19);

	/* VDV Set */
	LCD_WR_CMD(0xC4);
	LCD_WR_DATA(0x20);

	/* Frame Rate Control in Normal Mode */
	LCD_WR_CMD(0xC6);
	LCD_WR_DATA(0x0F);	//60MHZ

	/* Power Control 1 */
	LCD_WR_CMD(0xD0);
	LCD_WR_DATA(0xA4);
	LCD_WR_DATA(0xA1);

	/* Positive Voltage Gamma Control */
	LCD_WR_CMD(0xE0);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0x4C);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x23);

	/* Negative Voltage Gamma Control */
	LCD_WR_CMD(0xE1);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x23);

	/* Display Inversion On */
	LCD_WR_CMD(0x21);

	LCD_WR_CMD(0x29);

	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

	LCD_Clear(WHITE);
	/*打开显示*/
	LCD_PWR_ON;
}


























