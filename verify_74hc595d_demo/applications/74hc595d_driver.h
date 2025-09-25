/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-09-23     18452       the first version
 */
#ifndef APPLICATIONS_74HC595D_DRIVER_H_
#define APPLICATIONS_74HC595D_DRIVER_H_

#include "bsp_sys.h"



void _74hc595_update(void);
void _74hc595_clear_all(void);
void _74hc595_set_single(uint8_t led_idx, uint8_t on);
void _74hc595_set_multi(const uint8_t *idx_list, uint8_t cnt);
void _74hc595_init(void);


#endif /* APPLICATIONS_74HC595D_DRIVER_H_ */
