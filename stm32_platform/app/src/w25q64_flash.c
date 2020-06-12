/**
  ******************************************************************************
  * @file    flash.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "headers.h"
static void w25q64_flash_write_page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t len);
static void w25q64_flash_write_no_check(uint8_t* pBuffer,uint32_t wrtie_addr,uint16_t len);
static uint8_t s_flash_buff[4096];
uint16_t g_w25q64_flash_id = 0;

/**
  * @brief  w25q64_flash_init
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_init(void)
{
	drv_gpio_write_bit(GPIO_A,GPIO_Pin_2,TRUE);
    drv_gpio_write_bit(GPIO_A,GPIO_Pin_5,TRUE);
    drv_gpio_write_bit(GPIO_A,GPIO_Pin_6,TRUE);
    drv_gpio_write_bit(GPIO_A,GPIO_Pin_7,TRUE);
	hal_set_spi_clk_div(SPI_BaudRate_Prescaler_4);	
    drv_spi_cmd(SPI_1,ENABLE_DEF);
	g_w25q64_flash_id=w25q64_flash_read_id();
}
/**
  * @brief  w25q64_flash_enable
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_enable(void)
{
    hal_do_write_signal(FLASH_CS,FALSE);
}



/**
  * @brief  w25q64_flash_disable
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_disable(void)
{
    hal_do_write_signal(FLASH_CS,TRUE);
}


/**
  * @brief  w25q64_flash_read
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_read(uint8_t* pBuffer,uint32_t read_addr,uint16_t len)   
{ 
 	    	
    uint16_t i = 0 ;  
	w25q64_flash_enable();  
    hal_watchdog_feed();  
    
    hal_spi1_read_write_byte(W25Q64_READ_CMD);
    hal_spi1_read_write_byte(((uint8_t)((read_addr)>>16)));
    hal_spi1_read_write_byte(((uint8_t)((read_addr)>>8)));
    hal_spi1_read_write_byte(((uint8_t)(read_addr)));
    
    for(i=0;i<len;i++)
	{ 
        pBuffer[i]=hal_spi1_read_write_byte(0xff);  
    }         
     
    
	w25q64_flash_disable();                              	      
}


/**
  * @brief  get_w25q64_flash_sr
  * @note   None
  * @param  
  * @retval None
 */
uint8_t get_w25q64_flash_sr(void)   
{  
	  
    uint8_t flash_sr;
	w25q64_flash_enable(); 
    hal_spi1_read_write_byte(W25Q64_READ_SR_CMD);
    flash_sr = hal_spi1_read_write_byte(0xff);    
	       
	             
	w25q64_flash_disable();                               
	return flash_sr;   
} 


/**
  * @brief  w25q64_flash_write_enable
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_write_enable(void)   
{
    
	w25q64_flash_enable(); 
    hal_spi1_read_write_byte(W25Q64_WRITE_ENABLE_CMD) ;              
	w25q64_flash_disable();                                	      
} 


/**
  * @brief  w25q64_flash_write_disable
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_write_disable(void)   
{  
   
	w25q64_flash_enable();       
    hal_spi1_read_write_byte(W25Q64_WRITE_DISABLE_CMD) ;          
	w25q64_flash_disable();                               	      
}


/**
  * @brief  w25q64_flash_read_id
  * @note   None
  * @param  
  * @retval None
 */
uint16_t w25q64_flash_read_id(void)
{
	uint16_t w25q64_flash_id = 0;	    
	w25q64_flash_enable(); 	 
    hal_spi1_read_write_byte(W25Q64_READ_ID_CMD);    
	hal_spi1_read_write_byte(0x00); 
    hal_spi1_read_write_byte(0x00); 
    hal_spi1_read_write_byte(0x00);     
	w25q64_flash_id |= hal_spi1_read_write_byte(0xff)<<8;  
	w25q64_flash_id |= hal_spi1_read_write_byte(0xff);	 
	w25q64_flash_disable(); 			    
	return w25q64_flash_id;
}   


/**
  * @brief  w25q64_flash_erase_chip
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_erase_chip(void)   
{       
    w25q64_flash_write_enable();                  
    w25q64_flash_wait_busy();   
  	w25q64_flash_enable();
    hal_spi1_read_write_byte(W25Q64_ERASE_CHIP_CMD);            
	w25q64_flash_disable();                               	      
	w25q64_flash_wait_busy();   				   
} 


/**
  * @brief  w25q64_flash_wait_busy
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_wait_busy(void)   
{   
	while ((get_w25q64_flash_sr()&0x01)==0x01);   
}  


/**
  * @brief  w25q64_flash_erase_sector
  * @note   None
  * @param  sector_id 0~2047
  * @retval None
 */
void w25q64_flash_erase_sector(uint16_t sector_id)   
{   
    
    hal_watchdog_feed();
	sector_id *= 4096;
    w25q64_flash_write_enable();                  
    w25q64_flash_wait_busy();    
  	w25q64_flash_enable(); 
    hal_spi1_read_write_byte(W25Q64_ERASE_SECTOR_CMD); 
    hal_spi1_read_write_byte((uint8_t)((sector_id)>>16)) ;
    hal_spi1_read_write_byte((uint8_t)((sector_id)>>8));
    hal_spi1_read_write_byte((uint8_t)sector_id);
     
	w25q64_flash_disable();                             	      
    w25q64_flash_wait_busy();  
    hal_watchdog_feed();    
}  


/**
  * @brief  w25q64_flash_power_down
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_power_down(void)   
{ 
    
  	w25q64_flash_enable();       
    
    hal_spi1_read_write_byte(W25Q64_POWER_DOWN_CMD) ;   
            
	w25q64_flash_disable();                                	      
    hal_delay_us(3);                               
}   


/**
  * @brief  w25q64_flash_wake_up
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_wake_up(void)   
{  
    
  	w25q64_flash_enable();
    
    hal_spi1_read_write_byte(W25Q64_RELEASE_POWER_DOWN_CMD);    
       
	w25q64_flash_disable();                            	      
    hal_delay_us(3);                               
} 


/**
  * @brief  w25q64_flash_write_page
  * @note   None
  * @param  len 0~256,len is not beyong remain bytes of each page.(max bytes of each page is 256)
  * @retval None
 */
static void w25q64_flash_write_page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t len)
{
 	uint16_t i;  
    
    w25q64_flash_write_enable();                   
	w25q64_flash_enable();
    hal_spi1_read_write_byte(W25Q64_WRITE_PAGE_CMD);
    hal_spi1_read_write_byte((uint8_t)((WriteAddr)>>16)) ;
    hal_spi1_read_write_byte((uint8_t)((WriteAddr)>>8));
    hal_spi1_read_write_byte((uint8_t)WriteAddr);    
         
    for(i=0;i<len;i++)
    {
        hal_spi1_read_write_byte(pBuffer[i]);
        
    } 
    
	w25q64_flash_disable();                            
	w25q64_flash_wait_busy();					   
} 


/**
  * @brief  w25q64_flash_write_no_check
  * @note   None
  * @param  
  * @retval None
 */
static void w25q64_flash_write_no_check(uint8_t* pBuffer,uint32_t wrtie_addr,uint16_t len)   
{ 			 		 
	uint16_t page_remain;	   
	page_remain=256-wrtie_addr%256; 	 	    
	if(len<=page_remain)page_remain=len;
	while(1)
	{	   
		w25q64_flash_write_page(pBuffer,wrtie_addr,page_remain);
		if(len==page_remain)
        {
            break;
        }
	 	else
		{
			pBuffer += page_remain;
			wrtie_addr += page_remain;	

			len -= page_remain;			  
			if(len>256)
            {
                page_remain = 256; 
            }
			else
            {
                page_remain = len; 
            }                	  
		}
	};	    
} 


/**
  * @brief  w25q64_flash_write_no_check
  * @note   None
  * @param  
  * @retval None
 */
void w25q64_flash_Write(uint8_t* pBuffer,uint32_t write_addr,uint16_t len)   
{ 
	uint32_t sector_pos;
	uint16_t sector_offset;
	uint16_t sector_remain;	   
 	uint16_t i;    

	sector_pos = write_addr / 4096;
	sector_offset = write_addr % 4096;
	sector_remain = 4096 - sector_offset;

	if(len <= sector_remain)
    {
        sector_remain = len;
    }
	while(1) 
	{	
        hal_watchdog_feed();
		w25q64_flash_read(s_flash_buff,sector_pos*4096,4096);
		for(i=0;i<sector_remain;i++)
		{
			if(s_flash_buff[sector_offset+i]!=0XFF)
            {
                break;	
            }  
		}
		if(i < sector_remain)
		{
			w25q64_flash_erase_sector(sector_pos);
			for(i=0; i<sector_remain; i++)	   
			{
				s_flash_buff[i+sector_offset] = pBuffer[i];	  
			}
			w25q64_flash_write_no_check(s_flash_buff,sector_pos*4096,4096);

		}
        else
        {
            w25q64_flash_write_no_check(pBuffer,write_addr,sector_remain);
        }            				   
		if(len == sector_remain)
        {
            break;
        }
		else
		{
			sector_pos++;
			sector_offset = 0;

		   	pBuffer += sector_remain;  
			write_addr += sector_remain;
		   	len -= sector_remain;				
			if(len>4096)
            {
                sector_remain=4096;
            }	
			else
            {
                sector_remain=len;
            }                			
		}	 
	}	 	 
}
