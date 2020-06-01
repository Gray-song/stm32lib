/**
  ******************************************************************************
  * @file    drv_systick.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_SYSTICK_H_
#define _DRV_SYSTICK_H_
#include "stm32f103rc.h"

#define SYSTICK_CLK_SOURCE_HCLK_DIV8    ((uint32_t)0xFFFFFFFB)
#define SYSTICK_CLK_SOURCE_HCLK         ((uint32_t)0x00000004)


extern void drv_systick_clk_source_config(uint32_t systick_clk_source);
extern void drv_delay_ms(uint16_t ms);
extern void drv_delay_us(uint32_t us);
extern void drv_delay_init(uint32_t sys_clk_freq);


#endif
