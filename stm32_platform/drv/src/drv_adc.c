/**
  ******************************************************************************
  * @file    drv_adc.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_adc.h"

static voidfunc_t p_adc1_eoc_interrupt = NULL;
static voidfunc_t p_adc1_jeoc_interrupt = NULL;
static voidfunc_t p_adc1_awd_interrupt = NULL;
static voidfunc_t p_adc2_eoc_interrupt = NULL;
static voidfunc_t p_adc2_jeoc_interrupt = NULL;
static voidfunc_t p_adc2_awd_interrupt = NULL;
static voidfunc_t p_adc3_eoc_interrupt = NULL;
static voidfunc_t p_adc3_jeoc_interrupt = NULL;
static voidfunc_t p_adc3_awd_interrupt = NULL;

static volatile ADC_TypeDef* const ADC_MAP[ADC_COUNT] = 
{
    ADC1,
    ADC2,
    ADC3
};


/**
  * @brief  adc init
  * @note   None
  * @param  
  * @retval None
 */
 void drv_adc_init(adc_map_t adcx, adc_init_t adc_init_struct)
 {
     /*enable clk*/
     if(adcx < ADC_3)
     {
         RCC->APB2ENR &= ~(((uint32_t)0x01) << (adcx + 9));
         RCC->APB2ENR |= ((uint32_t)0x01) << (adcx + 9);
         RCC->APB2RSTR |= ((uint32_t)0x01) << (adcx + 9);
         RCC->APB2RSTR &= ~(((uint32_t)0x01) << (adcx + 9));
     }
     else
     {
         RCC->APB2ENR &= ~(((uint32_t)0x01) << 15);
         RCC->APB2ENR |= ((uint32_t)0x01) << 15;
         RCC->APB2RSTR |= ((uint32_t)0x01) << 15;
         RCC->APB2RSTR &= ~(((uint32_t)0x01) << 15);
     }
     /*set adc clk div*/
     RCC->CFGR &= ~(((uint32_t)0x03) << 14);
     RCC->CFGR |= ((uint32_t)0x02) << 14 ;
     
     ADC_MAP[adcx]->CR1 &= ~(((uint32_t)0x0f) << 16);
     ADC_MAP[adcx]->CR1 |= ((uint32_t)adc_init_struct.adc_mode) <<16;
     ADC_MAP[adcx]->CR1 &= ~(((uint32_t)0x01) << 8);
     ADC_MAP[adcx]->CR1 |= ((uint32_t)adc_init_struct.adc_convert_mode_scan) <<8;
     ADC_MAP[adcx]->CR2 &= ~(((uint32_t)0x01) << 1);
     ADC_MAP[adcx]->CR2 |= ((uint32_t)adc_init_struct.adc_convert_mode_continus) <<1;
     ADC_MAP[adcx]->CR2 &= ~(((uint32_t)0x07) << 17);
     ADC_MAP[adcx]->CR2 |= (uint32_t)adc_init_struct.adc_convert_trig_way;
     ADC_MAP[adcx]->CR2 &= ~(((uint32_t)0x01) << 11);
     ADC_MAP[adcx]->CR2 |= ((uint32_t)adc_init_struct.adc_data_align) <<11;
     ADC_MAP[adcx]->SQR1 &= ~(((uint32_t)0x0f) << 20);
     ADC_MAP[adcx]->SQR1 |= (((uint32_t)(adc_init_struct.adc_num_brOfChannel-1)) << 20);
     
     
 }


/**
  * @brief  adc channel enable/disable
  * @note   None
  * @param  
  * @retval None
 */ 
void drv_adc_channel_cmd(adc_map_t adcx,bool status)
{
    if(status == ENABLE_DEF)
    {
        ADC_MAP[adcx]->CR2 |= ((uint32_t)0x01);
    }
    else
    {
        ADC_MAP[adcx]->CR2 &= ~((uint32_t)0x01);
    }
}


/**
  * @brief  adc reset calibration
  * @note   None
  * @param  
  * @retval None
 */
void drv_adc_reset_calibration(adc_map_t adcx)
{
    ADC_MAP[adcx]->CR2 |= ((uint32_t)0x01)<<3;
    while(ADC_MAP[adcx]->CR2 & (((uint32_t)0x01)<<3));
}


/**
  * @brief  adc start calibration
  * @note   None
  * @param  
  * @retval None
 */
void drv_adc_start_calibration(adc_map_t adcx)
{
    ADC_MAP[adcx]->CR2 |= ((uint32_t)0x01)<<2;
    while(ADC_MAP[adcx]->CR2 & (((uint32_t)0x01)<<2));
}


/**
  * @brief  adc channel convert
  * @note   None
  * @param  
  * @retval None
 */
void drv_adc_channel_convert(adc_map_t adcx,adc_channel_t channel,adc_sample_duty_t adc_sample_duty)
{
    if(channel > ADC_CHANNEL_9)
    {
        ADC_MAP[adcx]->SMPR1 &= ~(((uint32_t)0x07) << (3*(channel-10))) ;
        ADC_MAP[adcx]->SMPR1 |= ((uint32_t)adc_sample_duty) << (3*(channel-10));
    }
    else
    {
        ADC_MAP[adcx]->SMPR2 &= ~(((uint32_t)0x07) << (3*channel)) ;
        ADC_MAP[adcx]->SMPR2 |= ((uint32_t)adc_sample_duty) << (3*channel);
    }
    ADC_MAP[adcx]->SQR3 &= ~((uint32_t)0x1f);
    ADC_MAP[adcx]->SQR3 |= channel;
    ADC_MAP[adcx]->CR2 |= ((uint32_t)0x05)<<20;
}


/**
  * @brief  read adc value
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_adc_read(const adc_map_t adcx)
{
    return (uint16_t)ADC_MAP[adcx]->DR;
}


/**
  * @brief  adc interrupt enable/disable
  * @note   None
  * @param  
  * @retval None
 */
void drv_adc_it_config(adc_map_t adcx,adc_interrupt_t interrupt_type,bool status)
{
    if(status == ENABLE_DEF)
    {
        ADC_MAP[adcx]->CR1 |= ((uint32_t)0x01) << (interrupt_type+5);
    }
    else
    {
        ADC_MAP[adcx]->CR1 &= ~(((uint32_t)0x01) << (interrupt_type+5));
    }

}


/**
  * @brief  adc interrupt config
  * @note   None
  * @param  
  * @retval None
 */
extern void drv_adc_isr_config(const adc_map_t adcx,voidfunc_t p_callback, adc_interrupt_t interrupt_type)
{
    if(adcx == ADC_1)
    {
        if(interrupt_type == EOC)
        {
            if(p_callback != NULL)
            {
                p_adc1_eoc_interrupt = p_callback;
            }
        }
        else if(interrupt_type == JEOC)
        {
            if(p_callback != NULL)
            {
                p_adc1_jeoc_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_adc1_awd_interrupt = p_callback;
            }
        }
            
            
    }
    else if(adcx == ADC_2)
    {
        if(interrupt_type == EOC)
        {
            if(p_callback != NULL)
            {
                p_adc2_eoc_interrupt = p_callback;
            }
        }
        else if(interrupt_type == JEOC)
        {
            if(p_callback != NULL)
            {
                p_adc2_jeoc_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_adc2_awd_interrupt = p_callback;
            }
        }
    }
    else
    {
        if(interrupt_type == EOC)
        {
            if(p_callback != NULL)
            {
                p_adc3_eoc_interrupt = p_callback;
            }
        }
        else if(interrupt_type == JEOC)
        {
            if(p_callback != NULL)
            {
                p_adc3_jeoc_interrupt = p_callback;
            }
        }
        else
        {
            if(p_callback != NULL)
            {
                p_adc3_awd_interrupt = p_callback;
            }
        }
    }
}


/**
  * @brief  clear interrupt flag
  * @note   None
  * @param  
  * @retval None
 */
void drv_adc_clear_it_flag(const adc_map_t adcx,adc_interrupt_t interrupt_type)
{
    if(interrupt_type == JEOC)
    {
        ADC_MAP[adcx]->SR &= ~((uint32_t)0x04);
    }
    else if(interrupt_type == EOC)
    {
        ADC_MAP[adcx]->SR &= ~((uint32_t)0x02);
    }
    else
    {
        ADC_MAP[adcx]->SR &= ~((uint32_t)0x01);
    }
}


/**
  * @brief  adc1 or adc2 interrupr service
  * @note   None
  * @param  
  * @retval None
 */
void ADC1_2_IRQHandler()
{
    if((ADC1->SR & (((uint32_t)0x01)<<0) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_1,AWD);
        if(p_adc1_awd_interrupt != NULL)
        {
            p_adc1_awd_interrupt();
        }
    }
    else if((ADC1->SR & (((uint32_t)0x01)<<1) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_1,EOC);
        if(p_adc1_eoc_interrupt != NULL)
        {
            p_adc1_eoc_interrupt();
        }
    }
    else
    {
        drv_adc_clear_it_flag(ADC_1,JEOC);
        if(p_adc1_jeoc_interrupt != NULL)
        {
            p_adc1_jeoc_interrupt();
        }
    }
    
    if((ADC2->SR & (((uint32_t)0x01)<<0) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_2,AWD);
        if(p_adc2_awd_interrupt != NULL)
        {
            p_adc2_awd_interrupt();
        }
    }
    else if((ADC2->SR & (((uint32_t)0x01)<<1) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_2,EOC);
        if(p_adc2_eoc_interrupt != NULL)
        {
            p_adc2_eoc_interrupt();
        }
    }
    else
    {
        drv_adc_clear_it_flag(ADC_2,JEOC);
        if(p_adc2_jeoc_interrupt != NULL)
        {
            p_adc2_jeoc_interrupt();
        }
    }
}


/**
  * @brief  adc3 interrupt service
  * @note   None
  * @param  
  * @retval None
 */
void ADC3_IRQHandler()
{
    
    if((ADC3->SR & (((uint32_t)0x01)<<0) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_3,AWD);
        if(p_adc3_awd_interrupt != NULL)
        {
            p_adc3_awd_interrupt();
        }
    }
    else if((ADC3->SR & (((uint32_t)0x01)<<1) )!= 0)
    {
        drv_adc_clear_it_flag(ADC_3,EOC);
        if(p_adc3_eoc_interrupt != NULL)
        {
            p_adc3_eoc_interrupt();
        }
    }
    else
    {
        drv_adc_clear_it_flag(ADC_3,JEOC);
        if(p_adc3_jeoc_interrupt != NULL)
        {
            p_adc3_jeoc_interrupt();
        }
    }
}

