/**
  ******************************************************************************
  * @file    hal_spi.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_spi.h"

/**
  * @brief  hal_spi_init
  * @note   None
  * @param  
  * @retval None
 */
void hal_spi_init(void)
{
    spi_init_t spi_init_struct;
    spi_init_struct.spi_baudrate_prescaler = SPI_BaudRate_Prescaler_256;
    spi_init_struct.spi_cpha = SPI_CPHA_2EDGE;
    spi_init_struct.spi_cpol = SPI_CPOL_HIGH;
    spi_init_struct.spi_data_len = SPI_DATA_8BIT;
    spi_init_struct.spi_direction = SPI_DIR_2LINES_FULl_DUPLEX;
    spi_init_struct.spi_first_bit = SPI_FirstBit_MSB;
    spi_init_struct.spi_hard_crc = SPI_HARD_CRC_DISABLE;
    spi_init_struct.spi_mode = SPI_MASTER_MODE ;
    spi_init_struct.spi_nss = SPI_NSS_SOFT;
    drv_spi_init(SPI_1,spi_init_struct);
    drv_spi_it_config(SPI_1,SPI_RECEIVE_INTERRPUT,DISABLE_DEF);
    drv_spi_it_config(SPI_1,SPI_SEND_INTERRPUT,DISABLE_DEF);
    drv_set_nss_soft_ssi(SPI_1,ENABLE_DEF);
    drv_spi_cmd(SPI_1,ENABLE_DEF);
}

/**
  * @brief  hal_set_spi_clk_div
  * @note   None
  * @param  
  * @retval None
 */
void hal_set_spi_clk_div(spi_baudrate_prescaler_t spi_baudrate_prescaler)
{
    drv_set_spi_clk_div(SPI_1,spi_baudrate_prescaler);
}



uint8_t hal_spi1_read_write_byte(uint8_t data)
{		
	uint16_t retry=0;	
    uint16_t spi1_sr = 0;
    spi1_sr = drv_get_spi_sr_status(SPI_1);
	while((spi1_sr&(((uint16_t)0x01)<<1))==0)         
	{
        spi1_sr = drv_get_spi_sr_status(SPI_1);
		retry++;
		if(retry>0XFFFE)
        {
            return 0;
        }
	}			  
	//SPI1->DR=data;
    drv_spi_send_8b(SPI_1,data) ;   
	retry=0;
    spi1_sr = drv_get_spi_sr_status(SPI_1);
    while((spi1_sr&(((uint16_t)0x01)<<0))==0)        
	{
        spi1_sr = drv_get_spi_sr_status(SPI_1);
		retry++;
		if(retry>0XFFFE)
        {
            return 0;
        }
	}	  						    
	return drv_spi_receive_8b(SPI_1);    				    
}


