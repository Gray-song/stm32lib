/**
  ******************************************************************************
  * @file    hal_usart.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_usart.h"
static void hal_usart1_rx_isr(void);
static void hal_usart1_tx_isr(void);
static void hal_usart2_rx_isr(void);
static void hal_usart2_tx_isr(void);
static void hal_usart3_rx_isr(void);
static void hal_usart3_tx_isr(void);
static void hal_usart4_rx_isr(void);
static void hal_usart4_tx_isr(void);
static void hal_usart5_rx_isr(void);
static void hal_usart5_tx_isr(void);
static void hal_usart_rx_isr(usart_ch_t usart_ch);
static void hal_usart_tx_isr(usart_ch_t usart_ch);


static volatile usart_queue_t usart_receive_q[USART_CHANNEL_COUNT] = 
{
    {0, 0, {0}},
    {0, 0, {0}}
};
static volatile usart_queue_t usart_transmit_q[USART_CHANNEL_COUNT] = 
{
    {0, 0, {0}},
    {0, 0, {0}}
};
static const usart_init_t usart_init_struct[USART_CHANNEL_COUNT] =
{
    USART_CONFIG
};


static const usart_iqr_t usart_isr_table[USART_CHANNEL_COUNT] = 
{
    USART_ISR_CONFIG
};


/**
  * @brief  hal_usart_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_usart_init(usart_ch_t usart_ch)
{
    drv_usart_init(usart_ch,&usart_init_struct[usart_ch]);
    drv_usart_isr_config(usart_ch,usart_isr_table[usart_ch].rx_isr,USART_RX_CALL_BAKE);
    drv_usart_isr_config(usart_ch,usart_isr_table[usart_ch].tx_isr,USART_TX_CALL_BAKE);
    drv_usart_it_config(usart_ch,USART_IT_RXNE,ENABLE_DEF);
    drv_usart_cmd(usart_ch,ENABLE_DEF);
}


/**
  * @brief  hal_usart_recieve_length_get
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
uint8_t hal_usart_recieve_length_get(usart_ch_t usart_ch)
{
    uint16_t len;
  
    hal_disable_rx_interrupt(usart_ch);
    if (usart_receive_q[usart_ch].head <= usart_receive_q[usart_ch].tail)
    {
     len = usart_receive_q[usart_ch].tail - usart_receive_q[usart_ch].head;
    }
    else
    {
     len = (USART_DATA_BUFFER_SIZE - usart_receive_q[usart_ch].head) + usart_receive_q[usart_ch].tail;
    }
    hal_enable_rx_interrupt(usart_ch);

    return len;
}


/**
  * @brief  hal_usart_send
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
bool hal_usart_send(usart_ch_t usart_ch, const uint8_t data[], uint8_t dt_len)
{
    /* you must disable interrupt hear and recover interrupt after checked */
    uint8_t data_index;
    bool b_value = TRUE;

    /* check data length */
    if ((0 == dt_len) || (dt_len > USART_DATA_BUFFER_SIZE)) 
    {
        b_value = FALSE;
    } 
    else   /* if the size of data to be sent is over the maximun size,the parameter can be wrong,then nothing should be sent */
    {
        /*disable USART transmission interrupts*/
        hal_disable_tx_interrupt(usart_ch);
        
        /* push all tatas to transmit buffer */
        for (data_index = 0; data_index < dt_len; data_index++)
        {
           usart_transmit_q[usart_ch].buffer[usart_transmit_q[usart_ch].tail] = data[data_index];
           usart_transmit_q[usart_ch].tail++;
           if (usart_transmit_q[usart_ch].tail >= USART_DATA_BUFFER_SIZE) 
           {
              usart_transmit_q[usart_ch].tail = 0;
           }

            /* queue full check */
            if (usart_transmit_q[usart_ch].tail == usart_transmit_q[usart_ch].head) 
            {
                /* Remove the oldest data */
                usart_transmit_q[usart_ch].head++;  
                if (usart_transmit_q[usart_ch].head >= USART_DATA_BUFFER_SIZE) 
                {
                    usart_transmit_q[usart_ch].head = 0;
                }
                /*over load*/
                b_value = FALSE;
            } 
        }
        
        /*Enable transmit interrupt*/
        hal_enable_tx_interrupt(usart_ch); 
    }
    
    return b_value;   
}


/**
  * @brief  hal_usart_read
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
uint8_t hal_usart_read(usart_ch_t usart_ch, uint8_t buffer[], uint8_t buffer_length, uint8_t read_len)
{
    /* you must disable interrupt hear and recover interrupt after checked */
    uint16_t u1_i = 0;

    /* disable receive interrupt */
    hal_disable_rx_interrupt(usart_ch);
    
    /* check input parameters */
    if (buffer_length >= read_len)
    {
        /* parameters ok, load data to target buffer */
        for(u1_i =0; (u1_i < read_len) && (usart_receive_q[usart_ch].head != usart_receive_q[usart_ch].tail); u1_i++) 
        {
            /* load data */
            buffer[u1_i] = usart_receive_q[usart_ch].buffer[usart_receive_q[usart_ch].head];
            usart_receive_q[usart_ch].head++;
            /* poniter warp process */
            if (usart_receive_q[usart_ch].head >= USART_DATA_BUFFER_SIZE) 
            {
               usart_receive_q[usart_ch].head = 0;              
            } 
        }
    } 
    
    /* recover receive interrupt */
    hal_enable_rx_interrupt(usart_ch);
    
    return u1_i;
}


/**
  * @brief  hal_usart_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart_rx_isr(usart_ch_t usart_ch)
{
    usart_receive_q[usart_ch].buffer[usart_receive_q[usart_ch].tail] = (uint8_t)drv_usart_receive(usart_ch);
    usart_receive_q[usart_ch].tail++;
    /* poniter warp process */
    if (usart_receive_q[usart_ch].tail >= USART_DATA_BUFFER_SIZE) 
    {
       usart_receive_q[usart_ch].tail = 0;              
    } 
       
    /* queue full check */
    if (usart_receive_q[usart_ch].tail == usart_receive_q[usart_ch].head) 
    {
       usart_receive_q[usart_ch].head++;
       if (usart_receive_q[usart_ch].head >= USART_DATA_BUFFER_SIZE) 
       {
          usart_receive_q[usart_ch].head = 0;              
       }              
    }
}


/**
  * @brief  hal_usart_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart_tx_isr(usart_ch_t usart_ch)
{
    if (usart_transmit_q[usart_ch].tail != usart_transmit_q[usart_ch].head)
    {        
        drv_usart_send(usart_ch, usart_transmit_q[usart_ch].buffer[usart_transmit_q[usart_ch].head]);
        
        usart_transmit_q[usart_ch].head++;
        if (usart_transmit_q[usart_ch].head >= USART_DATA_BUFFER_SIZE) 
        {
           usart_transmit_q[usart_ch].head = 0;
        }
        /* enable the transmit interrupt only if there have a data need to be sent */
        hal_enable_tx_interrupt(usart_ch);
    } 
    else         /* Note: this interrupt is a buffer empty interrupt, Do not enable interrput when no data need to transmit */
    {
       hal_disable_tx_interrupt(usart_ch);
    }

}
    
    
/**
  * @brief  hal_usart1_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart1_rx_isr()
{
    hal_usart_rx_isr(USART_1);
}


/**
  * @brief  hal_usart1_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart1_tx_isr()
{
    hal_usart_tx_isr(USART_1);
}


/**
  * @brief  hal_usart2_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart2_rx_isr()
{
    hal_usart_rx_isr(USART_2);
}


/**
  * @brief  hal_usart2_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart2_tx_isr()
{
    hal_usart_tx_isr(USART_2);
}


/**
  * @brief  hal_usart3_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart3_rx_isr()
{
    hal_usart_rx_isr(USART_3);
}


/**
  * @brief  hal_usart3_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart3_tx_isr()
{
    hal_usart_tx_isr(USART_3);
}


/**
  * @brief  hal_usart4_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart4_rx_isr()
{
    hal_usart_rx_isr(USART_4);
}


/**
  * @brief  hal_usart4_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart4_tx_isr()
{
    hal_usart_tx_isr(USART_4);
}


/**
  * @brief  hal_usart5_rx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart5_rx_isr()
{
    hal_usart_rx_isr(USART_5);
}


/**
  * @brief  hal_usart5_tx_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_usart5_tx_isr()
{
    hal_usart_tx_isr(USART_5);
}
