#if 0
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "common.h"
#include "drv_soft_spi.h"

#define SPI_DEVICE_NAME     "spi10"
#define SPI_CS_PIN           rt_pin_get("PA.6")

struct rt_spi_device *spi_dev_oled;


rt_uint8_t oled_device_id[4] = {0x90,0x00,0x00,0x00};
rt_uint8_t id[5]={0};

static void w25q_erase(void)
{
    rt_uint8_t chip_erase[1] = {0x20};
    struct rt_spi_message msg;
    msg.send_buf = chip_erase;
    msg.recv_buf = RT_NULL;
    msg.length = 1;
    msg.next = RT_NULL;
    msg.cs_take = 0;
    msg.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg);
    rt_kprintf("erase success\r\n");
}

void w25q_sector_erase(rt_uint32_t SectorAddr)
{
    rt_uint8_t page_write_cmd[4];

    rt_uint8_t write_enable = 0x06;
    struct rt_spi_message msg3;
    msg3.send_buf = &write_enable;
    msg3.recv_buf = RT_NULL;
    msg3.length = 1;
    msg3.next = RT_NULL;
    msg3.cs_take = 1;
    msg3.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg3);

    page_write_cmd[0] = 0x20;
    page_write_cmd[1] = (SectorAddr & 0xFF0000) >> 16;
    page_write_cmd[2] = (SectorAddr & 0xFF00) >> 8;
    page_write_cmd[3] = SectorAddr & 0xFF;

    struct rt_spi_message msg,msg2;
    msg.send_buf = page_write_cmd;
    msg.recv_buf = RT_NULL;
    msg.length = 4;
    msg.next = RT_NULL;
    msg.cs_take = 1;
    msg.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg);
}

void w25q_page_write(rt_uint8_t* pBuffer, rt_uint32_t WriteAddr, rt_uint16_t NumByteToWrite)
{
    rt_uint8_t page_write_cmd[4];

    rt_uint8_t write_enable = 0x06;
    struct rt_spi_message msg3;
    msg3.send_buf = &write_enable;
    msg3.recv_buf = RT_NULL;
    msg3.length = 1;
    msg3.next = RT_NULL;
    msg3.cs_take = 1;
    msg3.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg3);

    page_write_cmd[0] = 0x02;
    page_write_cmd[1] = (WriteAddr & 0xFF0000) >> 16;
    page_write_cmd[2] = (WriteAddr & 0xFF00) >> 8;
    page_write_cmd[3] = WriteAddr & 0xFF;

    struct rt_spi_message msg,msg2;
    msg.send_buf = page_write_cmd;
    msg.recv_buf = RT_NULL;
    msg.length = 4;
    msg.next = RT_NULL;
    msg.cs_take = 1;
    msg.cs_release = 0;
    rt_spi_transfer_message(spi_dev_oled, &msg);


    msg2.send_buf = pBuffer;
    msg2.recv_buf = RT_NULL;
    msg2.length = NumByteToWrite;
    msg2.next = RT_NULL;
    msg2.cs_take = 0;
    msg2.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg2);

    rt_kprintf("write done\r\n");  
}

void w25q_buffer_read(rt_uint8_t* pBuffer, rt_uint32_t ReadAddr, rt_uint16_t NumByteToRead)
{
    rt_uint8_t buffer_read_cmd[4];

    rt_uint8_t write_enable = 0x06;
    struct rt_spi_message msg3;
    msg3.send_buf = &write_enable;
    msg3.recv_buf = RT_NULL;
    msg3.length = 1;
    msg3.next = RT_NULL;
    msg3.cs_take = 1;
    msg3.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg3);


    buffer_read_cmd[0] = 0x03;
    buffer_read_cmd[1] = (ReadAddr & 0xFF0000) >> 16;
    buffer_read_cmd[2] = (ReadAddr & 0xFF00) >> 8;
    buffer_read_cmd[3] = ReadAddr & 0xFF;

    struct rt_spi_message msg,msg2;
    msg.send_buf = buffer_read_cmd;
    msg.recv_buf = RT_NULL;
    msg.length = 4;
    msg.next = RT_NULL;
    msg.cs_take = 1;
    msg.cs_release = 0;
    rt_spi_transfer_message(spi_dev_oled, &msg);

    msg2.send_buf = RT_NULL;
    msg2.recv_buf = pBuffer;
    msg2.length = NumByteToRead;
    msg2.next = RT_NULL;
    msg2.cs_take = 0;
    msg2.cs_release = 1;
    rt_spi_transfer_message(spi_dev_oled, &msg2);
}


void ab32_spi_init(void)
{
    rt_soft_spi_device_attach("spi0","spi10",RT_NULL,SPI_CS_PIN);
    spi_dev_oled = (struct rt_spi_device *)rt_device_find(SPI_DEVICE_NAME);
    if(!spi_dev_oled){
        rt_kprintf("spi sample run failed\n");
    }else {

        struct rt_spi_configuration cfg;
        {
            cfg.data_width = 8;
            cfg.mode = RT_SPI_MASTER | RT_SPI_MSB | RT_SPI_MODE_0;
            cfg.max_hz = 1*1000; //1.92 M
        }
        rt_spi_configure(spi_dev_oled, &cfg);
    }
}

// MSH_CMD_EXPORT(spi_w25q_sample, spi w25q sample);
#endif
