/**
  ******************************************************************************
  * @file    hal_flash.c
  * @author  gray
  * @version V1.0.0
  * @date    10-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  * chip in flash
  ******************************************************************************
*/
#include "hal_flash.h"
static uint16_t s_flash_buff[1024];//2k bytes

/**
  * @brief  hal_flash_write
  * @note   None
  * @param  
  * @retval None
 */
void hal_flash_write(uint32_t write_addr,uint16_t *pbuff,uint16_t len)
{
    
    uint32_t sector_pos;
    uint16_t sector_offset;
    uint16_t sector_remain;
    uint16_t i = 0 ;
    uint32_t offset_addr;
    if((write_addr < FLASH_USER_BASE_ADDR) || ((write_addr+(len*2))>=(FLASH_BASE_ADDR+1024*FLASH_SIZE)))
    {
        
        return ;
    }
    drv_flash_unlock();
    offset_addr =  write_addr -  FLASH_BASE_ADDR; 
    sector_pos = offset_addr/FLASH_SECTOR_SIZE;
    sector_offset = (offset_addr %FLASH_SECTOR_SIZE)/2;
    sector_remain = (FLASH_SECTOR_SIZE/2) - sector_offset;
    if(len <= sector_remain)
    {
        sector_remain = len;
    }
    while(1)
    {
        hal_flash_read(sector_pos*FLASH_SECTOR_SIZE+FLASH_BASE_ADDR,s_flash_buff,FLASH_SECTOR_SIZE/2);
        for(i=0; i<sector_remain; i++)
        {
            if(s_flash_buff[sector_offset+i] != 0xffff)
            {
                break;
            }
        }
        if(i < sector_remain)
        {
            drv_flash_erase_page(sector_pos*FLASH_SECTOR_SIZE+FLASH_BASE_ADDR);
            for(i=0; i<sector_remain; i++)
            {
                s_flash_buff[sector_offset+i] = pbuff[i];
            }
            drv_flash_write_no_check(sector_pos*FLASH_SECTOR_SIZE+FLASH_BASE_ADDR,s_flash_buff,FLASH_SECTOR_SIZE/2);
        }
        else
        {
            drv_flash_write_no_check(write_addr,pbuff,sector_remain);
        }
        if(sector_remain == len)
        {
            break;
        }
        else
        {
            sector_pos++;
            sector_offset = 0;
            pbuff += sector_remain;
            write_addr += 2*sector_remain;//sssss
            len -= sector_remain;
            if(len >(FLASH_SECTOR_SIZE/2))
            {
                sector_remain = FLASH_SECTOR_SIZE/2;
            }
            else
            {
                sector_remain = len;
            }
            
        }
    }
    drv_flash_lock();
}


/**
  * @brief  hal_flash_read
  * @note   None
  * @param  
  * @retval None
 */
void hal_flash_read(uint32_t read_addr,uint16_t *pbuff,uint16_t len)
{
    uint16_t i =0;
    for(i=0; i<len; i++)
    {
        pbuff[i] = drv_flash_read_half_word(read_addr);
        read_addr += 2;
    }
}
