/**
  ******************************************************************************
  * @file    drv_clock.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_spi.h"
static volatile SPI_TypeDef* const SPI_ARRAY[]=
{
    SPI1,
    SPI2,
    SPI3,
};
static voidfunc_t p_spi1_recv_interrupt = NULL;
static voidfunc_t p_spi1_send_interrupt = NULL;

static voidfunc_t p_spi2_recv_interrupt = NULL;
static voidfunc_t p_spi2_send_interrupt = NULL;

static voidfunc_t p_spi3_recv_interrupt = NULL;
static voidfunc_t p_spi3_send_interrupt = NULL;


/**
  * @brief  spi init 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_init(spi_channel_t channel,spi_init_t spi_init_strust)
{
    uint16_t tmpreg = 0;
    if(channel == SPI_1)
    {
        RCC->APB2ENR |= ((uint32_t)0x01)<<12;
    }
    else
    {
        RCC->APB1ENR |= ((uint32_t)0x01)<<(channel+13);
    }
    tmpreg = SPI_ARRAY[channel]->CR1;
  
  tmpreg &= 0x0000;
  
  tmpreg |= (uint16_t)(spi_init_strust.spi_direction | spi_init_strust.spi_mode |
                  spi_init_strust.spi_data_len | spi_init_strust.spi_cpol |  
                  spi_init_strust.spi_cpha | spi_init_strust.spi_nss |  
                  spi_init_strust.spi_baudrate_prescaler | spi_init_strust.spi_first_bit | spi_init_strust.spi_hard_crc);
  /* Write to SPIx CR1 */
  SPI_ARRAY[channel]->CR1 = tmpreg;
}


/**
  * @brief  drv_spi_cmd 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_cmd(spi_channel_t channel,bool status)
{
    if(status == ENABLE_DEF)
    {
        SPI_ARRAY[channel]->CR1 |= 0x0040;
    }
    else
    {
        SPI_ARRAY[channel]->CR1 &= ~(0x0040);
    }
}


/**
  * @brief  drv_spi_it_config 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_it_config(spi_channel_t channel,spi_it_source_t it_source ,bool status)
{
    if(status == ENABLE_DEF)
    {
        SPI_ARRAY[channel]->CR2 |= ((uint16_t)0x01)<<(6+it_source);
    }
    else
    {
        SPI_ARRAY[channel]->CR2 &= ~(((uint16_t)0x01)<<(6+it_source));
    }
}


/**
  * @brief  drv_spi_isr_config 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_isr_config(spi_channel_t channel,voidfunc_t p_callback,spi_it_source_t callback_type)
{
    if(channel == SPI_1)
    {
        
        if(callback_type == SPI_RECEIVE_INTERRPUT)
        {
            if(p_callback != NULL)
            {
                p_spi1_recv_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_spi1_send_interrupt = p_callback;
            }
        }
    }
    else if(channel == SPI_2)
    {
        
        if(callback_type == SPI_RECEIVE_INTERRPUT)
        {
            if(p_callback != NULL)
            {
                p_spi2_recv_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_spi2_send_interrupt = p_callback;
            }
        }
    }
    else
    {
        if(callback_type == SPI_RECEIVE_INTERRPUT)
        {
            if(p_callback != NULL)
            {
                p_spi3_recv_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_spi3_send_interrupt = p_callback;
            }
        }
    }
}


/**
  * @brief  drv_send_crc 
  * @note   None
  * @param  
  * @retval None
 */
void drv_send_crc(spi_channel_t channel)
{
    SPI_ARRAY[channel]->CR1 |= ((uint16_t)0x01 << 12);
}


/**
  * @brief  drv_set_crc_ploy 
  * @note   None
  * @param  
  * @retval None
 */
void drv_set_crc_poly(spi_channel_t channel,uint16_t poly)
{
    SPI_ARRAY[channel]->CRCPR &= (uint16_t)0x00;
    SPI_ARRAY[channel]->CRCPR |= poly;
}


/**
  * @brief  drv_get_crc 
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_get_crc(spi_channel_t channel,spi_crc_t spi_crc)
{
    uint16_t temp = 0;
    if(spi_crc == SPI_CRC_RX)
    {
        temp = SPI_ARRAY[channel]->RXCRCR;
    }
    else
    {
        temp = SPI_ARRAY[channel]->TXCRCR;
    }
    return temp;
}


uint16_t drv_get_crc_poly(spi_channel_t channel)
{
    return SPI_ARRAY[channel]->CRCPR;
}


/**
  * @brief  drv_set_spi_clk_div 
  * @note   None
  * @param  
  * @retval None
 */
void drv_set_spi_clk_div(spi_channel_t channel,spi_baudrate_prescaler_t spi_baudrate_prescaler)
{
    SPI_ARRAY[channel]->CR1 &= ~(0x0038);
    SPI_ARRAY[channel]->CR1 |= spi_baudrate_prescaler;
}


/**
  * @brief  drv_set_nss_soft_ssi 
  * @note   None
  * @param  
  * @retval None
 */
void drv_set_nss_soft_ssi(spi_channel_t channel,bool status)
{
    if(status == ENABLE_DEF)
    {
        SPI_ARRAY[channel]->CR1 |= 0x0100;
    }
    else
    {
        SPI_ARRAY[channel]->CR1 &= ~(0x0100);
    }
}


/**
  * @brief  drv_spi_sr_status 
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_get_spi_sr_status(spi_channel_t channel)
{
    return SPI_ARRAY[channel]->SR;
}


/**
  * @brief  drv_spi_receive_16b 
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_spi_receive_16b(spi_channel_t channel)
{
    return SPI_ARRAY[channel]->DR;
}


/**
  * @brief  drv_spi_receive_8b 
  * @note   None
  * @param  
  * @retval None
 */
uint8_t drv_spi_receive_8b(spi_channel_t channel)
{
    return (uint8_t)SPI_ARRAY[channel]->DR;
}


/**
  * @brief  drv_spi_send_8b 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_send_8b(spi_channel_t channel,uint8_t data)
{
    SPI_ARRAY[channel]->DR = data;
}


/**
  * @brief  drv_spi_send_16b 
  * @note   None
  * @param  
  * @retval None
 */
void drv_spi_send_16b(spi_channel_t channel,uint16_t data)
{
    SPI_ARRAY[channel]->DR = data;
}


/**
  * @brief  SPI1_IRQHandler 
  * @note   None
  * @param  
  * @retval None
 */
void SPI1_IRQHandler()
{
    if((SPI1->SR&((uint16_t)0x01)) != 0)
    {
        if(p_spi1_recv_interrupt != NULL)
        {
            p_spi1_recv_interrupt();
        }
    }
    
    if((SPI1->SR&((uint16_t)0x02)) != 0)
    {
        if(p_spi1_send_interrupt != NULL)
        {
            p_spi1_send_interrupt();
        }
    }
}


/**
  * @brief  SPI2_IRQHandler 
  * @note   None
  * @param  
  * @retval None
 */
void SPI2_IRQHandler()
{
    if((SPI2->SR&((uint16_t)0x01)) != 0)
    {
        if(p_spi2_recv_interrupt != NULL)
        {
            p_spi2_recv_interrupt();
        }
    }
    
    if((SPI2->SR&((uint16_t)0x02)) != 0)
    {
        if(p_spi2_send_interrupt != NULL)
        {
            p_spi2_send_interrupt();
        }
    }
}


/**
  * @brief  SPI3_IRQHandler 
  * @note   None
  * @param  
  * @retval None
 */
void SPI3_IRQHandler()
{
    if((SPI3->SR&((uint16_t)0x01)) != 0)
    {
        if(p_spi3_recv_interrupt != NULL)
        {
            p_spi3_recv_interrupt();
        }
    }
    
    if((SPI3->SR&((uint16_t)0x02)) != 0)
    {
        if(p_spi3_send_interrupt != NULL)
        {
            p_spi3_send_interrupt();
        }
    }
}
