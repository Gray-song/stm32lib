/**
  ******************************************************************************
  * @file    drv_iwdg.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_IWDG_H_
#define _DRV_IWDG_H_
#include "stm32f103rc.h"
#define IWDG_Prescaler_4            ((uint8_t)0x00)
#define IWDG_Prescaler_8            ((uint8_t)0x01)
#define IWDG_Prescaler_16           ((uint8_t)0x02)
#define IWDG_Prescaler_32           ((uint8_t)0x03)
#define IWDG_Prescaler_64           ((uint8_t)0x04)
#define IWDG_Prescaler_128          ((uint8_t)0x05)
#define IWDG_Prescaler_256          ((uint8_t)0x06)

#define KR_KEY_WriteAccess_Enable     ((uint16_t)0x5555)
#define KR_KEY_WriteAccess_Disable    ((uint16_t)0x0000)

#define KR_KEY_Reload    ((uint16_t)0xAAAA)
#define KR_KEY_Enable    ((uint16_t)0xCCCC)

extern void drv_iwdg_init(uint16_t ms);
extern void drv_iwdg_feed(void);
#endif 
