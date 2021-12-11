/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-9      Sam XIE      the first version
 *
 * thanks:  qwz
 */

#ifndef __DRV_SOFT_SPI_H_
#define __DRV_SOFT_SPI_H_

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include <drv_common.h>

rt_err_t rt_soft_spi_device_attach(const char *bus_name, const char *device_name, hal_sfr_t cs_gpiox, rt_uint8_t cs_gpio_pin);

struct ab32_soft_spi_pin
{
    hal_sfr_t GPIOx;
    rt_uint8_t GPIO_Pin;
};

struct ab32_soft_spi_config
{
    rt_uint8_t mosi_pin;
	rt_uint8_t miso_pin;
	rt_uint8_t sclk_pin;
    char *bus_name;
};

struct ab32_soft_spi_device
{
    rt_uint8_t cs_pin;
    char *bus_name;
    char *device_name;
};

/* ab32 soft spi dirver class */
struct ab32_soft_spi
{
	uint8_t mode;
	uint8_t cpha;
	uint8_t cpol;
	uint8_t data_width;
	uint8_t msb;
	uint16_t dummy_data;
	uint32_t spi_delay;
	uint8_t max_hz;
    struct ab32_soft_spi_config *config;
    struct rt_spi_configuration *cfg;
    struct rt_spi_bus spi_bus;
};

#endif /*__DRV_SOFT_SPI_H_ */
