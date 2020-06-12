/**
  ******************************************************************************
  * @file    drv_flash.h
  * @author  gray
  * @version V1.0.0
  * @date    10-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  * chip in flash
  ******************************************************************************
*/
#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_
#include "stm32f103rc.h"
#include "drv_systick.h"
#define FLASH_KEY1  0x45670123
#define FLASH_KEY2  0xCDEF89AB
#define FLASH_BASE_ADDR 0x08000000
#define FLASH_USER_BASE_ADDR 0x08000000 //should be > 0x08000000
#define FLASH_SIZE 256
#define FLASH_SECTOR_SIZE  2048
typedef enum 
{
    FLASH_OK  = 0,
    FLASH_ERR_BUSY,
    FLASH_ERR_PG ,
    FLASH_ERR_WRPRT,
    FLASH_ERR_TIMEOUT,
    FLASH_ERR_ADRR

}flash_err_t;

extern void drv_flash_unlock(void);
extern void drv_flash_lock(void);
extern flash_err_t drv_get_flash_status(void);
extern flash_err_t drv_flash_wait_done(uint16_t time);
extern flash_err_t drv_flash_erase_page(uint32_t uint32_t);
extern flash_err_t drv_flash_erase_chip(void);
extern uint16_t drv_flash_read_half_word(uint32_t read_addr);
extern flash_err_t drv_flash_write_half_word(uint32_t write_addr,uint16_t data);
extern void drv_flash_write_no_check(uint32_t write_addr,uint16_t *pbuff, uint16_t len);

#endif
