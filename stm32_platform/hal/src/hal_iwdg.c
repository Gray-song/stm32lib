/**
  ******************************************************************************
  * @file    hal_iwdg.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   This file provides all the RTC firmware functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_iwdg.h"

/**
  * @brief  hal_watchdog_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_watchdog_init(uint16_t time_out_ms)
{
    drv_iwdg_init(time_out_ms);
}


/**
  * @brief  hal_watchdog_feed
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_watchdog_feed(void)
{
    drv_iwdg_feed();
}

  
