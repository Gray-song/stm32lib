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
#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_
#include "stm32f103rc.h"
typedef enum 
{
    SPI_1 = 0 ,
    SPI_2,
    SPI_3,

}spi_channel_t;

typedef enum
{
    SPI_DIR_2LINES_FULl_DUPLEX = 0x0000,
    SPI_DIR_2LINES_RX = 0x0400,
    SPI_DIR_1LINE_RX = 0x8000,
    SPI_DIR_1LINE_TX = 0xC000,
}spi_direction_t;

typedef enum
{
    SPI_SLAVE_MODE = 0x0000,
    SPI_MASTER_MODE = 0x0104,
    
    
}spi_mode_t;

typedef enum
{
    SPI_DATA_8BIT = 0x0000,
    SPI_DATA_16BIT = 0x0800
    
}spi_data_len_t;

typedef enum
{
    SPI_CPOL_LOW = 0x0000,
    SPI_CPOL_HIGH = 0x0002
}spi_cpol_t;

typedef enum
{
    SPI_CPHA_1EDGE = 0x0000,
    SPI_CPHA_2EDGE = 0x0001
}spi_cpha_t;

typedef enum
{
    SPI_NSS_HARD = 0x0000,
    SPI_NSS_SOFT = 0x0200,
    
}spi_nss_t;

typedef enum
{
    SPI_BaudRate_Prescaler_2 = 0x0000,
    SPI_BaudRate_Prescaler_4 = 0x0008,
    SPI_BaudRate_Prescaler_8 = 0x0010,
    SPI_BaudRate_Prescaler_16 = 0x0018,
    SPI_BaudRate_Prescaler_32 = 0x0020,
    SPI_BaudRate_Prescaler_64 = 0x0028,
    SPI_BaudRate_Prescaler_128 = 0x0030,
    SPI_BaudRate_Prescaler_256 =0x0038,
    
}spi_baudrate_prescaler_t;

typedef enum
{
    SPI_FirstBit_MSB = 0x0000,
    SPI_FirstBit_LSB = 0x0080

    
}spi_first_bit_t;

typedef enum
{
    SPI_HARD_CRC_ENABLE = 0x2000,
    SPI_HARD_CRC_DISABLE = 0x0000

    
}spi_hard_crc_t;
typedef struct 
{
    spi_direction_t spi_direction;
    spi_mode_t spi_mode;
    spi_data_len_t spi_data_len; 
    spi_cpol_t spi_cpol;
    spi_cpha_t spi_cpha;
    spi_nss_t spi_nss;
    spi_baudrate_prescaler_t spi_baudrate_prescaler;
    spi_first_bit_t spi_first_bit; 
    spi_hard_crc_t spi_hard_crc ;
}spi_init_t;



typedef enum
{   
 
    SPI_RECEIVE_INTERRPUT = 0 ,
    SPI_SEND_INTERRPUT ,
    
    
}spi_it_source_t;
    
typedef enum
{
    SPI_CRC_RX = 0,
    SPI_CRC_TX,
}spi_crc_t;
extern void drv_spi_init(spi_channel_t channel,spi_init_t spi_init_strust);
extern void drv_spi_cmd(spi_channel_t channel,bool status);
extern void drv_spi_it_config(spi_channel_t channel,spi_it_source_t it_source ,bool status);
extern void drv_spi_isr_config(spi_channel_t channel,voidfunc_t p_callback,spi_it_source_t callback_type);
extern void drv_set_crc_poly(spi_channel_t channel,uint16_t poly);
extern void drv_set_spi_clk_div(spi_channel_t channel,spi_baudrate_prescaler_t spi_baudrate_prescaler);
extern void drv_set_nss_soft_ssi(spi_channel_t channel,bool status);
extern void drv_send_crc(spi_channel_t channel);
extern uint16_t drv_get_crc(spi_channel_t channel,spi_crc_t spi_crc);
extern uint16_t drv_get_crc_poly(spi_channel_t channel);
extern uint16_t drv_spi_receive_16b(spi_channel_t channel);
extern uint8_t drv_spi_receive_8b(spi_channel_t channel);
extern uint16_t drv_get_spi_sr_status(spi_channel_t channel);
extern void drv_spi_send_8b(spi_channel_t channel,uint8_t data);
extern void drv_spi_send_16b(spi_channel_t channel,uint16_t data);

#endif
