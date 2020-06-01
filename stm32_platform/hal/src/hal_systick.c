/**
  ******************************************************************************
  * @file    hal_systick.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   This file provides all the RTC firmware functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_systick.h"

/**
  * @brief  hal_delay_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_delay_init(uint32_t sys_clk_freq)
{
    drv_delay_init(sys_clk_freq);
}
 



