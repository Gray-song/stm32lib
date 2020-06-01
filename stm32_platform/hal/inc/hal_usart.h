/**
  ******************************************************************************
  * @file    hal_usart.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_USART_H_
#define _HAL_USART_H_
#include "drv_usart.h"
#include "platform_config.h"

#define hal_disable_tx_interrupt(usart_ch) drv_usart_it_config(usart_ch,USART_IT_TC,DISABLE_DEF)
#define hal_enable_tx_interrupt(usart_ch) drv_usart_it_config(usart_ch,USART_IT_TC,ENABLE_DEF)
#define hal_disable_rx_interrupt(usart_ch) drv_usart_it_config(usart_ch,USART_IT_RXNE,DISABLE_DEF)
#define hal_enable_rx_interrupt(usart_ch) drv_usart_it_config(usart_ch,USART_IT_RXNE,ENABLE_DEF)

#define USART_DATA_BUFFER_SIZE  64
#define USART_ISR_CONFIG \
{hal_usart1_rx_isr,hal_usart1_tx_isr},\
{hal_usart2_rx_isr,hal_usart2_tx_isr},\
{hal_usart3_rx_isr,hal_usart3_tx_isr},\
{hal_usart4_rx_isr,hal_usart4_tx_isr},\
{hal_usart5_rx_isr,hal_usart5_tx_isr}


typedef struct
{
    voidfunc_t rx_isr;
    voidfunc_t tx_isr;
}usart_iqr_t;


typedef struct
{
   uint8_t  head;
   uint8_t  tail;
   uint8_t  buffer[USART_DATA_BUFFER_SIZE];  
} usart_queue_t;

extern void hal_usart_init(usart_ch_t usart_ch);
extern uint8_t hal_usart_recieve_length_get(usart_ch_t usart_ch);
extern bool hal_usart_send(usart_ch_t usart_ch, const uint8_t data[], uint8_t dt_len);
extern uint8_t hal_usart_read(usart_ch_t usart_ch, uint8_t buffer[], uint8_t buffer_length, uint8_t read_len);
#endif
