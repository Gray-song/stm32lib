/**
  ******************************************************************************
  * @file    hal_iwdg.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_IWDG_H_
#define _HAL_IWDG_H_
#include "drv_iwdg.h"

extern void hal_watchdog_init(uint16_t time_out_ms);
extern void hal_watchdog_feed(void);
#endif
