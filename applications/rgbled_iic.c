/*
 * Copyright (c) 2020-2021, Bluetrum Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-8      Sam XIE      the first version
 *
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "rgbled_iic.h"
#include "stdlib.h"
#include "common.h"


/* use gpio */
static void P9813_PIN_write_frame(uint32_t data)
{ 
  uint8_t i;        
  uint32_t f_data = data;
  
  for(i=0; i<32; i++){
    rgbled_sclk_clr();
    if(f_data & 0x80000000){
      rgbled_sdin_set();
    }
    else{
        rgbled_sdin_clr();
    }
    rgbled_sclk_set();  // raise edge to set data
    f_data = f_data << 1;
  } 
}

static void P9813_PIN_write_start_frame(void)
{ 
  uint32_t start_frame = 0x00000000;
  P9813_PIN_write_frame(start_frame);     
}

static void P9813_PIN_write_data(uint8_t blue, uint8_t green, uint8_t red)
{
  uint8_t check_byte = 0xC0;  // starting flag "11"
  uint32_t send_data = 0;
  
  // calc check data
  check_byte |= (((~blue) >> 2) & 0x30);  // B7, B6
  check_byte |= (((~green) >> 4) & 0x0C);  // G7,G6
  check_byte |= (((~red) >> 6) & 0x03);   // R7,R6
  
  // create send data 32bit
  send_data |= (check_byte << 24) | (blue << 16) | (green << 8) | (red);
  
  //send_data = 0xFC0000FF;
  P9813_PIN_write_frame(send_data);
}
 
/*-------------------------------------------------- USER INTERFACES ------------------------------------------------*/

void ab32_rgb_led_init(void)
{
    rt_kprintf("rgb_led init!\r");
    rt_pin_mode(SDA_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(SCL_PIN, PIN_MODE_OUTPUT);
}

void rgb_led_open(uint8_t red, uint8_t green, uint8_t blue)
{
    P9813_PIN_write_start_frame();
    P9813_PIN_write_data(blue, green, red);
    P9813_PIN_write_start_frame();  // fix led bink bug
}

void ab32_rgb_led_close(void)
{
    ab32_rgb_led_init();
    rgb_led_open(0, 0, 0);
}
