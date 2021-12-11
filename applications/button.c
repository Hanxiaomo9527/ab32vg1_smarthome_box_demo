/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include <multi_button.h>
#include "common.h"

#define BUTTON_LED_PIN    rt_pin_get("PA.5")
#define BUTTON_MOTOR_PIN  rt_pin_get("PE.1")

static struct button button_led;
static struct button button_motor;


oled_msg_data_t oled_btn_data_msg;

static uint8_t button_read_led_pin(void)
{
    return rt_pin_read(BUTTON_LED_PIN);
}

static uint8_t button_read_motor_pin(void)
{
    return rt_pin_read(BUTTON_MOTOR_PIN);
}

static void button_set_led_callback(void *btn)
{
    uint32_t btn_event_val;
    rt_uint32_t set = 0;
    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case SINGLE_CLICK: // red
            rt_kprintf("set led red!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"set rgb_led: red!!");
            set = EVENT_LED_RED_FLAG;
            break;

        case DOUBLE_CLICK: // green
            rt_kprintf("set led green!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"set rgb_led: green!!");
            set = EVENT_LED_GREEN_FLAG;
            break;

        case LONG_PRESS_START: // blue
            rt_kprintf("set led blue!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"set rgb_led: blue!!");
            set = EVENT_LED_BLUE_FLAG;
            break;

        case LONG_PRESS_HOLD:  // close led
            rt_kprintf("set led close!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"set rgb_led: close!!");
            set = EVENT_LED_CLOSE_FLAG;
            break;


        default:
            set = NO_EVENT_FLAG;
            break;
    }
    if(set != NO_EVENT_FLAG)
    {
        //  need mutex_lock
        rt_event_send(&control_event, set);
        rt_mq_send(oled_messagequeue, (void*)&oled_btn_data_msg, sizeof(oled_msg_data_t));
    }
}

static void button_ctrl_motor_callback(void *btn)
{
    uint32_t btn_event_val;
    rt_uint32_t set = 0;
    btn_event_val = get_button_event((struct button *)btn);

    switch(btn_event_val)
    {
        case PRESS_DOWN: // open
            rt_kprintf("control motor open!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"control moto: open!!");
            set = EVENT_MOTOR_OPEN_FLAG;
            break;

        case PRESS_UP: // close
            rt_kprintf("control motor close!\r\n");
            strcpy(&oled_btn_data_msg.oled_view_data.action_status,"control moto: close!!");
            set = EVENT_MOTOR_CLOSE_FLAG;
            break;

        default:
            set = NO_EVENT_FLAG;
            break;
    }
    if(set != NO_EVENT_FLAG){
          //  need mutex_lock
          rt_event_send(&control_event, set);
          rt_mq_send(oled_messagequeue, (void*)&oled_btn_data_msg, sizeof(oled_msg_data_t));
     }
}

static void button_thread_entry(void* p)
{
    while(1)
    {
        /* 5ms */
        rt_thread_delay(RT_TICK_PER_SECOND/200);
        button_ticks();
    }
}


int ab32_button_init(void)
{
    rt_thread_t button_thread = RT_NULL;
    rt_kprintf("button init!!\r");
    /* Create background ticks thread */
    button_thread = rt_thread_create("button", button_thread_entry, RT_NULL, 512, 20, 10);
    if(button_thread != RT_NULL){
        rt_thread_startup(button_thread);
    }else{
        rt_kprintf("create button thread failed!\r\n");
        return RT_ERROR;
    }

    /* low level drive */
    rt_pin_mode(BUTTON_LED_PIN, PIN_MODE_INPUT_PULLUP);
    button_init(&button_led, button_read_led_pin, PIN_LOW);
    button_attach(&button_led, SINGLE_CLICK,     button_set_led_callback);
    button_attach(&button_led, DOUBLE_CLICK,     button_set_led_callback);
    button_attach(&button_led, LONG_PRESS_START, button_set_led_callback);
    button_attach(&button_led, LONG_PRESS_HOLD,         button_set_led_callback);
    button_start(&button_led);

    rt_pin_mode(BUTTON_MOTOR_PIN, PIN_MODE_INPUT_PULLUP);
    button_init(&button_motor, button_read_motor_pin, PIN_LOW);
    button_attach(&button_motor, PRESS_DOWN,   button_ctrl_motor_callback);
    button_attach(&button_motor, PRESS_UP,     button_ctrl_motor_callback);
    button_start(&button_motor);

    return RT_EOK;
}

// INIT_APP_EXPORT(ab32_button_init);

