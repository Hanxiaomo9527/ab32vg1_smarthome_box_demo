/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-12-8      Sam XIE       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "common.h"

#define LED_OFF 0
#define LED_ON  1

static rt_thread_t event_manager_thread = RT_NULL;
struct rt_event control_event;


static void control_event_init(void)
{
    rt_kprintf("control event init!\r\n");
    rt_event_init(&control_event, "control_event", RT_IPC_FLAG_FIFO);
}

//INIT_APP_EXPORT(control_event_init);


/* 事件处理线程 */
static void event_manager_thread_entry(void *parameter)
{
    while(1)
    {
        if(rt_event_recv(&control_event,
                       (MOTOR_OPEN_FLAG | MOTOR_CLOSE_FLAG | LED_RED_FLAG | LED_GREEN_FLAG | LED_BLUE_FLAG | LED_CLOSE_FLAG),
                       RT_EVENT_FLAG_OR,
                       RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {

            if(rt_event_recv(&control_event, MOTOR_OPEN_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_motor_value(100);
                // oled show
            }

            if(rt_event_recv(&control_event, MOTOR_CLOSE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_motor_value(0);
                // oled show
            }

            if(rt_event_recv(&control_event,LED_RED_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_led_red(LED_ON);
                // oled show
            }

            if (rt_event_recv(&control_event, LED_GREEN_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_led_green(LED_ON);
                // oled show
            }

            if (rt_event_recv(&control_event, LED_BLUE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                set_led_blue(LED_ON);
                // oled show
            }

            if (rt_event_recv(&control_event, LED_CLOSE_FLAG, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, RT_NULL) == RT_EOK)
            {
                led_write(LED_OFF, LED_OFF, LED_OFF);
                // oled show
            }
        }
    }
}

int ab32_event_manager_init(void)
{
    control_event_init();
    event_manager_thread = rt_thread_create("event_manager_thread", event_manager_thread_entry, RT_NULL, 1024*2, 26, 10);
    if (event_manager_thread != RT_NULL){
        rt_thread_startup(event_manager_thread);
    }else{
        rt_kprintf("creat event manager failed!\r\n");
    }

    return RT_EOK;
}
INIT_APP_EXPORT(ab32_event_manager_init);

#if 0
void load_menu(int8_t* state_addr)
{
    rt_uint32_t e;
    OLED_ShowString(30,0,"Loading...", 16);
    OLED_DrawBMP(0,2,128,8,loading_bmp); //RT-Thread图标

    rt_thread_delay(2000);

    //切换到主菜单
    func_index = table[func_index].sub_base_addr + table[func_index].sub_offset_addr;
    table[func_index].enter_operation(&table[func_index].sub_offset_addr);
}

void main_menu(int8_t* state_addr)
{
    int8_t i=20;
    OLED_Clear();

    if(*state_addr < 0)  *state_addr = 1;
    if(*state_addr >= 2)    *state_addr = 0;

    switch(*state_addr)
    {
        case 0:
            OLED_ShowCHinese(i,0,0);   //音
            OLED_ShowCHinese(i+18,0,1);//乐
            OLED_ShowCHinese(i+36,0,2);//播
            OLED_ShowCHinese(i+54,0,3);//放
            OLED_ShowCHinese(i+72,0,4);//器
            OLED_DrawBMP(40,2,40+48,8,music_bmp);//音乐图标

            break;

        case 1:
            OLED_ShowCHinese(46,0,5);//设
            OLED_ShowCHinese(64,0,6);//置
            OLED_DrawBMP(40,2,40+48,8,setting_bmp);//设置图标
            break;

        default:
            break;
    }
}


DIR *dirp = RT_NULL;
struct dirent *d = RT_NULL;
void playlists(int8_t* state_addr)
{
    int8_t   i = 0;
    uint8_t  j = 0;
    uint8_t  k = 2;

    if(*state_addr < 0)  *state_addr = song_info.songs_num-1; //第一首往上按到达末尾
    if(*state_addr >= song_info.songs_num)  *state_addr = 0;  //最后一首往下按到第一首

    OLED_Clear();
    OLED_ShowString(30,0,"PlayList", 16);

    //更新歌曲显示
    if(*state_addr%3 == 0)
    {
        dirp = opendir("/music"); // 打开 /music目录
        if (dirp == RT_NULL)
        {
            rt_kprintf("open directory error!\n");
        }
        else
        {
            i = *state_addr/3;
            while(i--)
            {
                for(j=0; j<3; ++j) //过三首
                {
                    d = readdir(dirp);
                }
            }

            for(j=0; j<3; ++j) //获取三首歌曲的名字
            {
                d = readdir(dirp);
                rt_strncpy(song_info.music_table[j],d->d_name,20);
            }
        }

        closedir(dirp); //关闭目录
    }


    for(i=0; i<=2; ++i)
    {
        OLED_ShowString(18,k,(uint8_t*)song_info.music_table[i], 16);
        k+=2;
    }

    //移动指示光标
    OLED_ShowString(0,2+(*state_addr%3)*2,"->", 16);
}


void settings_list(int8_t* state_addr)
{
    int8_t i=0,k=2;
    if(*state_addr < 0)  *state_addr = 1;
    if(*state_addr >= 3)  *state_addr = 0;
    OLED_Clear();
    OLED_ShowString(10,0,"Settings List", 16);
    OLED_ShowString(18,k,"volume", 16);
    OLED_ShowString(18,k+2,"language", 16);
    OLED_ShowString(18,k+4,"brightness", 16);

    //移动指示光标
    OLED_ShowString(0,2+(*state_addr%3)*2,"->", 16);
}

char  current_music_file_path[30];
void music_play(int8_t* state_addr)
{
    static int8_t pre_play_state = 0xFF; //默认该值不可能达到

    if(pre_play_state == *state_addr) //重复点击
    {
        pre_play_state = 0XFF;
        wavplayer_pause(); //使用wavplayer_play 会重新播放
    }
    else
    {
        pre_play_state = *state_addr; //记录当前播放位置

        rt_memcpy(current_music_file_path, "music/", 6);
        rt_strncpy(&current_music_file_path[6],song_info.music_table[*state_addr%3],20);
        rt_kprintf("play : %s\n",current_music_file_path);

        wavplayer_play(current_music_file_path); //播放
        rt_kprintf("volume = %d\n",wavplayer_volume_get());
    }

    //返回上一个状态
    func_index = table[func_index].back;
    table[func_index].enter_operation(&table[func_index].sub_offset_addr);
}


void volume_control(int8_t* state_addr)
{
    if(*state_addr > 9) *state_addr = 9;
    if(*state_addr < 0)   *state_addr = 0;

    wavplayer_volume_set(*state_addr*5);
    rt_kprintf("volume = %d\n",wavplayer_volume_get());
    OLED_Clear();
    OLED_ShowNum(30,0,*state_addr*10,2,16);
}

void language_setting(int8_t* state_addr)
{
    OLED_Clear();
    OLED_ShowString(0,2,"sorry", 16);
    OLED_ShowString(0,4,"you not is vip", 16);
    rt_thread_delay(2000);

    //返回上一个状态
    func_index = table[func_index].back;
    table[func_index].enter_operation(&table[func_index].sub_offset_addr);
}


void brightness_setting(int8_t* state_addr)
{
    OLED_Clear();
    OLED_ShowString(0,2,"sorry", 16);
    OLED_ShowString(0,4,"you not is vip", 16);
    rt_thread_delay(2000);

    //返回上一个状态
    func_index = table[func_index].back;
    table[func_index].enter_operation(&table[func_index].sub_offset_addr);
}

//获取歌曲数量
uint8_t songs_num_get(void)
{
    int num = 0;
    rt_uint32_t level;
    rt_thread_delay(1000);

    dirp = opendir("/music"); // 打开 /music目录
    while (dirp == RT_NULL)
    {
        dirp = opendir("/music"); // 打开 /music目录
        rt_kprintf("songs_num_get:open directory error!\n");
        rt_thread_delay(1000);
    }

    // 读取目录
    while ((d = readdir(dirp)) != RT_NULL)
    {
        num++;
    }

    return num;
}
#endif
