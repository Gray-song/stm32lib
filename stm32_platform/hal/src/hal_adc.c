/**
  ******************************************************************************
  * @file    hal_adc.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_adc.h"
static volatile bool convert_complete = TRUE;
static volatile uint8_t cur_convert = ADC_SIGNALS_COUNT;

static uint16_t s_adc_ele_current[ADC_SIGNALS_COUNT] = {0};
static adc_signal_t ai_signal_array[ADC_SIGNALS_COUNT] = 
{
    ADC_SIGNALS_CONFIG
};
static void hal_adc_complete_isr(void);


/**
  * @brief  hal_adc_complete_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
static void hal_adc_complete_isr(void) 
{
    /* parameter check */
    if(cur_convert >= ADC_SIGNALS_COUNT) 
    {
        /* parameter error */
        cur_convert = 0;
        convert_complete = TRUE;
    } 
    else 
    {
        /* load convert data */
        ai_signal_array[cur_convert].cvt_value = drv_adc_read(ADC_1);
        cur_convert++;
        /* user convert sequence complete check */
        if (cur_convert < ADC_SIGNALS_COUNT)
        {
            /* not complete, convert next signal */
             drv_adc_channel_convert(ADC_1, ai_signal_array[cur_convert].channel,ai_signal_array[cur_convert].adc_sample_duty); 
        }
        else
        {
            convert_complete = TRUE;
        }
    }
    /* clear interrupt flag */
    drv_adc_clear_it_flag(ADC_1, EOC);
}


/**
  * @brief  hal_adc_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
extern void hal_adc_init(void)
{
    adc_init_t adc_init_struct;
    adc_init_struct.adc_mode = ADC_Mode_Independent;
    adc_init_struct.adc_convert_mode_scan = DISABLE_DEF;
    adc_init_struct.adc_convert_mode_continus = DISABLE_DEF;
    adc_init_struct.adc_convert_trig_way = ADC_CONV_TRIG_SOFTWARE;
    adc_init_struct.adc_data_align = RIGHT_ALIGN;
    adc_init_struct.adc_num_brOfChannel = 1;
    drv_adc_init(ADC_1, adc_init_struct);
    drv_adc_channel_cmd(ADC_1 , ENABLE_DEF);
    drv_adc_reset_calibration(ADC_1);
    drv_adc_start_calibration(ADC_1);
    drv_adc_it_config(ADC_1,EOC,ENABLE_DEF);
    drv_adc_isr_config(ADC_1,&hal_adc_complete_isr,EOC);
}


/**
  * @brief  hal_adc_sequence_trigger
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
io_err_t hal_adc_sequence_trigger(void)
{
    io_err_t trg_rst = IO_ERR_BUSY;
    
    /* check last convert is complete or not */
    if (TRUE == convert_complete)
    {
        /* last convert complete, start new convert */
        /* set comvert busy */
        convert_complete = FALSE;
        /* set current convert to signal 0 (Note: not the adc ch0) */
        cur_convert = 0;
        drv_adc_channel_convert(ADC_1, ai_signal_array[cur_convert].channel,ai_signal_array[cur_convert].adc_sample_duty);
        trg_rst = IO_ERR_OK;
    }
    else
    {
        /* convert busy, do nothing, match MISRA-C */
    }

    return trg_rst;
}


/**
  * @brief  hal_ai_read
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_ai_read(void)
{
    //static bool adc_get_first_f = TRUE;
    uint8_t index;

    drv_adc_it_config(ADC_1,EOC,DISABLE_DEF);
    for (index = 0; index < ADC_SIGNALS_COUNT; index++)
    {       
        #if ADC_FILTER_ENABLE   
        /*there are bug*/        
        if(FALSE == adc_get_first_f)
        {      
            filter_val = s_adc_ele_current[index] - (s_adc_ele_current[index] >> 3);
            filter_val += ai_signal_array[index].cvt_value >> 3;
            s_adc_ele_current[index] = filter_val;
        }
        else
        {  
            
            s_adc_ele_current[index] = ai_signal_array[index].cvt_value;  
        }
        
        #else
        s_adc_ele_current[index] = ai_signal_array[index].cvt_value;
        #endif
    }
    //adc_get_first_f = FALSE;
    drv_adc_it_config(ADC_1,EOC,ENABLE_DEF);
}


/**
  * @brief  hal_adc_signal_get
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
uint16_t hal_adc_signal_get(adc_signal_name_t signl) 
{
   return s_adc_ele_current[(uint8_t)signl];
}
