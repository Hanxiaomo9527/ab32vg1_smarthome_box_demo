/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author     Notes
 * 2021-12-8      Sam XIE      the first version
 */
#include <rtthread.h>
#include "board.h"

#define LED_RED_PIN     rt_pin_get("PE.1")
#define LED_GREEN_PIN   rt_pin_get("PE.4")
#define LED_BLUE_PIN    rt_pin_get("PA.1")

typedef struct
{
    rt_uint8_t red :1;
    rt_uint8_t green :1;
    rt_uint8_t blue :1;
    rt_uint8_t bReserved :5;
} rt_rgb_status;

typedef struct
{
    rt_uint8_t red_pin;
    rt_uint8_t blue_pin;
    rt_uint8_t green_pin;
    union
    {
        rt_uint8_t status;
        rt_rgb_status bit_rgb_status;
    };
} rt_rgb_led;

#ifdef OPEN_LED_THREAD
static rt_thread_t led_thread = RT_NULL;
#endif

static rt_rgb_led led = { 0 };

void set_led_red(rt_bool_t on)
{
    rt_pin_write(led.red_pin, on);
    led.bit_rgb_status.red = on;
}

void set_led_green(rt_bool_t on)
{
    rt_pin_write(led.green_pin, on);
    led.bit_rgb_status.green = on;
}

void set_led_blue(rt_bool_t on)
{
    rt_pin_write(led.blue_pin, on);
    led.bit_rgb_status.blue = on;
}

rt_uint8_t led_read(void)
{
    return led.status;
}

void led_write(rt_bool_t r, rt_bool_t g, rt_bool_t b)
{
    set_led_red(r);
    set_led_green(g);
    set_led_blue(b);
}

void rgb_write(rt_uint8_t status)
{
    led.status = status;
    rt_pin_write(led.red_pin, led.bit_rgb_status.red);
    rt_pin_write(led.green_pin, led.bit_rgb_status.green);
    rt_pin_write(led.blue_pin, led.bit_rgb_status.blue);
}

#ifdef OPEN_LED_THREAD
static void led_event_manager()
{
    if (rt_event_recv(&control_event,
                      (UP_FLAG | ENTRY_FLAG | RETURN_FLAG | DOWN_FLAG),
                      RT_EVENT_FLAG_OR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
        // todo:
    }
}

static void led_thread_entry(void *parameter)
{
#if 0
    rt_uint8_t t = 0;

    while (1)
    {
        if(t>255) t = 0;
        else t++;

        rgb_write(t);
        rt_thread_mdelay(50);
        rgb_write(0xff);// close
        rt_thread_mdelay(950);
    }
#endif
    while(1)
    {
        /* 5ms */
        rt_thread_delay(RT_TICK_PER_SECOND/200);
        led_event_manager();
    }
}
#endif


int ab32_led_init(void)
{
    int ret = RT_ERROR;
    rt_kprintf("led init!!\r\n");


    led.red_pin   = LED_RED_PIN;
    led.green_pin = LED_GREEN_PIN;
    led.blue_pin  = LED_BLUE_PIN;
    rt_pin_mode(led.red_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(led.green_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(led.blue_pin, PIN_MODE_OUTPUT);

#ifdef OPEN_LED_THREAD
    led_thread = rt_thread_create("led", led_thread_entry, RT_NULL, 512, 25, 10);
    /* 创建成功则启动线程 */
    if (led_thread != RT_NULL){
        rt_thread_startup(led_thread);
        rt_kprintf("led thread create OK\n");
        ret = RT_EOK;
    }
    else{
        rt_kprintf("led thread create Failed\n");
        return ret;
    }
#endif
    return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ab32_led_init, rgb_led_sample);

