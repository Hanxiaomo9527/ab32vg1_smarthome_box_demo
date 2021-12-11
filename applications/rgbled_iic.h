/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */
#ifndef APPLICATIONS_RGB_LED_IIC_H_
#define APPLICATIONS_RGB_LED_IIC_H_

#include "rtthread.h"
#include <rtdevice.h>

//---- RGB_LED IIC 端口定义----------------
#define SDA_PIN rt_pin_get("PE.2")
#define SCL_PIN rt_pin_get("PE.3")

#define rgbled_sclk_clr() rt_pin_write(SCL_PIN, PIN_LOW)
#define rgbled_sclk_set() rt_pin_write(SCL_PIN, PIN_HIGH)

#define rgbled_sdin_clr() rt_pin_write(SDA_PIN, PIN_LOW)
#define rgbled_sdin_set() rt_pin_write(SDA_PIN, PIN_HIGH)

//-------------------- user interfaces ---------------------------

/**
 * @brief Initialize RGB LED device.
 *
 * @return none
 */
void ab32_rgb_led_init(void);


/**
 * @brief Set light parameters for RGB LED
 *
 * @param red:    Red light parameter
 * @param green:  Green light parameter
 * @param blue:   Blue light parameter
 *
 * @return none
 */
void rgb_led_open(uint8_t red, uint8_t green, uint8_t blue);


/**
 * @brief Close RGB LED
 *
 * @return none
 */
void rgb_led_close(void);



#endif  // APPLICATIONS_RGB_LED_IIC_H_
