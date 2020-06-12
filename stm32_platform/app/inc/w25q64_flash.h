/**
  ******************************************************************************
  * @file    flash.h
  * @author  gray
  * @version V1.0.0
  * @date    8-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _FLASH_H_
#define _FLASH_H_
#include "hal_spi.h"
#define W25Q64_READ_CMD                 0X03
#define W25Q64_READ_SR_CMD		        0x05 
#define W25Q64_WRITE_ENABLE_CMD		    0x06
#define W25Q64_WRITE_DISABLE_CMD		0x04
#define W25Q64_READ_ID_CMD		        0x90
#define W25Q64_ERASE_CHIP_CMD			0xC7 
#define W25Q64_ERASE_SECTOR_CMD		    0x20 
#define W25Q64_POWER_DOWN_CMD			0xB9
#define W25Q64_RELEASE_POWER_DOWN_CMD	0xAB 
#define W25Q64_WRITE_PAGE_CMD		    0x02
extern uint16_t g_w25q64_flash_id;
extern void w25q64_flash_read(uint8_t* pBuffer,uint32_t read_addr,uint16_t len);
extern void w25q64_flash_enable(void);
extern void w25q64_flash_disable(void);
extern uint8_t get_w25q64_flash_sr(void);
extern uint16_t w25q64_flash_read_id(void);
extern void w25q64_flash_write_disable(void);
extern void w25q64_flash_write_enable(void);
extern void w25q64_flash_init(void);
extern void w25q64_flash_erase_chip(void);
extern void w25q64_flash_wait_busy(void);
extern void w25q64_flash_erase_sector(uint16_t sector_id);
extern void w25q64_flash_power_down(void) ;
extern void w25q64_flash_wake_up(void);
extern void w25q64_flash_Write(uint8_t* pBuffer,uint32_t write_addr,uint16_t len);
#endif 




