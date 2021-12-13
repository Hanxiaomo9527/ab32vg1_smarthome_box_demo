/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-12-8      Sam XIE       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "common.h"

#define MOTOR_OFF 0
#define MOTOR_ON  1

static rt_thread_t event_manager_thread = RT_NULL;
struct rt_event control_event;


static void control_event_init(void)
{
    rt_kprintf("control event init!\r");
    rt_event_init(&control_event, "control_event", RT_IPC_FLAG_FIFO);
}


/* 事件处理线程 */
static void event_manager_thread_entry(void *parameter)
{
    while(1)
    {
        if(rt_event_recv(&control_event,
                       (EVENT_MOTOR_OPEN_FLAG | EVENT_MOTOR_CLOSE_FLAG | EVENT_LED_RED_FLAG | EVENT_LED_GREEN_FLAG | EVENT_LED_BLUE_FLAG | EVENT_LED_CLOSE_FLAG),
                       RT_EVENT_FLAG_OR,
                       RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {

            if(rt_event_recv(&control_event, EVENT_MOTOR_OPEN_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_motor_value(MOTOR_ON);
                // oled show
            }

            if(rt_event_recv(&control_event, EVENT_MOTOR_CLOSE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_motor_value(MOTOR_OFF);
                // oled show
            }

            if(rt_event_recv(&control_event,EVENT_LED_RED_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                rgb_led_open(60, 0, 0);
                // oled show
            }

            if (rt_event_recv(&control_event, EVENT_LED_GREEN_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                rgb_led_open(0, 60, 0);
                // oled show
            }

            if (rt_event_recv(&control_event, EVENT_LED_BLUE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                rgb_led_open(0, 0, 60);
                // oled show
            }

            if (rt_event_recv(&control_event, EVENT_LED_CLOSE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                rgb_led_open(60, 60, 60);
                rt_thread_delay(2000);
                ab32_rgb_led_close();
                // oled show
            }
        }
    }
}

int ab32_event_manager_init(void)
{
    control_event_init();
    event_manager_thread = rt_thread_create("event_manager_thread", event_manager_thread_entry, RT_NULL, 1024*2, 26, 10);
    if (event_manager_thread != RT_NULL){
        rt_thread_startup(event_manager_thread);
    }else{
        rt_kprintf("creat event manager failed!\r\n");
        return RT_ERROR;
    }

    return RT_EOK;
}
//INIT_APP_EXPORT(ab32_event_manager_init);

