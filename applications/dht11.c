/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-27     Sam Xie       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "sensor.h"
#include "sensor_dallas_dht11.h"
#include "drv_gpio.h"
#include "common.h"

/* Modify this pin according to the actual wiring situation */
#define DHT11_DATA_PIN    GET_PIN(F, 0)

static void dht11_read_temp_thread(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;
    rt_uint8_t get_data_freq = 1; /* 1Hz */

    dev = rt_device_find("temp_dht11");
    if (dev == RT_NULL)
    {
        return;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);

        if (res != 1)
        {
            rt_kprintf("read data failed! result is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
        {
            if (sensor_data.data.temp >= 0)
            {
                uint8_t temp = (sensor_data.data.temp & 0xffff) >> 0;      // get temp
                uint8_t humi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
                rt_kprintf("temp:%d, humi:%d\n" ,temp, humi);
            }
        }

        rt_thread_delay(5000);
    }
}

int ab32_dht11_init(void)
{
    struct rt_sensor_config cfg;
    rt_kprintf("dht11 init!!\r");
    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);

    rt_thread_t dht11_thread = RT_NULL;

    dht11_thread = rt_thread_create("dht_tem", dht11_read_temp_thread, RT_NULL, 512, RT_THREAD_PRIORITY_MAX / 2, 20);
    if (dht11_thread != RT_NULL){
        rt_thread_startup(dht11_thread);
    }else{
        rt_kprintf("create dht11 read temp thread failed!\r\n");
        return RT_ERROR;
    }
    return RT_EOK;
}
