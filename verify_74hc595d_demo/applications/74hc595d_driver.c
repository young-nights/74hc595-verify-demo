/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-09-23     18452       the first version
 */
#include "74hc595d_driver.h"


#define HC595_SER_PIN    GET_PIN(B, 0)
#define HC595_SRCLK_PIN  GET_PIN(B, 1)
#define HC595_RCLK_PIN   GET_PIN(B, 2)

#define HC595_NUM        3  // 三片级联

static rt_uint8_t _74hc595_buffer[HC595_NUM] = {0};  // 缓存，led[0] 是最远端



/* 私有：输出一个字节到 595 */
static void _74hc595_send_byte(rt_uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        rt_pin_write(HC595_SER_PIN, (data & 0x80) ? PIN_HIGH : PIN_LOW);
        rt_pin_write(HC595_SRCLK_PIN, PIN_LOW);
        rt_thread_mdelay(1);  // 简单延时，确保时序
        rt_pin_write(HC595_SRCLK_PIN, PIN_HIGH);
        data <<= 1;
    }
}


/* 刷新所有 595 输出 */
void hc595_update(void)
{
    rt_pin_write(HC595_RCLK_PIN, PIN_LOW);
    for (int i = HC595_NUM - 1; i >= 0; i--) {
        hc595_send_byte(hc595_buffer[i]);
    }
    rt_pin_write(HC595_RCLK_PIN, PIN_HIGH);  // 锁存
}

/* 控制某一颗 LED（0~23） */
void hc595_write(uint8_t led_idx, uint8_t on)
{
    if (led_idx >= HC595_NUM * 8) return;
    uint8_t byte_idx = led_idx / 8;
    uint8_t bit_mask = 1 << (led_idx % 8);

    if (on)
        hc595_buffer[HC595_NUM - 1 - byte_idx] |= bit_mask;
    else
        hc595_buffer[HC595_NUM - 1 - byte_idx] &= ~bit_mask;

    hc595_update();
}

/* 初始化引脚 */
void hc595_init(void)
{
    rt_pin_mode(HC595_SER_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HC595_SRCLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HC595_RCLK_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(HC595_SER_PIN, PIN_LOW);
    rt_pin_write(HC595_SRCLK_PIN, PIN_LOW);
    rt_pin_write(HC595_RCLK_PIN, PIN_LOW);
}

