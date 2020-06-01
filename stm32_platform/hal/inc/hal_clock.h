/**
  ******************************************************************************
  * @file    hal_clock.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_CLOCK_H_
#define _HAL_CLOCK_H_
#include "drv_clock.h"
extern void  hal_get_clock_freq_info(sys_clk_freq_info_t *sys_clk_freq_info) ;
extern void hal_clock_init(void);
#endif

