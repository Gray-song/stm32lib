/**
  ******************************************************************************
  * @file    hal_systick.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_SYSTICK_H_
#define _HAL_SYSTICK_H_
#include "drv_systick.h"

#define hal_delay_ms(ms) drv_delay_ms((uint16_t)ms)
#define hal_delay_us(us) drv_delay_us((uint32_t)us)
extern void hal_delay_init(uint32_t sys_clk_freq);

#endif
