/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 *
 */

#include "oled_spi.h"
#include "stdlib.h"
#include "oledfont.h"


#define SPI_CS_PIN  rt_pin_get("PE.1")
#define SPI_DC_PIN  rt_pin_get("PE.2")

extern struct rt_spi_device *spi_dev_oled;

void oled_write_bytes(unsigned *dat, unsigned len, unsigned cmd)
{
    if(cmd)
    {
        oled_dc_set();
    }
    else
    {
        oled_dc_clr();
    }  
    oled_dc_set();
}


void oled_write_byte(unsigned data,unsigned cmd)
{   
    oled_write_bytes(&data, 1, cmd);
}


/********************************************
 * fill_Picture
 ********************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        oled_write_byte(0xb0+m,0);     //page0-page1
        oled_write_byte(0x00,0);       //low column start address
        oled_write_byte(0x10,0);       //high column start address
        for(n=0;n<128;n++)
        {
            oled_write_byte(fill_Data,1);
        }
    }
}


//坐标设置
void oled_set_pos(unsigned char x, unsigned char y)
{
    uint8_t tmp[3] = {0xb0+y, ((x&0xf0)>>4)|0x10, (x&0x0f)|0x01};
    oled_write_bytes(tmp, 3, OLED_CMD);
#if 0
    oled_write_byte(0xb0+y,OLED_CMD);
    oled_write_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    oled_write_byte((x&0x0f),OLED_CMD);
#endif
}
//开启OLED显示
void oled_display_on(void)
{
    uint8_t tmp[3] = {0X8D, 0X14, 0XAF};
    oled_write_bytes( tmp, 3, OLED_CMD);
#if 0
    oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_write_byte(0X14,OLED_CMD);  //DCDC ON
    oled_write_byte(0XAF,OLED_CMD);  //DISPLAY ON
#endif
}

//关闭OLED显示
void oled_display_off(void)
{
    uint8_t tmp[3] = {0X8D, 0X10, 0XAE};
    oled_write_bytes( tmp, 3, OLED_CMD);
#if 0
    oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_write_byte(0X10,OLED_CMD);  //DCDC OFF
    oled_write_byte(0XAE,OLED_CMD);  //DISPLAY OFF
#endif
}
//清屏函数,清完屏,整个屏幕是黑色的和没点亮一样!!!
void oled_clear(void)
{
    uint8_t i;
    uint8_t tmp_cmd[3] = {0X0, 0x00, 0x10};
    uint8_t tmp[128];
    memset( tmp, 0x0, 128 );
    for(i=0;i<8;i++)  
    {  
        tmp_cmd[0] = 0xb0+i;
        oled_write_bytes( tmp_cmd, 3, OLED_CMD);
        oled_write_bytes( tmp, 128, OLED_DATA);
    }//更新显示
#if 0
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        oled_write_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        oled_write_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        oled_write_byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)oled_write_byte(0,OLED_DATA);
    } //更新显示
#endif
}
void oled_on(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        oled_write_byte(0xb0+i,OLED_CMD);    //设置页地址（0~7）
        oled_write_byte(0x00,OLED_CMD);      //设置显示位置—列低地址
        oled_write_byte(0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)oled_write_byte(1,OLED_DATA);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void oled_showchar(uint8_t x,uint8_t y,uint8_t chr)
{
    unsigned char c=0;	
    c=chr-' ';//得到偏移后的值
    if(x>Max_Column-1){x=0;y=y+2;}
    if(SIZE ==16)
    {
        oled_set_pos(x,y);
        oled_write_bytes( (uint8_t *)&F8X16[c*16], 8, OLED_DATA );
        oled_set_pos(x,y+1);
        oled_write_bytes( (uint8_t *)&F8X16[c*16+8], 8, OLED_DATA );
    }
    else {	
        oled_set_pos(x,y+1);
        oled_write_bytes( (uint8_t *)F6x8[c], 6, OLED_DATA );
    }
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式   0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void oled_shownum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                oled_showchar(x+(size2/2)*t,y,' ');
                continue;
            }
            else
                enshow=1;
        }
        oled_showchar(x+(size2/2)*t,y,temp+'0');
    }
}
//显示一个字符号串
void oled_showstring(uint8_t x,uint8_t y,char *chr)
{
  unsigned char j=0;
  uint8_t x_t = x,y_t = y;
  
  while (chr[j]!='\0')
  {	
    // add for CR/LF
    if( ('\r' == chr[j]) && ('\n' == chr[j+1]) ){  // CR LF
      while(x_t <= 120){  // fill rest chars in current line
        oled_showchar(x_t,y_t,' ');
        x_t += 8;
      }
      j += 2;
    }
    else if( ('\r' == chr[j]) || ('\n' == chr[j]) ){   // CR or LF
      while(x_t <= 120){  // fill rest chars in current line
        oled_showchar(x_t,y_t,' ');
        x_t += 8;
      }
      j += 1;
    }
    else{
      if(x_t>120){  // line end, goto next line
        x_t = 0;
        y_t += 2;
        if(y_t >= 8){  // can only display 4 line
          break;
        }
      }
      oled_showchar(x_t,y_t,chr[j]);
      x_t += 8;
      j++;
    }
  }
}

//显示汉字  (x,y)为坐标，no为数组位置
void oled_showchinese(uint8_t x,uint8_t y,uint8_t no)
{
    uint8_t t,adder=0;
    oled_set_pos(x,y);	
    for(t=0;t<16;t++)
    {
        oled_write_byte(Hzk[2*no][t],OLED_DATA);
        adder+=1;
    }	
    oled_set_pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
        oled_write_byte(Hzk[2*no+1][t],OLED_DATA);
        adder+=1;
    }		
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oled_drawbmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;
    
    if(y1%8==0) y=y1/8;      
    else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        oled_set_pos(x0,y);
        for(x=x0;x<x1;x++)
        {      
            oled_write_byte(BMP[j++],OLED_DATA);
        }
    }
}

void load_menu(void)
{
    oled_showstring(30,0,"Loading...");
    oled_drawbmp(0,2,128,8,loading_bmp); //RT-Thread图标

    rt_thread_delay(2000);
}

//初始化SSD1306
void ab32_oled_init(void)
{
    rt_kprintf("oled init!!\r");

    rt_pin_mode(SPI_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SPI_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(SPI_DC_PIN, PIN_HIGH);
    rt_pin_write(SPI_DC_PIN, PIN_HIGH);


    oled_dc_clr();
    oled_rst_clr();
    oled_rst_set();
    rt_thread_mdelay(100);
    oled_rst_clr();
    rt_thread_mdelay(100);
    oled_rst_set(); 

    oled_write_byte(0xAE,OLED_CMD);//--turn off oled panel
    oled_write_byte(0x00,OLED_CMD);//---set low column address
    oled_write_byte(0x10,OLED_CMD);//---set high column address
    oled_write_byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_write_byte(0x81,OLED_CMD);//--set contrast control register
    oled_write_byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    oled_write_byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     
    oled_write_byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction  
    oled_write_byte(0xA6,OLED_CMD);//--set normal display
    oled_write_byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    oled_write_byte(0x3F,OLED_CMD);//--1/64 duty
    oled_write_byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    oled_write_byte(0x00,OLED_CMD);//

    oled_write_byte(0xD5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    oled_write_byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec

    oled_write_byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    oled_write_byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

    // oled_write_byte(0xD8,OLED_CMD);//set area color mode off
    // oled_write_byte(0x05,OLED_CMD);//

    oled_write_byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    oled_write_byte(0x12,OLED_CMD);//

    oled_write_byte(0xDB,OLED_CMD);//set Vcomh
    oled_write_byte(0x40,OLED_CMD);//Set VCOM Deselect Level

    oled_write_byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    oled_write_byte(0x02,OLED_CMD);//

    oled_write_byte(0x8D,OLED_CMD);//set charge pump enable
    oled_write_byte(0x14,OLED_CMD);//

    oled_write_byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    oled_write_byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)   
 
    oled_clear();
    oled_set_pos(0,0); 
    oled_write_byte(0xAF,OLED_CMD);/*display ON*/ 

}
