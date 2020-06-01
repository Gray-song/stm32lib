/**
  ******************************************************************************
  * @file    hal_adc.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_
#include "drv_adc.h"
#include "platform_config.h"
#define ADC_FILTER_ENABLE 0 
typedef struct 
{
   adc_channel_t channel;
   uint16_t cvt_value;
   adc_sample_duty_t adc_sample_duty;
}adc_signal_t;
extern void hal_adc_init(void);
extern void hal_ai_read(void); 
extern io_err_t hal_adc_sequence_trigger(void);
extern uint16_t hal_adc_signal_get(adc_signal_name_t signl);
#endif
