/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include <rthw.h>

/* 按键事件 */
#define NO_EVENT_FLAG       (0)
#define EVENT_MOTOR_OPEN_FLAG     (1 << 0)
#define EVENT_MOTOR_CLOSE_FLAG    (1 << 1)
#define EVENT_LED_RED_FLAG        (1 << 2)
#define EVENT_LED_GREEN_FLAG      (1 << 3)
#define EVENT_LED_BLUE_FLAG       (1 << 4)
#define EVENT_LED_CLOSE_FLAG      (1 << 5)

extern struct rt_event control_event; //功能控制事件
extern rt_mq_t oled_messagequeue; // oled 消息队列

typedef struct _oled_data_t{
    int temp_value;
    int humi_value;
    char action_status[64];
}oled_data_t;

typedef struct _oled_msg_data_t{
    oled_data_t oled_view_data;
 //   rt_uint32_t data_size;
}oled_msg_data_t;


void set_motor_value(rt_uint32_t pwm_value);

void rgb_led_open(uint8_t red, uint8_t green, uint8_t blue);

void ab32_rgb_led_close(void);

//void ab32_spi_init(void);
int ab32_oled_init(void);

int ab32_motor_init(void);

int ab32_button_init(void);

int ab32_dht11_init(void);

int ab32_event_manager_init(void);

int ab32_uart_init(int argc, char *argv[]);   // 8266 通讯 cmd 网络状态监听

#endif /* _COMMON_H_ */
