/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-27     Sam Xie       the first version
 */
#include "./include/common.h"
#include "./include/uart.h"

#define  LOG_TAG  "uart"
#include <ulog.h>

#define UART1_NAME "uart1"
#define UART_BUFFER_LENGTH (800)

#define require(x,laber)             do{if(!(x)) goto laber;} while(0)
#define require_noerr( x, laber )    do{if( (x)==-1 || (x)==0 ) goto laber;} while(0)

static rt_thread_t uart_thread = RT_NULL;
static struct rt_semaphore receive_sem;
static rt_device_t serial_dev;

static void print_uart_data(uint8_t *buffer,int buflen,int UART_PRINT_FLAG){
    if(UART_PRINT_FLAG)
        rt_kprintf("\r\nWIFI--->MCU [");
    else
        rt_kprintf("\r\nMCU--->WIFI [");

    for(int i = 0; i < buflen; i++){
        rt_kprintf("%02X ", buffer[i]);
    }
    rt_kprintf("]\r\n");
}



static void parse_uart_recvdata(uint8_t* data, int datalen)
{
    switch (data[1]) {
        case DEVICE_LED_RED:
            rt_kprintf("set led red!\r\n");
            break;
        case DEVICE_LED_YELLOW:
            rt_kprintf("set led yellow!\r\n");
            break;
        case DEVICE_LED_BLUE:
            rt_kprintf("set led blue!\r\n");
            break;
        case DEVICE_MOTOR_OPEN:
            rt_kprintf("set motor open!\r\n");
            break;
        case DEVICE_MOTOR_CLOSE:
            rt_kprintf("set motor close!\r\n");
            break;
        default:
            break;
    }
}

/*
 * AA cmd len data1 data2 sum    (len:6)
 */
int uart_get_one_packet( uint8_t* inBuf, int inBufLen )
{
    int ret = -1;
    while(1)
    {
        inBufLen = rt_device_read(serial_dev, -1, inBuf, 6);
        if(inBufLen != 6){
            rt_sem_take(&receive_sem, RT_WAITING_FOREVER);
            return ret;
        }else {
            return inBufLen;
        }
    }
}


static void serial_thread_entry(void *parameter)
{
    int recvlen = 0;
    uint8_t *inDataBuffer = RT_NULL;
    inDataBuffer = malloc(UART_BUFFER_LENGTH);
    require(inDataBuffer,exit);

    memset(inDataBuffer, 0 ,UART_BUFFER_LENGTH);
    while(1)
    {
        recvlen = uart_get_one_packet(inDataBuffer, UART_BUFFER_LENGTH);
        if(recvlen <= 0){
            msleep(100);
            continue;
        }

        print_uart_data(inDataBuffer, 6, WIFI2MCU);
        parse_uart_recvdata(inDataBuffer, 6);
        memset(inDataBuffer, 0 ,UART_BUFFER_LENGTH);
    }

exit:
    if(RT_NULL != inDataBuffer){
        free(inDataBuffer);
        inDataBuffer = NULL;
    }
    rt_thread_delete(uart_thread);
    uart_thread = RT_NULL;
}

static rt_err_t uart_receive(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&receive_sem);

    return RT_EOK;
}

/**
 * @brief  8266 通讯 cmd 网络状态监听
 *
 * @return
 */
int ab32_uart_init(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];

    if(argc == 2){
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }else{
        rt_strncpy(uart_name, UART1_NAME, RT_NAME_MAX);
    }

    serial_dev = rt_device_find(uart_name);
    if(!serial_dev){
        rt_kprintf("find %s failed!\n",uart_name);
        return RT_ERROR;
    }

    rt_sem_init(&receive_sem, "recevice_sem", 0, RT_IPC_FLAG_FIFO);

    rt_device_open(serial_dev, RT_DEVICE_FLAG_INT_RX);

    rt_device_set_rx_indicate(serial_dev, uart_receive);

    uart_thread = rt_thread_create("uart", serial_thread_entry, RT_NULL, 1024, 25, 10);

    if(uart_thread != RT_NULL){
        rt_kprintf("creat uart thread successful\n");
        rt_thread_startup(uart_thread);
    }else{
        rt_kprintf("creat uart thread failed\n");
        ret = RT_ERROR;
    }

    return ret;

}

// MSH_CMD_EXPORT(ab32_uart_init,uart1 init);



