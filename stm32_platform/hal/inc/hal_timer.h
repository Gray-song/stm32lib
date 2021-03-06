/**
  ******************************************************************************
  * @file    hal_timer.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_
#include "drv_timer.h"
#define hal_set_timer_cap_edge(timx,channel,edge) drv_set_timer_cap_edge(timx,channel,edge)
#define hal_get_timer_cap_value(timx,channel) drv_get_timer_cap_value(timx,channel)
extern void hal_timer_init(void);
extern void hal_timer_cap_init(void);
extern void hal_timer_pwm_init(void);
extern uint32_t hal_get_pwm_period(void);
extern uint32_t hal_get_high_time(void);
#endif
