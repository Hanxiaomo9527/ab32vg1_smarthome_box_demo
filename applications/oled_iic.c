/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 *
 * thanks:  werper 和  Eureka1024
 */

#include "oled_iic.h"
#include "stdlib.h"
#include "oledfont.h"
<<<<<<< HEAD

=======
//#include "delay.h"
>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
//uint8_t OLED_GRAM[8][128];           //定义模拟显存

/**********************************************
 * IIC Start
 **********************************************/
void iic_start()
{
    oled_sclk_set() ;
    oled_sdin_set();
    oled_sdin_clr();
    oled_sclk_clr();
}

/**********************************************
 * IIC Stop
 *********************************************/
void iic_stop()
{
    oled_sclk_set() ;
    oled_sdin_clr();
    oled_sdin_set();
}

void iic_wait_ack()
{
    oled_sclk_set() ;
    oled_sclk_clr();
}

/**********************************************
 * IIC Write byte
 *********************************************/
void write_iic_byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m,da;
    da=IIC_Byte;
    oled_sclk_clr();
    for(i=0;i<8;i++)
    {
        m=da;
        m=m&0x80;
        if(m==0x80)
        {
            oled_sdin_set();
        }
        else
            oled_sdin_clr();

        da=da<<1;
        oled_sclk_set();
        oled_sclk_clr();
    }
}
/**********************************************
 * IIC Write Command
 **********************************************/
void write_iic_command(unsigned char IIC_Command)
{
    iic_start();
    write_iic_byte(0x78);            //Slave address,SA0=0
    iic_wait_ack();
    write_iic_byte(0x00);            //write command
    iic_wait_ack();
    write_iic_byte(IIC_Command);
    iic_wait_ack();
    iic_stop();
}
/**********************************************
 * IIC Write Data
 **********************************************/
void write_iic_data(unsigned char IIC_Data)
{
    iic_start();
    write_iic_byte(0x78);            //D/C#=0; R/W#=0
    iic_wait_ack();
    write_iic_byte(0x40);            //write data
    iic_wait_ack();
    write_iic_byte(IIC_Data);
    iic_wait_ack();
    iic_stop();
}


void oled_write_byte(unsigned data,unsigned cmd)
{
    if(cmd)
    {
        write_iic_data(data);
    }
    else
    {
        write_iic_command(data);
    }
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
    oled_write_byte(0xb0+y,OLED_CMD);
    oled_write_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    oled_write_byte((x&0x0f),OLED_CMD);
}
//开启OLED显示
void oled_display_on(void)
{
    oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_write_byte(0X14,OLED_CMD);  //DCDC ON
    oled_write_byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示
void oled_display_off(void)
{
    oled_write_byte(0X8D,OLED_CMD);  //SET DCDC命令
    oled_write_byte(0X10,OLED_CMD);  //DCDC OFF
    oled_write_byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的和没点亮一样!!!
void oled_clear(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        oled_write_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        oled_write_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        oled_write_byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)oled_write_byte(0,OLED_DATA);
    } //更新显示
}
void oled_on(void)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        oled_write_byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        oled_write_byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        oled_write_byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)oled_write_byte(1,OLED_DATA);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void oled_showchar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
//    unsigned char c=0,i=0;
//    c=chr - ' ';//得到偏移后的值
//    if(x>Max_Column-1){x=0;y=y+2;}
//    if(Char_Size ==16)
//    {
//        oled_set_pos(x,y);
//        for(i=0;i<8;i++)
//            oled_write_byte(F8X16[c*16+i],OLED_DATA);
//
//
//        oled_set_pos(x,y+1);
//        for(i=0;i<8;i++)
//            oled_write_byte(F8X16[c*16+i+8],OLED_DATA);
//    }
//    else
//    {
//        oled_set_pos(x,y);
//        for(i=0;i<6;i++)
//        oled_write_byte(F6x8[c][i],OLED_DATA);
//    }

    unsigned char c=0,i=0;
    c=chr - ' ';//得到偏移后的值
    if(x>Max_Column-1){x=0;y=y+2;}

    oled_set_pos(x,y);
    iic_start();
    write_iic_byte(0x78);            //D/C#=0; R/W#=0
    iic_wait_ack();
    write_iic_byte(0x40);            //write data
    iic_wait_ack();
    for(i=0; i<8; i++)
    {
        write_iic_byte(F8X16[c*16+i]);
        iic_wait_ack();
    }
    iic_stop();

    oled_set_pos(x,y+1);
    iic_start();
    write_iic_byte(0x78);            //D/C#=0; R/W#=0
    iic_wait_ack();
    write_iic_byte(0x40);            //write data
    iic_wait_ack();
    for(i=0; i<8; i++)
    {
        write_iic_byte(F8X16[c*16+i+8]);
        iic_wait_ack();
    }
    iic_stop();
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
                oled_showchar(x+(size2/2)*t,y,' ',size2);
                continue;
            }
            else
                enshow=1;
        }
        oled_showchar(x+(size2/2)*t,y,temp+'0',size2);
    }
}
//显示一个字符号串
void oled_showstring(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        if(chr[j] == '.') break; //遇到 '.'终止显示

        oled_showchar(x,y,chr[j],Char_Size);
        x+=8;
        if(x>120)
        {
            x=0;y+=2;
        }
        j++;
    }
}

//显示汉字  (x,y)为坐标，no为数组位置
void oled_showchinese(uint8_t x,uint8_t y,uint8_t no)
{
    uint8_t t;

    oled_set_pos(x,y);
    //批量写入数据
    iic_start();
    write_iic_byte(0x78);            //D/C#=0; R/W#=0
    iic_wait_ack();
    write_iic_byte(0x40);            //write data
    iic_wait_ack();
    for(t=0; t<16; t++)
    {
        write_iic_byte(Hzk[2*no][t]);
        iic_wait_ack();
    }
    iic_stop();

    oled_set_pos(x,y+1);
    iic_start();
    write_iic_byte(0x78);            //D/C#=0; R/W#=0
    iic_wait_ack();
    write_iic_byte(0x40);            //write data
    iic_wait_ack();
    for(t=0;t<16;t++)
    {
        write_iic_byte(Hzk[2*no+1][t]);
        iic_wait_ack();
    }
    iic_stop();
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void oled_drawbmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    for(y=y0;y<y1;y++)
    {
        oled_set_pos(x0,y);

        iic_start();
        write_iic_byte(0x78);            //D/C#=0; R/W#=0
        iic_wait_ack();
        write_iic_byte(0x40);            //write data
        iic_wait_ack();
        for(x=x0;x<x1;x++)
        {
            write_iic_byte(BMP[j++]);
            iic_wait_ack();
        }
        iic_stop();
    }
}


//初始化SSD1306
void ab32_oled_init(void)
{
<<<<<<< HEAD
    rt_kprintf("oled init!!\r\n");
=======
>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
    rt_pin_mode(SDA_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SCL_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(SDA_PIN, PIN_HIGH);
    rt_pin_write(SCL_PIN, PIN_HIGH);

    rt_thread_mdelay(800);
    oled_write_byte(0xAE,OLED_CMD);//--display off
    oled_write_byte(0x00,OLED_CMD);//---set low column address
    oled_write_byte(0x10,OLED_CMD);//---set high column address
    oled_write_byte(0x40,OLED_CMD);//--set start line address
    oled_write_byte(0xB0,OLED_CMD);//--set page address
    oled_write_byte(0x81,OLED_CMD); // contract control
    oled_write_byte(0xFF,OLED_CMD);//--128
    oled_write_byte(0xA1,OLED_CMD);//set segment remap
    oled_write_byte(0xA6,OLED_CMD);//--normal / reverse
    oled_write_byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    oled_write_byte(0x3F,OLED_CMD);//--1/32 duty
    oled_write_byte(0xC8,OLED_CMD);//Com scan direction
    oled_write_byte(0xD3,OLED_CMD);//-set display offset
    oled_write_byte(0x00,OLED_CMD);//

    oled_write_byte(0xD5,OLED_CMD);//set osc division
    oled_write_byte(0x80,OLED_CMD);//

    oled_write_byte(0xD8,OLED_CMD);//set area color mode off
    oled_write_byte(0x05,OLED_CMD);//

    oled_write_byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    oled_write_byte(0xF1,OLED_CMD);//

    oled_write_byte(0xDA,OLED_CMD);//set com pin configuartion
    oled_write_byte(0x12,OLED_CMD);//

    oled_write_byte(0xDB,OLED_CMD);//set Vcomh
    oled_write_byte(0x30,OLED_CMD);//

    oled_write_byte(0x8D,OLED_CMD);//set charge pump enable
    oled_write_byte(0x14,OLED_CMD);//

    oled_write_byte(0xAF,OLED_CMD);//--turn on oled panel
}
