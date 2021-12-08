/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-27     Sam Xie       the first version
 */
#ifndef APPLICATIONS_DEVICE_INCLUDE_UART_H_
#define APPLICATIONS_DEVICE_INCLUDE_UART_H_

enum{
    MCU2WIFI = 0,
    WIFI2MCU
}UART_PRINT_FLAG;


enum {
    DEVICE_LED_RED      = 0x1,
    DEVICE_LED_YELLOW   = 0x2,
    DEVICE_LED_BLUE     = 0x3,
    DEVICE_MOTOR_OPEN   = 0x4,
    DEVICE_MOTOR_CLOSE  = 0x5
}DEVICE_SEND_CMDDATA;

#endif /* APPLICATIONS_DEVICE_INCLUDE_UART_H_ */
