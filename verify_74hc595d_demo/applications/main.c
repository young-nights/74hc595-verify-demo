/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-09-22     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "bsp_sys.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t pos = 0;
  int8_t  step = 1;          // 方向：+1 向右，-1 向左

  _74hc595_init();
//  _74hc595_set_single(5);             // 只亮第 5 颗
  _74hc595_update();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
      _74hc595_clear_all();          // 全部熄灭
      _74hc595_set_single(pos, 1);   // 只点亮当前一颗
      rt_thread_mdelay(150);

      pos += step;
      if (pos >= 20) { step = -1; pos = 20; }   // 到最右折返
      if (pos == 0)  { step = +1; }             // 到最左折返
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

