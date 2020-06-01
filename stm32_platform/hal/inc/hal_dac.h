/**
  ******************************************************************************
  * @file    hal_dac.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_DAC_H_
#define _HAL_DAC_H_
#include "drv_dac.h"

extern void hal_dac_init(void);
extern void hal_set_vol_for_12b(uint16_t vol);
extern void hal_set_vol_for_8b(uint16_t vol);
#endif
