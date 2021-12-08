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
<<<<<<< HEAD
#include "common.h"
=======
#include "./device/include/common.h"
#include "sensor.h"

>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4

#define  LOG_TAG  "main"
#include <ulog.h>
#include "drv_gpio.h"

<<<<<<< HEAD

int main(void)
{
#if 1
    rt_kprintf("Welcome to use smarthome box\n");
    ab32_led_init();
    ab32_oled_init();
    ab32_motor_init();
    ab32_button_init();
    ab32_uart_init(0, RT_NULL);
    ab32_event_manager_init();
    // ab32_dht11_init();
    // ad32_infrared_sensor_init();
#endif
#if 0
=======
#if 0
static int app_watchdog_hook_default(ab32_handle_t handle)
{
    LOG_D("app_watchdog_hook_default");

    return RT_EOK;
}

static int app_wifistatus_hook_default(ab32_handle_t handle)
{
    LOG_D("app_wifistatus_hook_default");

    return RT_EOK;
}

ab32_handle_t ab32_instance_manager(void)
{
    ab32_handle_t ab32_handle = NULL;
    ab32_uart_init();
    ab32_oled_init();
    ab32_sdcard_init();
    ab32_button_init();
    ab32_led_init();
    ab32_dht11_init();
    ab32_audio_init();

    return ab32_handle;
}

static ab32_hooks_t app_hooks = {
    .reboot = app_watchdog_hook_default,
    .notify_wifi_status = app_wifistatus_hook_default,
    .notify_sd_filesize = NULL
};
#endif

// ab32_handle_t g_instance_handle = NULL;


int main(void)
{
#if 0
    int ret = -1;

    g_instance_handle = ab32_instance_manager();
    if(NULL == g_instance_handle){
        LOG_E("application create error");
        return RT_ERROR;
    }

//    ab32_hooks_register(g_instance_handle, &app_hooks);
#endif

#if 1
>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
    uint8_t pin = rt_pin_get("PE.1");
    rt_pin_mode(pin, PIN_MODE_OUTPUT);

    rt_kprintf("Hello, world\n");
    ab32_uart_init(0, RT_NULL);

    while (1)
    {
        rt_pin_write(pin, PIN_LOW);
        rt_thread_mdelay(500);
        rt_pin_write(pin, PIN_HIGH);
        rt_thread_mdelay(500);
    }
#endif
}
