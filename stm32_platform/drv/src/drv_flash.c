/**
  ******************************************************************************
  * @file    drv_flash.c
  * @author  gray
  * @version V1.0.0
  * @date    10-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_flash.h"



/**
  * @brief  drv_flash_unlock
  * @note   None
  * @param  
  * @retval None
 */
void drv_flash_unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}


/**
  * @brief  drv_flash_lock
  * @note   None
  * @param  
  * @retval None
 */
void drv_flash_lock(void)
{
    FLASH->CR |= ((uint32_t)0x01)<<7;
}


/**
  * @brief  drv_get_flash_status
  * @note   None
  * @param  
  * @retval None
 */
flash_err_t drv_get_flash_status(void)
{
    flash_err_t flash_status = FLASH_OK;
    uint32_t status = 0;
    status = FLASH->SR ;
    if((status & ((uint32_t)0x01 <<0)) != 0)
    {
        flash_status = FLASH_ERR_BUSY;
    }
    else if((status & ((uint32_t)0x01 <<2)) != 0)
    {
        flash_status = FLASH_ERR_PG;
    }
    else if((status & ((uint32_t)0x01 <<4)) != 0)
    {
        flash_status = FLASH_ERR_WRPRT;
    }
    else
    {
        flash_status = FLASH_OK;
    }
    return flash_status;
}


/**
  * @brief  drv_flash_wait_done
  * @note   None
  * @param  
  * @retval None
 */
flash_err_t drv_flash_wait_done(uint16_t time)
{
    flash_err_t status ;
    do
    {
        status = drv_get_flash_status();
        if(status != FLASH_ERR_BUSY)
        {
            break;
        }
        drv_delay_us(1);
        time--;  
    }while(time);
    if(time == 0)
    {
        status = FLASH_ERR_TIMEOUT;
    }
    return status;
    
}


/**
  * @brief  drv_flash_erase_page
  * @note   None
  * @param  
  * @retval None
 */
flash_err_t drv_flash_erase_page(uint32_t page_addr)
{
    flash_err_t flash_status;
    if((FLASH->CR & (((uint32_t)0x01)<<7)) != 0)
    {
        drv_flash_unlock();
    }
    flash_status = drv_flash_wait_done(0x5fff);
    if(flash_status == FLASH_OK)
    {
        FLASH->CR |= (((uint32_t)0x01) << 1);
        FLASH->AR = page_addr;
        FLASH->CR |= (((uint32_t)0x01) << 6);
        flash_status = drv_flash_wait_done(0x5fff);
        if(flash_status != FLASH_ERR_BUSY)
        {
            FLASH->CR &= ~(((uint32_t)0x01) << 1);
        }
    }
    return flash_status;
}



flash_err_t drv_flash_erase_chip(void)
{
    flash_err_t flash_status;
    if((FLASH->CR & (((uint32_t)0x01)<<7)) != 0)
    {
        drv_flash_unlock();
    }
    flash_status = drv_flash_wait_done(0x5fff);
    if(flash_status == FLASH_OK)
    {
        FLASH->CR |= (((uint32_t)0x01) << 2);
        FLASH->CR |= (((uint32_t)0x01) << 6);
        flash_status = drv_flash_wait_done(0xffff);
        if(flash_status != FLASH_ERR_BUSY)
        {
            FLASH->CR &= ~(((uint32_t)0x01) << 2);
        }
    }
    return flash_status;
}


/**
  * @brief  drv_flash_write_half_word
  * @note   None
  * @param  page_addr % 2 shall  be 0
  * @retval None
 */
flash_err_t drv_flash_write_half_word(uint32_t write_addr,uint16_t data)
{
    flash_err_t flash_status;
    if((write_addr%2) != 0)
    {
        flash_status = FLASH_ERR_ADRR;
        return flash_status;
    }
    if((FLASH->CR & (((uint32_t)0x01)<<7)) != 0)
    {
        drv_flash_unlock();
    }
    flash_status = drv_flash_wait_done(0xff);
    if(flash_status == FLASH_OK)
    {
        FLASH->CR |= (((uint32_t)0x01) << 0);
        *((uint16_t *)write_addr) = data;
        flash_status = drv_flash_wait_done(0xff);
        if(flash_status != FLASH_ERR_BUSY)
        {
            FLASH->CR &= ~(((uint32_t)0x01) << 0);
        }
    }
    return flash_status;
}


/**
  * @brief  drv_flash_read_half_word
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_flash_read_half_word(uint32_t read_addr)
{
    return *((uint16_t*)read_addr);
}


/**
  * @brief  drv_flash_write_no_check
  * @note   None
  * @param  
  * @retval None
 */
void drv_flash_write_no_check(uint32_t write_addr,uint16_t *pbuff, uint16_t len)
{
    uint16_t i;
    for(i=0; i<len; i++)
    {
        drv_flash_write_half_word(write_addr,pbuff[i]);
        write_addr += 2;
    }
}


