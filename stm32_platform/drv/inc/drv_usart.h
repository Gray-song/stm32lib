/**
  ******************************************************************************
  * @file    drv_usart.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_USART_H_
#define _DRV_USART_H_

#include "stm32f103rc.h"
#include "drv_clock.h"
typedef enum 
{
    DATA_LEN_8BIT = 0x0000,
    DATA_LEN_9BIT = 0x1000
}usart_data_len_t;

typedef enum 
{
    STOP_BITS_1 = 0x0000,
    STOP_BITS_0_5 = 0x1000,
    STOP_BITS_2 = 0x2000,
    STOP_BITS_1_5 = 0x3000
}usart_stop_bits_t;

typedef enum
{
    USART_PARITY_NONE = 0x0000,
    USART_PARITY_EVENT = 0x0400,
    USART_PARITY_ODD = 0x0600
}usart_parity_t;

typedef enum
{
    HW_FLOW_CTRL_None = 0x0000,
    HW_FLOW_CTRL_RTS = 0x0100,
    HW_FLOW_CTRL_CTS = 0x0200,
    HW_FLOW_CTRL_RTS_CTS = 0x0300

}usart_hardware_flow_ctrl_t;

typedef enum
{
    USART_MODE_RX = 0x0004,
    USART_MODE_TX = 0x0008,
    USART_MODE_RX_TX = 0x000C,

}usart_mode_t;

typedef enum 
{
    USART_1 = 0,
    USART_2 ,
    USART_3 ,
    USART_4 ,
    USART_5 

}usart_ch_t;


typedef struct 
{
    uint32_t usart_baudrate;
    usart_data_len_t data_len;
    usart_stop_bits_t stop_bits;
    usart_parity_t parity;
    usart_hardware_flow_ctrl_t hardware_flow_ctrl;
    usart_mode_t mode;
   
}usart_init_t;

/*low 5 is position of IT_ENABLE_BIT in USARTx->CRn, */
/*middle 3 is which USARTx->CRn IT_ENABLE_BIT in, (USARTx->CR1,USARTx->CR2,USARTx->CR3)*/
/*high 4 is position of IT_STATUS_BIT in USARTx->SR, */
typedef enum
{
    USART_IT_PE = 0x0028,
    USART_IT_TXE = 0x0727,
    USART_IT_TC  = 0x0626,
    USART_IT_RXNE = 0x0525,
    USART_IT_IDLE = 0x0424,
    USART_IT_LBD = 0x0846,
    USART_IT_CTS = 0x096A,
    USART_IT_ERR = 0x0060,
    USART_IT_ORE = 0x0360,
    USART_IT_NE = 0x0260,
    USART_IT_FE = 0x0160

}usart_it_source_t;

typedef enum
{
    USART_RX_CALL_BAKE = 0,
	USART_TX_CALL_BAKE
}sci_callback_t;

extern void drv_usart_init(usart_ch_t usart_ch,const usart_init_t* usart_init_struct);
extern void drv_usart_cmd(usart_ch_t usart_ch,bool status);
extern void drv_usart_it_config(usart_ch_t usart_ch,usart_it_source_t it_source,bool status);
extern bool drv_get_usart_it_status(usart_ch_t usart_ch,usart_it_source_t it_source);
extern void drv_usart_send(usart_ch_t usart_ch,uint16_t data);
extern uint16_t drv_usart_receive(usart_ch_t usart_ch);
extern void drv_usart_isr_config(usart_ch_t usart_ch,voidfunc_t p_callback,sci_callback_t callback_type);
#endif
