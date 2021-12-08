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
<<<<<<< HEAD
    rt_uint32_t t = 0;
=======

    rt_uint32_t t = 0;

>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
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

<<<<<<< HEAD
int ab32_motor_init(void)
{
    rt_kprintf("motor init!!\r\n");

    rt_pin_mode(MOTOR_PIN, PIN_MODE_OUTPUT); // 设置LED引脚脚模式为输出
    rt_pin_write(MOTOR_PIN, PIN_HIGH);    // 拉高LED引脚

    motor_pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (motor_pwm_dev == RT_NULL){
=======
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
>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
        rt_kprintf("motor control run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

<<<<<<< HEAD
    rt_pwm_set(motor_pwm_dev, PWM_DEV_CHANNEL, period, pulse);// 设置PWM周期和脉冲宽度默认值
    rt_pwm_enable(motor_pwm_dev, PWM_DEV_CHANNEL);
}

/* 导出到 msh 命令列表中 */
// MSH_CMD_EXPORT(sg, sg sample <sg_angle:0~180>);



#if 0
#include "task.h"

#define THREAD_PRIORITY         7
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        3

#define PWM_DEV_NAME_R        "lpwm1"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL_R     3       /* PWM通道 */

#define PWM_DEV_NAME_G        "lpwm0"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL_G     1       /* PWM通道 */

#define PWM_DEV_NAME_B        "lpwm2"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL_B     3       /* PWM通道 */

struct rt_device_pwm *pwm_dev_r;      /* PWM设备句柄 */
struct rt_device_pwm *pwm_dev_g;      /* PWM设备句柄 */
struct rt_device_pwm *pwm_dev_b;      /* PWM设备句柄 */

static rt_thread_t pwm_led_tid = RT_NULL;

/* 线程 pwm_led_thread_entry 的入口函数 */
/**
  * @brief  pwm_led_thread_entry
  * @param  parameter
  * @retval None
  */
static void pwm_led_thread_entry(void *parameter)
{
    rt_uint32_t period, pulse_r,pulse_g,pulse_b, dir_r,dir_g,dir_b;

    period = 655360;    /* 周期为0.5ms，单位为纳秒ns */
    dir_r = 1;           /* PWM脉冲宽度值的增减方向 */
    dir_g = 1;
    dir_b = 1;
    pulse_r = 0;          /* PWM脉冲宽度值，单位为纳秒ns */
    pulse_g = 0;
    pulse_b = 0;

    rt_uint16_t r,g,b;

    /* 查找设备 */
    pwm_dev_r = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME_R);
    if (pwm_dev_r == RT_NULL)
    {
        rt_kprintf("pwm led r run failed! can't find %s device!\n", PWM_DEV_NAME_G);
    }

    pwm_dev_g = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME_G);
    if (pwm_dev_g == RT_NULL)
    {
        rt_kprintf("pwm led g run failed! can't find %s device!\n", PWM_DEV_NAME_G);
    }

    pwm_dev_b = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME_B);
    if (pwm_dev_b == RT_NULL)
    {
        rt_kprintf("pwm led b run failed! can't find %s device!\n", PWM_DEV_NAME_B);
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev_r, PWM_DEV_CHANNEL_R, period, pulse_r);
    rt_pwm_set(pwm_dev_g, PWM_DEV_CHANNEL_G, period, pulse_g);
    rt_pwm_set(pwm_dev_b, PWM_DEV_CHANNEL_B, period, pulse_b);

    /* 使能设备 */
    rt_pwm_enable(pwm_dev_r, PWM_DEV_CHANNEL_R);
    rt_pwm_enable(pwm_dev_g, PWM_DEV_CHANNEL_G);
    rt_pwm_enable(pwm_dev_b, PWM_DEV_CHANNEL_B);

    while (1)
    {
        for (r =0 ; r < 8; r++)
        {
            if (dir_r)
            {
                pulse_r += 81920;      /* 从0值开始每次增加81920ns */
            }
            else
            {
                pulse_r -= 81920;      /* 从最大值开始每次减少81920ns */
            }
            if ((pulse_r) >= period)
            {
                dir_r = 0;
            }
            if (81920 > pulse_r)
            {
                dir_r = 1;
            }
            /* 设置PWM周期和脉冲宽度 */
            rt_pwm_set(pwm_dev_r, PWM_DEV_CHANNEL_R, period, pulse_r);
            for(g = 0; g < 8; g++)
            {
                if (dir_g)
                {
                    pulse_g += 81920;      /* 从0值开始每次增加81920ns */
                }
                else
                {
                    pulse_g -= 81920;      /* 从最大值开始每次减少81920ns */
                }
                if ((pulse_g) >= period)
                {
                    dir_g = 0;
                }
                if (81920 > pulse_g)
                {
                    dir_g = 1;
                }
                rt_pwm_set(pwm_dev_g, PWM_DEV_CHANNEL_G, period, pulse_g);
                for(b = 0; b < 8; b++)
                {
                    rt_thread_mdelay(10);
                    if (dir_b)
                    {
                        pulse_b += 81920;      /* 从0值开始每次增加81920ns */
                    }
                    else
                    {
                        pulse_b -= 81920;      /* 从0值开始每次减少81920ns */
                    }
                    if ((pulse_b) >= period)
                    {
                        dir_b = 0;
                    }
                    if (81920 > pulse_b)
                    {
                        dir_b = 1;
                    }

                    rt_pwm_set(pwm_dev_b, PWM_DEV_CHANNEL_B, period, pulse_b);
                }
            }

        }
    }
}

/* 线程初始化*/
int thread_init(void)
{
    /* 创建线程，名称是 pwm_led_thread，入口是 pwm_led_thread*/
    pwm_led_tid = rt_thread_create("pwm_led_thread",
                                   pwm_led_thread_entry,
                                   RT_NULL,
                                   THREAD_STACK_SIZE,
                                   THREAD_PRIORITY,
                                   THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (pwm_led_tid != RT_NULL)
        rt_thread_startup(pwm_led_tid);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_init, thread init);
#endif
=======
    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(motor_pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* 使能设备 */
    rt_pwm_enable(motor_pwm_dev, PWM_DEV_CHANNEL);
}

/* 导出到 msh 命令列表中 */
// MSH_CMD_EXPORT(sg, sg sample <sg_angle:0~180>);
>>>>>>> cdbb41c1cd7e7d3fe4ab6f5f45403ec79b6c01c4
