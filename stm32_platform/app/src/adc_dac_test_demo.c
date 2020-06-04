/**
  ******************************************************************************
  * @file    gpio_test_demo.h
  * @author  gray
  * @version V1.0.0
  * @date    4-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/

#include "headers.h"


/**
  * @brief  adc_test
  * @note   DAC is PA4 ADC IS PA0 ,PA1
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void adc_dac_test()
{
    hal_ai_read();
    (void)hal_adc_sequence_trigger();
    hal_set_vol_for_12b(2500);
}


/**
  * @brief  adc_test
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void adc_result_response(void)
{
    uint8_t adc_result_buff[5] = {0};
    uint16_t adc_result;
    static uint8_t i=0;
    
    
    if(i>ADC_SEND_PERIOD_100MS)
    {
        i=0;
        adc_result_buff[0] = 0xfc;
        adc_result = hal_adc_signal_get(ADC_3V3);
        adc_result_buff[1] = (uint8_t)((adc_result&0xff00)>>8);
        adc_result_buff[2] = (uint8_t)(adc_result&0x00ff);
        adc_result = hal_adc_signal_get(ADC_GND);
        adc_result_buff[3] = (uint8_t)((adc_result&0xff00)>>8);
        adc_result_buff[4] = (uint8_t)(adc_result&0x00ff);
        hal_usart_send(USART_1,adc_result_buff,5);
    }
    else
    {
        i++;
    }
    
}
