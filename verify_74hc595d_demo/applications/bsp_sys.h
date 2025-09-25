/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-09-23     18452       the first version
 */
#ifndef APPLICATIONS_BSP_SYS_H_
#define APPLICATIONS_BSP_SYS_H_

#include <rtthread.h>
#include <drv_common.h>
#include <board.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtdbg.h>

#include "main.h"
#include "74hc595d_driver.h"



// 以下为移植时必须需要的结构体等的初始化---------------------------------------------------------------------------------------------------------
typedef struct {
    rt_uint16_t  kprintf_cnt;                    // 用于打印序列
    rt_uint32_t  ulog_cnt;                       // ulog打印序列
    //------------------------------------------------------------

}RecordStruct;
extern RecordStruct Record;

#endif /* APPLICATIONS_BSP_SYS_H_ */
