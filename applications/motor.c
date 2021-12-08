/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include "common.h"

#define MOTOR_PIN           rt_pin_get("PE.4")
#define PWM_DEV_NAME        "lpwm0"
#define PWM_DEV_CHANNEL     0

struct rt_device_pwm *motor_pwm_dev;      /* PWM设备句柄 */
struct rt_pwm_configuration cfg = {0};

rt_uint32_t period = 10000000;   /* 周期为10ms，单位为纳秒ns */
rt_uint32_t pulse = 0;           /* PWM脉冲宽度值，单位为纳秒ns */

void set_motor_value(rt_uint32_t pwm_value)
{

    rt_uint32_t t = 0;

    pulse = pwm_value;

    /* 计算舵机目标位置所需要的高电平脉冲宽度 */
    t = 2000000/180*pulse;
    /* pwm设定的占空比为低电平宽度，舵机控制使用高电平宽度控制旋转角度 */
    pulse = period-500000-t;
    /* 设置PWM周期和脉冲宽度 */
    rt_pwm_set(motor_pwm_dev, PWM_DEV_CHANNEL, period, pulse);
}

void get_motor_value(int pwm_value)
{
    if(rt_pwm_get(motor_pwm_dev, &cfg) != 0){
        rt_kprintf("Get info of motor pwm_value error.\n");
        pwm_value = -1;
    }else{
        pwm_value = (int)(((double)(cfg.pulse)/(cfg.period)) * 100);
        rt_kprintf("pulse:%ld period:%ld duty_cycle:%d\n", cfg.pulse, cfg.period, pwm_value);
    }
}

static int ab32_motor_init(void)
{
    /* 设置LED引脚脚模式为输出 */
    rt_pin_mode(MOTOR_PIN, PIN_MODE_OUTPUT);
    /* 拉高LED引脚 */
    rt_pin_write(MOTOR_PIN, PIN_HIGH);

    /* 查找设备 */
    motor_pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (motor_pwm_dev == RT_NULL)
    {
        rt_kprintf("motor control run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(motor_pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* 使能设备 */
    rt_pwm_enable(motor_pwm_dev, PWM_DEV_CHANNEL);
}

/* 导出到 msh 命令列表中 */
// MSH_CMD_EXPORT(sg, sg sample <sg_angle:0~180>);
