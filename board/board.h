/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-11-18     greedyhao         first version
 */

#ifndef BOARD_H__
#define BOARD_H__

#include <rtthread.h>
#include <ab32vgx.h>
#ifdef RT_USING_PIN
#include <drv_gpio.h>
#endif
#ifdef RT_USING_WDT
#include <drv_wdt.h>
#endif


#ifndef USER_UART1_BANDRATE
#define USER_UART1_BANDRATE
#endif

#ifndef AB32_RT_HW_US_DELAY
#define AB32_RT_HW_US_DELAY
#endif

//#define RT_SPI_SOFT
//#define BSP_USING_SOFT_SPI1

#endif
