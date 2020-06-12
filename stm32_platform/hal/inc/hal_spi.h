/**
  ******************************************************************************
  * @file    hal_spi.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_
#include "drv_spi.h"
#include "drv_gpio.h"

extern void hal_spi_init(void);
extern void hal_set_spi_clk_div(spi_baudrate_prescaler_t spi_baudrate_prescaler);
extern  uint8_t hal_spi1_read_write_byte(uint8_t data);
#endif 
