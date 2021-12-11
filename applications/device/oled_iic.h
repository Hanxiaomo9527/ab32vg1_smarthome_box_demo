/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */
#ifndef APPLICATIONS_OLED_IIC_H_
#define APPLICATIONS_OLED_IIC_H_

#include "rtthread.h"
#include <rtdevice.h>

//#include "stdlib.h"
#define OLED_MODE 0
#define SIZE 8
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64
//---- OLED IIC 端口定义----------------
#define SDA_PIN rt_pin_get("PE.2")
#define SCL_PIN rt_pin_get("PE.3")

#define oled_sclk_clr() rt_pin_write(SCL_PIN, PIN_LOW)
#define oled_sclk_set() rt_pin_write(SCL_PIN, PIN_HIGH)

#define oled_sdin_clr() rt_pin_write(SDA_PIN, PIN_LOW)
#define oled_sdin_set() rt_pin_write(SDA_PIN, PIN_HIGH)

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

//OLED控制用函数
void oled_write_byte(unsigned dat,unsigned cmd);
void oled_set_pos(unsigned char x, unsigned char y);
void oled_display_on(void);
void oled_display_off(void);
void oled_clear(void);
void oled_showchar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void oled_shownum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void oled_showstring(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);
void oled_showchinese(uint8_t x,uint8_t y,uint8_t no);
void oled_drawbmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
//void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
//void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);

void fill_picture(unsigned char fill_Data);

//void Picture();
void iic_start();
void iic_stop();
void write_iic_command(unsigned char IIC_Command);
void write_iic_data(unsigned char IIC_Data);
void write_iic_byte(unsigned char IIC_Byte);
void iic_wait_ack();

void ab32_oled_init(void);

#endif /* APPLICATIONS_OLED_IIC_H_ */
