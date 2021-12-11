/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */

/**
 * Notice!
 * All functions or data that are called during an interrupt need to be in RAM.
 * You can do it the way exception_isr() does.
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "common.h"


#define  LOG_TAG  "main"
#include <ulog.h>
#include "drv_gpio.h"


int main(void)
{
    rt_kprintf("\r\n==================================\r");
    rt_kprintf("|* Welcome to use smarthome box *|\r");
    rt_kprintf("==================================\r\n");
    ab32_oled_init();
    rt_thread_delay(500);
    ab32_rgb_led_close();
    ab32_dht11_init();
//    ab32_oled_init();
//    oled_clear();
//    load_menu();

    ab32_motor_init();
    ab32_button_init();
    ab32_uart_init(0, RT_NULL);
    ab32_event_manager_init();
    // ad32_infrared_sensor_init();
}
