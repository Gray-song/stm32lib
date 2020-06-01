/**
  ******************************************************************************
  * @file    drv_usart.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_usart.h"
static volatile USART_TypeDef* const USART_ARRAY[]=
{
    USART1 ,
    USART2 ,
    USART3 ,
    UART4 ,
    UART5 ,
};

static voidfunc_t p_usart1_recv_interrupt = NULL;
static voidfunc_t p_usart1_send_interrupt = NULL;
static voidfunc_t p_usart2_recv_interrupt = NULL;
static voidfunc_t p_usart2_send_interrupt = NULL;
static voidfunc_t p_usart3_recv_interrupt = NULL;
static voidfunc_t p_usart3_send_interrupt = NULL;
static voidfunc_t p_usart4_recv_interrupt = NULL;
static voidfunc_t p_usart4_send_interrupt = NULL;
static voidfunc_t p_usart5_recv_interrupt = NULL;
static voidfunc_t p_usart5_send_interrupt = NULL;


/**
  * @brief usart init 
  * @note   None
  * @param  
  * @retval None
 */
void drv_usart_init(usart_ch_t usart_ch,const usart_init_t* usart_init_struct)
{
    uint32_t t_usart_clk_freq = 0;
    uint16_t t_div_mantissa = 0;
    uint16_t t_div_fraction = 0;
    sys_clk_freq_info_t t_clk_freq_info;
    float temp = 0 ;
    drv_get_clock_freq_info(&t_clk_freq_info);
    /*enable clock*/
    if(usart_ch == USART_1)
    {
        RCC->APB2ENR |= (uint32_t)0x00004000;
        t_usart_clk_freq = t_clk_freq_info.pclk2_clk_freq;
    }
    else
    {
        RCC->APB1ENR |= (uint32_t)0x01 << (usart_ch + 16);
        t_usart_clk_freq = t_clk_freq_info.pclk1_clk_freq;
    }
    /*congif data_len*/
    USART_ARRAY[usart_ch]->CR1 &= ~((uint32_t)0x1000);  //clear 0
    USART_ARRAY[usart_ch]->CR1 |= (uint32_t)usart_init_struct->data_len;
    /*congif parity*/
    USART_ARRAY[usart_ch]->CR1 &= ~((uint32_t)0x0600); 
    USART_ARRAY[usart_ch]->CR1 |= (uint32_t)usart_init_struct->parity;
    /*congif mode*/
    USART_ARRAY[usart_ch]->CR1 &= ~((uint32_t)0x000C);
    USART_ARRAY[usart_ch]->CR1 |= (uint32_t)usart_init_struct->mode;
    /*congif stopbits*/
    USART_ARRAY[usart_ch]->CR2 &= ~((uint32_t)0x3000);
    USART_ARRAY[usart_ch]->CR2 |= (uint32_t)usart_init_struct->stop_bits;
    /*congif usart_baudrate*/
    temp = ((float)t_usart_clk_freq)/(usart_init_struct->usart_baudrate*16);
    t_div_mantissa = (uint16_t)temp;
    t_div_fraction = (uint16_t)((temp - t_div_mantissa)*16);
    if((t_div_fraction & 0xfff0) !=0)
    {
        /*t_div_fraction > 4 bit,t_div_mantissa+1,t_div_fraction =0*/
        t_div_fraction = 0;
        t_div_mantissa += 1;
    }
    else
    {
    
    }
    t_div_mantissa <<= 4;
    t_div_mantissa += t_div_fraction;
    USART_ARRAY[usart_ch]->BRR = t_div_mantissa;
    /*congif hardware_flow_ctrl(USART_4,USART_5 no surport)*/
    if((usart_ch == USART_1 )|| (usart_ch == USART_2 ) ||(usart_ch == USART_3 ))
    {
        USART_ARRAY[usart_ch]->CR3 &= ~((uint32_t)0x0300);
        USART_ARRAY[usart_ch]->CR3 |= (uint32_t)usart_init_struct->hardware_flow_ctrl;
    }
    
}


/**
  * @brief usart enable/disable 
  * @note   None
  * @param  
  * @retval None
 */
void drv_usart_cmd(usart_ch_t usart_ch,bool status)
{
    if(status == ENABLE_DEF)
    {
        USART_ARRAY[usart_ch]->CR1 |= (uint32_t)0x2000;
    }
    else
    {
        USART_ARRAY[usart_ch]->CR1 &= ~((uint32_t)0x2000);
    }
}


/**
  * @brief usart interrupt config
  * @note   None
  * @param  
  * @retval None
 */
void drv_usart_it_config(usart_ch_t usart_ch,usart_it_source_t it_source,bool status)
{
    uint8_t usartreg = 0x00;
    uint32_t itpos = 0x00;
    uint32_t itmask = 0x00;
    if(it_source==USART_IT_CTS)
    {
        if((usart_ch == USART_4) || (usart_ch == USART_5))
        {
            return;
        }
    }
    
    /* Get the USART register index */
    usartreg = (((uint8_t)it_source) >> 0x05);
    /* Get the interrupt position */
    itpos = ((uint16_t)it_source) & ((uint16_t)0x001F);
    itmask = (((uint32_t)0x01) << itpos);
    if(usartreg == 0x01)
    {
        if( status==ENABLE_DEF)
        {
            USART_ARRAY[usart_ch]->CR1 |= itmask;
        }
        else
        {
             USART_ARRAY[usart_ch]->CR1 &= ~itmask;
        }
        
    }
    else if(usartreg == 0x02)
    {
        if( status==ENABLE_DEF)
        {
            USART_ARRAY[usart_ch]->CR2 |= itmask;
        }
        else
        {
             USART_ARRAY[usart_ch]->CR2 &= ~itmask;
        }
    }
    else
    {
        if( status==ENABLE_DEF)
        {
            USART_ARRAY[usart_ch]->CR3 |= itmask;
        }
        else
        {
             USART_ARRAY[usart_ch]->CR3 &= ~itmask;
        }
    }  
}


/**
  * @brief get usart xx bit of usart SR resgiter 
  * @note   None
  * @param  
  * @retval None
 */
bool drv_get_usart_it_status(usart_ch_t usart_ch,usart_it_source_t it_source)
{
    bool bitstatus = FALSE;
    uint32_t bitpos = 0x00;
    uint32_t itmask = 0x00;
    uint8_t usartreg = 0x00;
    if(it_source==USART_IT_CTS)
    {
        if((usart_ch == USART_4) || (usart_ch == USART_5))
        {
            return FALSE;
        }
    }
    /* Get the USART register index */
    usartreg = (((uint8_t)it_source) >> 0x05);
    /* Get the interrupt position */
    itmask = ((uint16_t)it_source) & ((uint16_t)0x001F);
    itmask = (uint32_t)0x01 << itmask;
    if(usartreg == 0x01)
    {      
        itmask = USART_ARRAY[usart_ch]->CR1 & itmask;             
    }
    else if(usartreg == 0x02)
    {
        itmask = USART_ARRAY[usart_ch]->CR2 & itmask;
    }
    else
    {
        itmask = USART_ARRAY[usart_ch]->CR3 & itmask;
    }  
    bitpos = it_source >> 0x08;
    bitpos = (uint32_t)0x01 << bitpos;
    bitpos &= USART_ARRAY[usart_ch]->SR;
    
    if ((itmask != (uint16_t)FALSE)&&(bitpos != (uint16_t)FALSE))
    {
        bitstatus = TRUE;
    }
    else
    {
        bitstatus = FALSE;
    }
    return bitstatus;
}
 

/**
  * @brief  usart send 
  * @note   None
  * @param  
  * @retval None
 */
void drv_usart_send(usart_ch_t usart_ch,uint16_t data)
{
    USART_ARRAY[usart_ch]->DR = (data & (uint16_t)0x01FF);
}


/**
  * @brief  usart receive 
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_usart_receive(usart_ch_t usart_ch)
{
    return (uint16_t)(USART_ARRAY[usart_ch]->DR & (uint16_t)0x01FF);
}


/**
  * @brief  usart interrupt config 
  * @note   None
  * @param  
  * @retval None
 */
void drv_usart_isr_config(usart_ch_t usart_ch,voidfunc_t p_callback,sci_callback_t callback_type)
{
    if(usart_ch == USART_1)
    {
        if( NULL != p_callback )
        {
            if(callback_type == USART_RX_CALL_BAKE)
            {
                p_usart1_recv_interrupt = p_callback;
            }
            else
            {
                p_usart1_send_interrupt = p_callback;
            }
        }
        
    }
    else if(usart_ch == USART_2)
    {
        if( NULL != p_callback )
        {
            if(callback_type == USART_RX_CALL_BAKE)
            {
                p_usart2_recv_interrupt = p_callback;
            }
            else
            {
                p_usart2_send_interrupt = p_callback;
            }
        }
    }
    else if(usart_ch == USART_3)
    {
        if( NULL != p_callback )
        {
            if(callback_type == USART_RX_CALL_BAKE)
            {
                p_usart3_recv_interrupt = p_callback;
            }
            else
            {
                p_usart3_send_interrupt = p_callback;
            }
        }
    }
    else if(usart_ch == USART_4)
    {
        if( NULL != p_callback )
        {
            if(callback_type == USART_RX_CALL_BAKE)
            {
                p_usart4_recv_interrupt = p_callback;
            }
            else
            {
                p_usart4_send_interrupt = p_callback;
            }
        }
    }
    else
    {
        if( NULL != p_callback )
        {
            if(callback_type == USART_RX_CALL_BAKE)
            {
                p_usart5_recv_interrupt = p_callback;
            }
            else
            {
                p_usart5_send_interrupt = p_callback;
            }
        }
    }
}


/**
  * @brief  usart1 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void USART1_IRQHandler(void) 
{
    bool rx_status = FALSE;
    bool tx_status = FALSE;
    rx_status = drv_get_usart_it_status(USART_1,USART_IT_RXNE);
    if(rx_status == TRUE)  /*RX IRQ*/
    {
        if(NULL != p_usart1_recv_interrupt)
        {
            p_usart1_recv_interrupt();
        }
        
    }
    else
    {
    
    }
    tx_status = drv_get_usart_it_status(USART_1,USART_IT_TC);
    if(tx_status == TRUE)  /*TX  IRQ*/
    {
        if(NULL != p_usart1_send_interrupt)
        {
            p_usart1_send_interrupt();
        }
        
    }
    else
    {
    
    }
}


/**
  * @brief  usart2 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void USART2_IRQHandler(void) 
{
    bool rx_status = FALSE;
    bool tx_status = FALSE;
    rx_status = drv_get_usart_it_status(USART_2,USART_IT_RXNE);
    if(rx_status == TRUE)  /*RX IRQ*/
    {
        if(NULL != p_usart2_recv_interrupt)
        {
            p_usart2_recv_interrupt();
        }
        
    }
    else
    {
    
    }
    tx_status = drv_get_usart_it_status(USART_2,USART_IT_TC);
    if(tx_status == TRUE)  /*TX IRQ*/
    {
        if(NULL != p_usart2_send_interrupt)
        {
            p_usart2_send_interrupt();
        }
        
    }
    else
    {
    
    }
}


/**
  * @brief  usart3 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void USART3_IRQHandler(void) 
{
    bool rx_status = FALSE;
    bool tx_status = FALSE;
    rx_status = drv_get_usart_it_status(USART_3,USART_IT_RXNE);
    if(rx_status == TRUE)  /*RX IRQ*/
    {
        if(NULL != p_usart3_recv_interrupt)
        {
            p_usart3_recv_interrupt();
        }
        
    }
    else
    {
    
    }
    tx_status = drv_get_usart_it_status(USART_3,USART_IT_TC);
    if(tx_status == TRUE)  /*TX IRQ*/
    {
        if(NULL != p_usart3_send_interrupt)
        {
            p_usart3_send_interrupt();
        }
        
    }
    else
    {
    
    }
}


/**
  * @brief  usart4 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void USART4_IRQHandler(void) 
{
    bool rx_status = FALSE;
    bool tx_status = FALSE;
    rx_status = drv_get_usart_it_status(USART_4,USART_IT_RXNE);
    if(rx_status == TRUE)  /*RX IRQ*/
    {
        if(NULL != p_usart4_recv_interrupt)
        {
            p_usart4_recv_interrupt();
        }
        
    }
    else
    {
    
    }
    tx_status = drv_get_usart_it_status(USART_4,USART_IT_TC);
    if(tx_status == TRUE)  /*TX IRQ*/
    {
        if(NULL != p_usart4_send_interrupt)
        {
            p_usart1_send_interrupt();
        }
        
    }
    else
    {
    
    }
}


/**
  * @brief  usart5 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void USART5_IRQHandler(void) 
{
    bool rx_status = FALSE;
    bool tx_status = FALSE;
    rx_status = drv_get_usart_it_status(USART_5,USART_IT_RXNE);
    if(rx_status == TRUE)  /*RX IRQ*/
    {
        if(NULL != p_usart5_recv_interrupt)
        {
            p_usart5_recv_interrupt();
        }
        
    }
    else
    {
    
    }
    tx_status = drv_get_usart_it_status(USART_5,USART_IT_TC);
    if(tx_status == TRUE)  /*TX IRQ*/
    {
        if(NULL != p_usart5_send_interrupt)
        {
            p_usart5_send_interrupt();
        }
        
    }
    else
    {
    
    }
}
