/**
  ******************************************************************************
  * @file    drv_adc.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_
#include "stm32f103rc.h"

#define ADC_COUNT 3
#define ADC_CONV_TRIG_EXTERN_T1_CC1                ((uint32_t)0x00000000) /*!< For ADC1 and ADC2 */
#define ADC_CONV_TRIG_EXTERN_T1_CC2                ((uint32_t)0x00020000) /*!< For ADC1 and ADC2 */
#define ADC_CONV_TRIG_EXTERN_T2_CC2                ((uint32_t)0x00060000) /*!< For ADC1 and ADC2 */
#define ADC_CONV_TRIG_EXTERN_T3_TRGO               ((uint32_t)0x00080000) /*!< For ADC1 and ADC2 */
#define ADC_CONV_TRIG_EXTERN_T4_CC4                ((uint32_t)0x000A0000) /*!< For ADC1 and ADC2 */
#define ADC_CONV_TRIG_EXTERN_EXTI11_OR_T8_TRGO    ((uint32_t)0x000C0000) /*!< For ADC1 and ADC2 */

#define ADC_CONV_TRIG_EXTERN_T1_CC3                ((uint32_t)0x00040000) /*!< For ADC1, ADC2 and ADC3 */
#define ADC_CONV_TRIG_SOFTWARE                     ((uint32_t)0x000E0000) /*!< For ADC1, ADC2 and ADC3 */

#define ADC3_CONV_TRIG_EXTERN_T3_CC1                ((uint32_t)0x00000000) /*!< For ADC3 only */
#define ADC3_CONV_TRIG_EXTERN_T2_CC3                ((uint32_t)0x00020000) /*!< For ADC3 only */
#define ADC3_CONV_TRIG_EXTERN_T8_CC1                ((uint32_t)0x00060000) /*!< For ADC3 only */
#define ADC3_CONV_TRIG_EXTERN_T8_TRGO               ((uint32_t)0x00080000) /*!< For ADC3 only */
#define ADC3_CONV_TRIG_EXTERN_T5_CC1                ((uint32_t)0x000A0000) /*!< For ADC3 only */
#define ADC3_CONV_TRIG_EXTERN_T5_CC3                ((uint32_t)0x000C0000) /*!< For ADC3 only */

typedef enum
{
    ADC_1 = 0,
    ADC_2,
    ADC_3

}adc_map_t;


typedef enum
{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1 ,
    ADC_CHANNEL_2 ,
    ADC_CHANNEL_3 ,
    ADC_CHANNEL_4 ,
    ADC_CHANNEL_5 ,
    ADC_CHANNEL_6 ,
    ADC_CHANNEL_7 ,
    ADC_CHANNEL_8 ,
    ADC_CHANNEL_9 ,
    ADC_CHANNEL_10 ,
    ADC_CHANNEL_11 ,
    ADC_CHANNEL_12 ,
    ADC_CHANNEL_13 ,
    ADC_CHANNEL_14 ,
    ADC_CHANNEL_15 ,
    ADC_CHANNEL_16 ,
    ADC_CHANNEL_17 ,
}adc_channel_t;

typedef enum
{
    ADC_Mode_Independent = 0,
    ADC_Mode_RegInjecSimult  ,                  
    ADC_Mode_RegSimult_AlterTrig ,        
    ADC_Mode_InjecSimult_FastInterl ,          
    ADC_Mode_InjecSimult_SlowInterl ,           
    ADC_Mode_InjecSimult  ,                     
    ADC_Mode_RegSimult  ,                       
    ADC_Mode_FastInterl ,                       
    ADC_Mode_SlowInterl  ,                      
    ADC_Mode_AlterTrig
}adc_mode_t;

typedef enum
{
   EOC =  0,
   JEOC ,
   AWD    
}adc_interrupt_t;

typedef enum
{
    RIGHT_ALIGN = 0 ,
    LEFT_ALIGN
}adc_data_align_t;

typedef enum
{
    adc_clk_1_5 = 0,
    adc_clk_7_5,
    adc_clk_13_5,
    adc_clk_28_5,
    adc_clk_41_5,
    adc_clk_55_5,
    adc_clk_71_5,
    adc_clk_239_5

}adc_sample_duty_t;
    
typedef struct 
{
    adc_mode_t adc_mode;
    bool adc_convert_mode_scan;
    bool adc_convert_mode_continus;
    uint32_t adc_convert_trig_way;
    adc_data_align_t adc_data_align;
    uint8_t adc_num_brOfChannel;
}adc_init_t;
extern void drv_adc_init(adc_map_t adcx, adc_init_t adc_init_struct);
extern void drv_adc_channel_cmd(adc_map_t adcx,bool status);
extern void drv_adc_reset_calibration(adc_map_t adcx);
extern void drv_adc_start_calibration(adc_map_t adcx);
extern void drv_adc_channel_convert(adc_map_t adcx,adc_channel_t channel,adc_sample_duty_t);
extern uint16_t drv_adc_read(const adc_map_t adcx);
extern void drv_adc_it_config(adc_map_t adcx,adc_interrupt_t interrupt_type,bool status);
extern void drv_adc_isr_config(const adc_map_t adcx,voidfunc_t p_callback, adc_interrupt_t interrupt_type);
extern void drv_adc_clear_it_flag(const adc_map_t adcx,adc_interrupt_t interrupt_type);
#endif

