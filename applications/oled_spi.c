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

/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: MIT License
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-15     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "oled_spi.h"


static uint8_t pin_scl;
static uint8_t pin_sda;
static uint8_t pin_res;
static uint8_t pin_dc;
static uint8_t pin_cs;

#define OLED_CLK_1          rt_pin_write(pin_scl,PIN_HIGH)
#define OLED_CLK_0          rt_pin_write(pin_scl,PIN_LOW)

#define OLED_SDIN_1         rt_pin_write(pin_sda,PIN_HIGH)
#define OLED_SDIN_0         rt_pin_write(pin_sda,PIN_LOW)

#define OLED_DC_1           rt_pin_write(pin_dc,PIN_HIGH)
#define OLED_DC_0           rt_pin_write(pin_dc,PIN_LOW)

#define OLED_RST_1          rt_pin_write(pin_res,PIN_HIGH)
#define OLED_RST_0          rt_pin_write(pin_res,PIN_LOW)

#define OLED_CS_1           rt_pin_write(pin_cs,PIN_HIGH)
#define OLED_CS_0           rt_pin_write(pin_cs,PIN_LOW)

//
//void oled_delay(void)
//{
//    uint32_t time = 10;
//    while(time--);
//}

static void oled_write_byte(uint8_t dat)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        OLED_CLK_0;
        if(dat&0x80)
        {
            OLED_SDIN_1;
        }
        else
        {
            OLED_SDIN_0;
        }
        //oled_delay();
        OLED_CLK_1;
        dat<<=1;
        //oled_delay();
    }
    //OLED_CS_1;
    //OLED_DC_1;
}


void ssd1306_Reset(void)
{
    OLED_CS_1;// un-select OLED

    OLED_RST_0;
    rt_thread_mdelay(10);
    OLED_RST_1;
    rt_thread_mdelay(10);
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte)
{
    OLED_DC_0;  // command
    OLED_CS_0;  // select OLED
    oled_write_byte(byte);
    OLED_CS_1;// un-select OLED
    OLED_DC_1;  // data
}

// Send data
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size)
{
    OLED_DC_1;  // data
    OLED_CS_0;  // select OLED
    for(size_t i=0;i<buff_size;i++)
    {
        oled_write_byte(buffer[i]);
    }
    OLED_CS_1;// un-select OLED
    OLED_DC_1;  // data
}


static void oled_gpio_init(void)
{
    pin_scl = rt_pin_get("PA.0");
    pin_sda = rt_pin_get("PA.1");
  //  pin_res = rt_pin_get("PA.0");
    pin_dc = rt_pin_get("PA.2");
    pin_cs = rt_pin_get("PE.4");

    rt_pin_mode(pin_scl, PIN_MODE_OUTPUT);
    rt_pin_mode(pin_sda, PIN_MODE_OUTPUT);
   // rt_pin_mode(pin_res, PIN_MODE_OUTPUT);
    rt_pin_mode(pin_dc, PIN_MODE_OUTPUT);
    rt_pin_mode(pin_cs, PIN_MODE_OUTPUT);

    rt_pin_write(pin_scl,PIN_HIGH);
    rt_pin_write(pin_sda,PIN_HIGH);
   // rt_pin_write(pin_res,PIN_HIGH);
    rt_pin_write(pin_dc,PIN_HIGH);
    rt_pin_write(pin_cs,PIN_HIGH);
}


void ssd1306_TestAll(void)
{
    oled_gpio_init();
    ssd1306_Init();
#if 0
    ssd1306_Fill(Black);
    ssd1306_SetCursor(2, 0);
    ssd1306_WriteString("RT-Thread", Font_11x18, White);
    ssd1306_SetCursor(2, 18);
    ssd1306_WriteString("AB32VG1", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18);
    ssd1306_WriteString("USB POWER", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18+18);
    ssd1306_WriteString("chenbin182 - 3240408", Font_6x8, White);
    ssd1306_UpdateScreen();
#endif
    ssd1306_Fill(Black);
    ssd1306_SetCursor(2, 0);
    ssd1306_WriteString("RT-Thread", Font_11x18, White);
    ssd1306_SetCursor(2, 18);
    ssd1306_WriteString("AB32VG1", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18);
    ssd1306_WriteString("Sam XIE", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18+18);
    ssd1306_WriteString("hanxiaomo12138@gmail.com", Font_6x8, White);
    ssd1306_UpdateScreen();
}

void ssd1306_TestAll(void)
{
    ssd1306_Fill(Black);
    ssd1306_SetCursor(2, 0);
    ssd1306_WriteString("RT-Thread", Font_11x18, White);
    ssd1306_SetCursor(2, 18);
    ssd1306_WriteString("AB32VG1", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18);
    ssd1306_WriteString("Sam XIE", Font_11x18, White);
    ssd1306_SetCursor(2, 18+18+18);
    ssd1306_WriteString("hanxiaomo12138@gmail.com", Font_6x8, White);
    ssd1306_UpdateScreen();
}

uint8_t power_buff[2] = {0};
uint16_t adc_buff[5] = {0};
char str_buff[16] = {0};
static uint32_t value;

void ssd1306_refresh(void)
{
    ssd1306_Fill(Black);

//    vio_read_hold_coils(0,3,power_buff);  //
//    vio_read_input_regs(0,5,adc_buff);  //


    ssd1306_SetCursor(2, 0);
    value = adc_buff[0];
    rt_snprintf(str_buff, 16, "IN   %d.%03d V ",value / 1000, value % 1000);
    ssd1306_WriteString(str_buff, Font_7x10, White);


    ssd1306_SetCursor(2, 12);
//    if(dio_get_val(power_buff,0) > 0)
//    {
//        ssd1306_WriteString("USB1", Font_7x10, Black);
//    }else {
//        ssd1306_WriteString("USB1", Font_7x10, White);
//    }
    value = adc_buff[1];
    rt_snprintf(str_buff, 16, " %d.%03d A ",value / 1000, value % 1000);
    ssd1306_WriteString(str_buff, Font_7x10, White);


    ssd1306_SetCursor(2, 12+12);
//    if(dio_get_val(power_buff,1) > 0)
//    {
//        ssd1306_WriteString("USB2", Font_7x10, Black);
//    }else {
//        ssd1306_WriteString("USB2", Font_7x10, White);
//    }
    value = adc_buff[2];
    rt_snprintf(str_buff, 16, " %d.%03d A ",value / 1000, value % 1000);
    ssd1306_WriteString(str_buff, Font_7x10, White);


    ssd1306_SetCursor(2, 12+12+12);
//    if(dio_get_val(power_buff,2) > 0)
//    {
//        ssd1306_WriteString("USB3", Font_7x10, Black);
//    }else {
//        ssd1306_WriteString("USB3", Font_7x10, White);
//    }
    value = adc_buff[3];
    rt_snprintf(str_buff, 16, " %d.%03d A ",value / 1000, value % 1000);
    ssd1306_WriteString(str_buff, Font_7x10, White);


    ssd1306_SetCursor(2, 12+12+12+12);
    ssd1306_WriteString("chenbin182 - 3240408", Font_6x8, White);
    ssd1306_UpdateScreen();
}

