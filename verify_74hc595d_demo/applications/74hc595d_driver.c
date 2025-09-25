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


#define HC595_SER_PIN    GET_PIN(A, 5)
#define HC595_SCLK_PIN   GET_PIN(A, 6)
#define HC595_RCLK_PIN   GET_PIN(A, 7)

#define HC595_NUM        3  // 三片级联

#define LED_TOTAL        21 // 控制的灯的总数

/* 2. 统一合法性检查 */
static int led_valid(uint8_t idx)
{
    return idx < LED_TOTAL;
}

static rt_uint8_t _74hc595_buffer[HC595_NUM] = {0xFF,0xFF, 0xFF | 0xE0};  // 缓存，led[0] 是最远端


/* 私有：输出一个字节到 595 */
static void _74hc595_send_byte(rt_uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        rt_pin_write(HC595_SER_PIN, (data & 0x80) ? PIN_HIGH : PIN_LOW);
        rt_pin_write(HC595_SCLK_PIN, PIN_LOW);
        rt_thread_mdelay(1);  // 简单延时，确保时序
        rt_pin_write(HC595_SCLK_PIN, PIN_HIGH);
        data <<= 1;
    }
}

/* 刷新所有 595 输出 */
void _74hc595_update(void)
{
    rt_pin_write(HC595_RCLK_PIN, PIN_LOW);
    for (int i = HC595_NUM - 1; i >= 0; i--) {
        _74hc595_send_byte(_74hc595_buffer[i]);
    }
    rt_pin_write(HC595_RCLK_PIN, PIN_HIGH);  // 锁存
}


/* 全部熄灭 */
void _74hc595_clear_all(void)
{
    for (uint8_t i = 0; i < HC595_NUM; i++) {
        _74hc595_buffer[i] = 0xFF;
    }
    _74hc595_update();
}


/* 控制某一颗 LED（0~23） */
void _74hc595_set_single(uint8_t led_idx, uint8_t on)
{
    if (!led_valid(led_idx)) return;
    /* 再置低对应位 */
    uint8_t byte_idx = led_idx / 8;
    uint8_t bit_mask = 1 << (led_idx % 8);
    if (on)
        _74hc595_buffer[HC595_NUM - 1 - byte_idx] &= ~bit_mask;
    else
        _74hc595_buffer[HC595_NUM - 1 - byte_idx] |=  bit_mask;
    _74hc595_update();
}


/* 同时点亮多颗，旧灯自动熄灭 */
void _74hc595_set_multi(const uint8_t *idx_list, uint8_t cnt)
{
    /* 先全灭 */
    for (uint8_t i = 0; i < HC595_NUM; i++) {
        _74hc595_buffer[i] = 0xFF;
    }
    _74hc595_buffer[0] |= 0xE0;               // ★最远片 Q5~Q7 永远熄灭

    /* 再把列表里的位拉低 */
    for (uint8_t k = 0; k < cnt; k++) {
        uint8_t idx = idx_list[k];
        if (!led_valid(idx)) continue;
        uint8_t byte_idx = idx / 8;
        uint8_t bit_mask = 1 << (idx % 8);
        _74hc595_buffer[HC595_NUM - 1 - byte_idx] &= ~bit_mask;
    }
    _74hc595_update();
}

/* 初始化引脚 */
void _74hc595_init(void)
{
    rt_pin_mode(HC595_SER_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HC595_SCLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HC595_RCLK_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(HC595_SER_PIN, PIN_LOW);
    rt_pin_write(HC595_SCLK_PIN, PIN_LOW);
    rt_pin_write(HC595_RCLK_PIN, PIN_LOW);

    _74hc595_clear_all();
}

