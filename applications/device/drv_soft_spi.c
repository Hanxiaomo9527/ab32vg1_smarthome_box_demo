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

#include "board.h"

#ifdef RT_USING_SPI
#ifdef RT_SPI_SOFT

#include "../rt-thread/components/drivers/include/drivers/spi.h"
#include "drv_soft_spi.h"

#include <string.h>

// #define DRV_DEBUG
#define LOG_TAG              "drv.spisoft"
#include <drv_log.h>

enum{
#ifdef BSP_USING_SOFT_SPI1
    SOFT_SPI1_INDEX,
#endif
};

#define SPI0_MOSI_PIN "PE.1"
#define SPI0_MISO_PIN "PF.0"
#define SPI0_SCLK_PIN "PE.0"
//CS = PA.5
#define SOFT_SPI1_BUS_CONFIG { \
.mosi_pin = 14, \
.miso_pin = 21, \
.sclk_pin = 13, \
.bus_name = "spi0", \
}

// //PB2 10 ;PE5 18;PE6 19;PB1 9;
// #define SOFT_SPI1_BUS_CONFIG {              \
//     .mosi_pin = 18,                         \
//     .miso_pin = 10,                         \
//     .sclk_pin = 9,                          \
//     .bus_name = "spi0",                     \
// }

static struct ab32_soft_spi_config soft_spi_config[] ={
#ifdef BSP_USING_SOFT_SPI1
    SOFT_SPI1_BUS_CONFIG,
#endif
};

static struct ab32_soft_spi soft_spi_bus_obj[sizeof(soft_spi_config) / sizeof(soft_spi_config[0])] = {0};

static rt_err_t ab32_spi_init(struct ab32_soft_spi *spi_drv, struct rt_spi_configuration *cfg){
    RT_ASSERT(spi_drv != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

	//mode = master
    if (cfg->mode & RT_SPI_SLAVE){
		return RT_EIO;
    }
	else
		spi_drv->mode = RT_SPI_MASTER;

    if (cfg->mode & RT_SPI_3WIRE){
        return RT_EIO;
    }

    if (cfg->data_width == 8 || cfg->data_width == 16)
		spi_drv->data_width = cfg->data_width;
	else{
        return RT_EIO;
    }


    if (cfg->mode & RT_SPI_CPHA){
		spi_drv->cpha = 1;
    }
    else{
        spi_drv->cpha = 0;
    }

    if (cfg->mode & RT_SPI_CPOL){
		spi_drv->cpol = 1;
    }
    else{
        spi_drv->cpol = 0;
    }

    if (cfg->mode & RT_SPI_NO_CS){
    }
    else{
    }

    if (cfg->max_hz >= 1200000){
		spi_drv->spi_delay = 0;
    }else if (cfg->max_hz >= 1000000){
        spi_drv->spi_delay = 8;
    }else if (cfg->max_hz >= 830000){
        spi_drv->spi_delay = 16;
    }
    else {
        spi_drv->spi_delay = 24;
    }

    LOG_D("SPI limiting freq: %d, BaudRatePrescaler: %d",
          cfg->max_hz,
          spi_drv->max_hz);

    if (cfg->mode & RT_SPI_MSB){
		spi_drv->msb = 1;
    }
    else{
        spi_drv->msb = 0;
    }

    rt_pin_mode(spi_drv->config->mosi_pin,PIN_MODE_OUTPUT_OD);
    rt_pin_write(spi_drv->config->mosi_pin,PIN_HIGH);
    rt_pin_mode(spi_drv->config->miso_pin,PIN_MODE_INPUT_PULLDOWN);

    rt_pin_mode(spi_drv->config->sclk_pin,PIN_MODE_OUTPUT_OD);
	if(spi_drv->cpol)
	    rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
	else
	    rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
    LOG_D("%s init done", spi_drv->config->bus_name);
    return RT_EOK;
}

static inline void spi_delay(rt_uint32_t us){
    //  if(us==0)return;
    //延时非US级别,而是1个tick的周期,传入0,spi实测频率为330hz,超级慢
    rt_thread_mdelay(us);
}

static rt_uint32_t soft_spi_read_write_bytes(struct ab32_soft_spi *spi_drv, rt_uint8_t* send_buff, rt_uint8_t* recv_buff, rt_uint32_t len){
	rt_uint8_t dataIndex = 0;
	rt_uint8_t time = 1;
	for(rt_uint32_t i = 0; i<len; i++){
		if(spi_drv->cpha){	//CPHA=1
            if(rt_pin_read(spi_drv->config->sclk_pin))
            {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
            }
            else {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
            }
		}
		if(spi_drv->data_width == 16)
			time = 2;
		do{
			for(rt_uint8_t j = 0; j < 8; j++){
				if ((send_buff[dataIndex] & 0x80) != 0){
				    rt_pin_write(spi_drv->config->mosi_pin,PIN_HIGH);
				}else{
				    rt_pin_write(spi_drv->config->mosi_pin,PIN_LOW);
				}
				send_buff[dataIndex] <<= 1;
				spi_delay(spi_drv->spi_delay);
                if(rt_pin_read(spi_drv->config->sclk_pin))
                {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
                }
                else {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                }

				recv_buff[dataIndex] <<= 1;
				if (rt_pin_read(spi_drv->config->miso_pin))
					recv_buff[dataIndex] |= 0x01;
				spi_delay(spi_drv->spi_delay);
				if(time != 0 || j != 7){
                    if(rt_pin_read(spi_drv->config->sclk_pin))
                    {
                        rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
                    }
                    else {
                        rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                    }
				}
			}
			dataIndex++;
		}while((--time)==1);
		time = 1;
		spi_delay(spi_drv->spi_delay);
	}
    return len;
}

static rt_uint32_t soft_spi_read_bytes(struct ab32_soft_spi *spi_drv, rt_uint8_t* recv_buff, rt_uint32_t len){
	rt_uint8_t send_buff = spi_drv->dummy_data;
	rt_uint32_t dataIndex = 0;
	rt_uint8_t time = 1;
		if(spi_drv->cpha){	//CPHA=1
            if(rt_pin_read(spi_drv->config->sclk_pin))
            {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
            }
            else {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
            }
		}
	for(rt_uint32_t i = 0; i<len; i++){

		if(spi_drv->data_width == 16)
			time = 2;
		do{
			for(rt_uint8_t j = 0; j < 8; j++){
				if ((send_buff & 0x80) != 0){
					rt_pin_write(spi_drv->config->mosi_pin,PIN_HIGH);
				}else{
					rt_pin_write(spi_drv->config->mosi_pin,PIN_LOW);
				}
				send_buff <<= 1;
				spi_delay(spi_drv->spi_delay);
                if(rt_pin_read(spi_drv->config->sclk_pin))
                {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
                }
                else {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                }

                *recv_buff <<= 1;
				if (rt_pin_read(spi_drv->config->miso_pin))
				{
					*recv_buff |= 0x01;
				}
				else
				{
					*recv_buff &= 0xfe;
				}
				spi_delay(spi_drv->spi_delay);
				if(time != 0 || j != 7){
                    if(rt_pin_read(spi_drv->config->sclk_pin))
                    {
                        rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
                    }
                    else {
                        rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                    }
				}
			}
			recv_buff ++;
			dataIndex++;
		}while((--time)==1);
		time = 1;
		spi_delay(spi_drv->spi_delay);
		LOG_D("DONE ONE BYTE %d",dataIndex);
		LOG_D("%d",spi_drv->spi_delay);
	}
    return len;
}


static rt_uint32_t soft_spi_write_bytes(struct ab32_soft_spi *spi_drv, rt_uint8_t* send_buff, rt_uint32_t len){
	rt_uint8_t recv_buff = 0;
	rt_uint32_t dataIndex = 0;
	rt_uint8_t time = 1;
	LOG_D("%x",send_buff[0]);
	if(spi_drv->cpha){	//CPHA=1
            if(rt_pin_read(spi_drv->config->sclk_pin))
            {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
            }
            else {
                rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
            }
		}
	for(uint32_t i = 0; i<len; i++){

		if(spi_drv->data_width == 16)
			time = 2;
		do{
			for(rt_uint8_t j = 0; j < 8; j++){
				if ((send_buff[dataIndex] & 0x80) != 0){
					rt_pin_write(spi_drv->config->mosi_pin,PIN_HIGH);
					LOG_D("PIN_HIGH");
				}else{
					rt_pin_write(spi_drv->config->mosi_pin,PIN_LOW);
					LOG_D("PIN_LOW");
				}
				send_buff[dataIndex] <<= 1;
				spi_delay(spi_drv->spi_delay);
                if(rt_pin_read(spi_drv->config->sclk_pin))
                {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
                }
                else {
                    rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                }

				recv_buff <<= 1;
				if (rt_pin_read(spi_drv->config->miso_pin))
					recv_buff |= 0x01;
				spi_delay(spi_drv->spi_delay);
				if(time != 0 || j != 7){
				    if(rt_pin_read(spi_drv->config->sclk_pin))
				    {
				        rt_pin_write(spi_drv->config->sclk_pin,PIN_LOW);
				    }
				    else {
				        rt_pin_write(spi_drv->config->sclk_pin,PIN_HIGH);
                    }
				}
			}
			dataIndex++;
		}while((--time)==1);
		time = 1;
		spi_delay(spi_drv->spi_delay);
	}
    return len;
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message){
    rt_uint32_t state;
    rt_size_t message_length;
    rt_uint8_t *recv_buf;
    const rt_uint8_t *send_buf;
    rt_uint8_t pin = rt_pin_get("PE.6");

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    struct ab32_soft_spi *spi_drv =  rt_container_of(device->bus, struct ab32_soft_spi, spi_bus);
    struct ab32_soft_spi_pin *cs = device->parent.user_data;

    if (message->cs_take){
        rt_pin_write(cs->GPIO_Pin,PIN_LOW);
    }

    LOG_D("%s transfer prepare and start", spi_drv->config->bus_name);
    LOG_D("%s sendbuf: %02x, recvbuf: %02x, length: %d",
          spi_drv->config->bus_name,
          (message->send_buf),
          ((rt_uint8_t *)(message->recv_buf)), message->length);

    message_length = message->length;
    recv_buf = message->recv_buf;
    send_buf = message->send_buf;
    if(message_length){
        if (message->send_buf && message->recv_buf){
            state = soft_spi_read_write_bytes(spi_drv, (rt_uint8_t *)send_buf, (rt_uint8_t *)recv_buf, message_length);
            LOG_D("soft_spi_read_write_bytes");
        }
        else if (message->send_buf){
            state = soft_spi_write_bytes(spi_drv, (rt_uint8_t *)send_buf, message_length);
            LOG_D("soft_spi_write_bytes");
        }
        else{
            memset((rt_uint8_t *)recv_buf, 0x00, message_length);
            state = soft_spi_read_bytes(spi_drv, (rt_uint8_t *)recv_buf, message_length);
            LOG_D("soft_spi_read_bytes");
        }

        if (state != message_length){
            LOG_I("spi transfer error : %d", state);
            message->length = 0;
        }
        else{
            LOG_D("%s transfer done", spi_drv->config->bus_name);
        }
    }

    if (message->cs_release){
        rt_pin_write(cs->GPIO_Pin,PIN_HIGH);
    }

    return message->length;
}

static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration){
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(configuration != RT_NULL);

    struct ab32_soft_spi *spi_drv =  rt_container_of(device->bus, struct ab32_soft_spi, spi_bus);
    spi_drv->cfg = configuration;

    return ab32_spi_init(spi_drv, configuration);
}

static const struct rt_spi_ops ab32_spi_ops ={
    .configure = spi_configure,
    .xfer = spixfer,
};

static int rt_soft_spi_bus_init(void){
    rt_err_t result;
    for (int i = 0; i < sizeof(soft_spi_config) / sizeof(soft_spi_config[0]); i++){
        soft_spi_bus_obj[i].config = &soft_spi_config[i];
        soft_spi_bus_obj[i].spi_bus.parent.user_data = &soft_spi_config[i];
        result = rt_spi_bus_register(&soft_spi_bus_obj[i].spi_bus, soft_spi_config[i].bus_name, &ab32_spi_ops);
        RT_ASSERT(result == RT_EOK);

        LOG_D("%s bus init done", soft_spi_config[i].bus_name);
    }

    return result;
}

/**
  * Attach the spi device to SPI bus, this function must be used after initialization.
  */
rt_err_t rt_soft_spi_device_attach(const char *bus_name, const char *device_name, hal_sfr_t cs_gpiox, rt_uint8_t cs_gpio_pin){
    RT_ASSERT(bus_name != RT_NULL);
    RT_ASSERT(device_name != RT_NULL);

    rt_err_t result;
    struct rt_spi_device *spi_device;
    struct ab32_soft_spi_pin *cs_pin;

    /* attach the device to spi bus*/
    spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);
    cs_pin = (struct ab32_soft_spi_pin *)rt_malloc(sizeof(struct ab32_soft_spi_pin));
    RT_ASSERT(cs_pin != RT_NULL);
    cs_pin->GPIOx = cs_gpiox;
    cs_pin->GPIO_Pin = cs_gpio_pin;
    rt_pin_mode(cs_pin->GPIO_Pin, PIN_MODE_OUTPUT);
    result = rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin);

    if (result != RT_EOK){
        LOG_E("%s attach to %s faild, %d\n", device_name, bus_name, result);
    }

    RT_ASSERT(result == RT_EOK);

    LOG_D("%s attach to %s done", device_name, bus_name);

    return result;
}

int rt_soft_spi_init(void){
    return rt_soft_spi_bus_init();
}
INIT_BOARD_EXPORT(rt_soft_spi_init);

#endif
#endif /* RT_USING_SPI */
