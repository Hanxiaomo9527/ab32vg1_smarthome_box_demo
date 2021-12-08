#ifndef COMMON_H__
#define COMMON_H__

#include <rtconfig.h>
#include <rtthread.h>
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 0

#define FIRMWARE_VERSION 1.0.0

typedef void * ab32_handle_t;

typedef int (*ab32_fp_reboot_hook_t)(ab32_handle_t handle);
typedef int (*ab32_fp_notify_wifi_status_hook_t)(ab32_handle_t handle);
typedef int (*ab32_fp_notify_sd_filesize_hook_t)(ab32_handle_t handle);

typedef struct{
   ab32_fp_reboot_hook_t  reboot;
   ab32_fp_notify_wifi_status_hook_t  notify_wifi_status;
   ab32_fp_notify_sd_filesize_hook_t  notify_sd_filesize;
}ab32_hooks_t;


void ab32_audio_init(void);  // 播放音频
void ab32_button_init(void); // 按键处理
void ab32_dht11_init(void);  // 温湿度接收
void ab32_led_init(void);    // 调节灯光
void ab32_oled_init(void);   // 对用户的页面展示
void ab32_sdcard_init();     // sd卡 存储歌曲和温湿度记录与excal
void ab32_uart_init(void);   // 8266 通讯 cmd 网络状态监听

#endif

#ifdef __cplusplus
}
#endif

int ab32_uart_init(int argc, char *argv[]);   // 8266 通讯 cmd 网络状态监听

#endif /* COMMON_H__ */
