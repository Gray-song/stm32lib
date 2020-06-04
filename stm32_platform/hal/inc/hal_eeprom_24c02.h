/**
  ******************************************************************************
  * @file    hal_eeprom_24c02.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  * you shall be init iic before you use the moudle.
  ******************************************************************************
*/
#ifndef _HAL_EEPROM_24C02_H_
#define _HAL_EEPROM_24C02_H_
#include "drv_simulation_iic.h"
#include "hal_iwdg.h"
#define EEPROM_24C02_ADDR 0xA0
//#define EACH_PAGE_MAX_BYTE_NUM 8
#define PAGE_MAX 256
#define W_CMD 0
#define R_CMD 1
extern bool hal_eeprom_24c02_write(const uint8_t p_data_buffer[], uint16_t phy_start_addr, uint16_t length);
extern bool hal_eeprom_24c02_erase(uint16_t phy_start_addr, uint16_t length);
extern bool hal_eeprom_24c02_read(uint8_t *p_data_buffer, uint16_t phy_start_addr, uint16_t length);
extern bool hal_eeprom_24c02_ck_empty(uint16_t phy_start_addr, uint16_t length);
extern bool hal_eeprom_24c02_ck_ok(void);
#endif
