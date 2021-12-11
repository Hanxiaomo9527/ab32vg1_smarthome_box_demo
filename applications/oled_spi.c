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
#include "common.h"

rt_mq_t oled_messagequeue = RT_NULL; // oled 消息队列

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

int temp_bak = 0;
int humi_bak = 0;
void ssd1306_mainview_update(int screencolor, int wordcolor, int tempvalue, int humivalue, const char *actionstatus)
{
    char message[20] = "";
    if(humivalue && tempvalue){
        humi_bak = humivalue;
        temp_bak = tempvalue;
    }
    rt_sprintf(message, "TEMP:%d   HUMI:%d",temp_bak, humi_bak);

//    rt_kprintf("!!temp:%d, humi:%d, action_status:%s\r\n",tempvalue, humivalue, actionstatus);

    ssd1306_Fill(screencolor);
    ssd1306_SetCursor(2, 0);
    ssd1306_WriteString("RT-Thread", Font_11x18, wordcolor);
    ssd1306_SetCursor(2, 18);
    ssd1306_WriteString("AB32VG1", Font_7x10, wordcolor);
    ssd1306_SetCursor(2, 18+10);
    ssd1306_WriteString(actionstatus, Font_6x8, wordcolor);
    ssd1306_SetCursor(2, 18+10+8);
    ssd1306_WriteString("TIME:21/12/12 00:59", Font_6x8, wordcolor);
    ssd1306_SetCursor(2, 18+10+8+8);
    ssd1306_WriteString(message, Font_6x8, wordcolor);
    ssd1306_SetCursor(2, 18+10+8+8+8);
    ssd1306_WriteString("Sam XIE 17503114086", Font_6x8, wordcolor);
    ssd1306_UpdateScreen();
}


static void oled_update_screen_thread()
{
    oled_msg_data_t recvmsgbuffer;
    char buf[128];
    oled_messagequeue = rt_mq_create("oled_messagequeue", 128, 10, RT_IPC_FLAG_FIFO);
    if(oled_messagequeue != RT_NULL){
        rt_kprintf("create oled_msgqueue successful\r\n");
    }else{
        rt_kprintf("create oled_msgqueue failed\r\n");
        return;
    }

    while(1){
       if(rt_mq_recv(oled_messagequeue, (void*)&recvmsgbuffer, sizeof(oled_msg_data_t), RT_WAITING_FOREVER) != RT_EOK){
            rt_kprintf("recv msg failed\r\n");
        }else{
            rt_kprintf("recv msg humi:%d\r",recvmsgbuffer.oled_view_data.humi_value);
            rt_kprintf("recv msg temp:%d\r",recvmsgbuffer.oled_view_data.temp_value);
            rt_kprintf("msg action_status:%s\r",recvmsgbuffer.oled_view_data.action_status);
            ssd1306_mainview_update(White, Black, recvmsgbuffer.oled_view_data.temp_value, recvmsgbuffer.oled_view_data.humi_value, recvmsgbuffer.oled_view_data.action_status);
        }

    }

}

int ab32_oled_init(void)
{
    static rt_thread_t oled_thread = RT_NULL;

    rt_kprintf("oled init!!\r");

    oled_gpio_init();
    ssd1306_Init();
    ssd1306_TestAll();
    oled_thread = rt_thread_create("oled", oled_update_screen_thread, RT_NULL, 1024, 25, 5);
    if (oled_thread != RT_NULL) {
        rt_thread_startup(oled_thread);
    } else {
        rt_kprintf("create oled thread failed\r\n");
    }

}

