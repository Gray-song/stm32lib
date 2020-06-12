/**
  ******************************************************************************
  * @file    hal_flash.h
  * @author  gray
  * @version V1.0.0
  * @date    10-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  * chip in flash
  ******************************************************************************
*/
#ifndef _HAL_FLASH_H_
#define _HAL_FLASH_H_
#include "drv_flash.h"
extern void hal_flash_write(uint32_t write_addr,uint16_t *pbuff,uint16_t len);
extern void hal_flash_read(uint32_t read_addr,uint16_t *pbuff,uint16_t len);
#endif






