/**
  ******************************************************************************
  * @file    hal.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/

#ifndef _HAL_H_
#define _HAL_H_
#include "hal_clock.h"
#include "hal_gpio.h"
#include "hal_systick.h"
#include "hal_nvic.h"
#include "hal_usart.h"
#include "hal_iwdg.h"
#include "hal_timer.h"
#include "hal_adc.h"
#include "hal_dac.h"
#include "hal_simulation_iic.h"
#include "hal_eeprom_24c02.h"
extern sys_clk_freq_info_t g_sys_clock_info;
extern void hal_init(void);
#endif

