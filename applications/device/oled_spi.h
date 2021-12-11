/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */
#ifndef APPLICATIONS_OLED_SPI_H_
#define APPLICATIONS_OLED_SPI_H_

#include "rtthread.h"
#include <rtdevice.h>

//#include "stdlib.h"
#define OLED_MODE 0
#define SIZE 16
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64



//---- OLED SPI 端口定义----------------

// #define SPI_CS_PIN   rt_pin_get("PE.4")
// D0 时钟
// #define SPI_SCLK_PIN rt_pin_get("PA.0")
// D1 串行数据
// #define SPI_MOSI_PIN rt_pin_get("PA.1")
// D/C 数据/命令
// #define SPI_MISO_PIN rt_pin_get("PA.2")





#define oled_cs_clr() rt_pin_write(SPI_CS_PIN, PIN_LOW)
#define oled_cs_set() rt_pin_write(SPI_CS_PIN, PIN_HIGH)

#define oled_dc_clr() rt_pin_write(SPI_DC_PIN, PIN_LOW)
#define oled_dc_set() rt_pin_write(SPI_DC_PIN, PIN_HIGH)

#define oled_rst_clr()
#define oled_rst_set()

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

//-------------------------------- display define ------------------------------
// for 8*16 char can only display 4 rows, 16 chars each row.
#define OLED_DISPLAY_ROW_1    0    // yellow
#define OLED_DISPLAY_ROW_2    2    // blue
#define OLED_DISPLAY_ROW_3    4    // blue
#define OLED_DISPLAY_ROW_4    6    // blue
#define OLED_DISPLAY_COLUMN_START    0    // colloum from left pos 0

#define OLED_DISPLAY_MAX_CHAR_PER_ROW    16   // max 16 chars each row


//OLED控制用函数
void oled_write_byte(unsigned dat,unsigned cmd);
void oled_write_bytse(unsigned *dat, unsigned len, unsigned cmd);
void oled_display_on(void);
void oled_display_off(void);

//void oled_drawpoint(uint8_t x,uint8_t y,uint8_t t);
//void oled_fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void oled_showchar(uint8_t x,uint8_t y,uint8_t chr);
void oled_shownum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);

void oled_set_pos(unsigned char x, unsigned char y);
void oled_showchinese(uint8_t x,uint8_t y,uint8_t no);
void oled_drawbmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void fill_picture(unsigned char fill_Data);

void ab32_oled_init(void);
//void oled_clear(void);
void oled_showstring(uint8_t x,uint8_t y,char *chr);


//void Picture();
// void iic_start();
// void iic_stop();
// void write_iic_command(unsigned char IIC_Command);
// void write_iic_data(unsigned char IIC_Data);
// void write_iic_byte(unsigned char IIC_Byte);
// void iic_wait_ack();



#endif /* APPLICATIONS_OLED_SPI_H_ */
