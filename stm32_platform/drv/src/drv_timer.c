/**
  ******************************************************************************
  * @file    drv_timer.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_timer.h"

static volatile TIM_TypeDef*  const TIMER_ARRAY_MAP[TIMER_COUNT] = 
{
    TIM1,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    TIM6,
    TIM7,
    TIM8
};

/*if memorey is not enough,this array is can modify*/
static  timer_interrupt_fun_point_t p_callback_array[TIMER_COUNT] =
{
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
    {NULL,NULL,NULL,NULL,NULL},
};


/**
  * @brief timer init
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_init(timer_t timx,uint16_t psc, uint16_t load_value)
{
     /*enable clk*/
    if(timx == TIMER1)
    {
        RCC->APB2ENR |= (uint32_t)0x01 << 11;
    }
    else if(timx == TIMER8)
    {
        RCC->APB2ENR |= (uint32_t)0x01 << 13;
    }
    else
    {
        RCC->APB1ENR |= (uint32_t)0x01 << (timx-1);
    }
    
    TIMER_ARRAY_MAP[timx]->ARR = load_value;
    TIMER_ARRAY_MAP[timx]->PSC = psc;
}


/**
  * @brief timer enanle/disable
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_cmd(timer_t timx,bool status)
{
    if(status == ENABLE_DEF)
    {
        TIMER_ARRAY_MAP[timx]->CR1 |= (uint16_t)0x01;
    }
    else
    {
        TIMER_ARRAY_MAP[timx]->CR1 &= ~((uint16_t)0x01);
    }
    
}


/**
  * @brief timer interrupt enable/disable
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_it_config(timer_t timx,timer_it_source_t it_source,bool status)
{
    if(status == ENABLE_DEF)
    {
        TIMER_ARRAY_MAP[timx]->DIER |= (uint16_t)0x01 << it_source;
    }
    else
    {  
        TIMER_ARRAY_MAP[timx]->DIER &= ~((uint16_t)0x01 << it_source);
    
    }
}


/**
  * @brief timer interrupt config
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_isr_config(const timer_t timx,voidfunc_t p_callback,timer_it_source_t callback_type)
{
    if(callback_type == TIM_INTERRUPT_OVER_FLOW)
    {
        if(p_callback != NULL)
        {
            p_callback_array[timx].p_tim_overflow_interrupt = p_callback;
        }       
    }
    else if(callback_type == TIM_INTERRUPT_CC1)
    {
        if(p_callback != NULL)
        {
            p_callback_array[timx].p_tim_cc1_interrupt = p_callback;
        }  
    }
    else if(callback_type == TIM_INTERRUPT_CC2)
    {
        if(p_callback != NULL)
        {
            p_callback_array[timx].p_tim_cc2_interrupt = p_callback;
        }
    }
    else if(callback_type == TIM_INTERRUPT_CC3)
    {
        if(p_callback != NULL)
        {
            p_callback_array[timx].p_tim_cc3_interrupt = p_callback;
        }
    
    }
    else if(callback_type == TIM_INTERRUPT_CC4)
    {
        if(p_callback != NULL)
        {
            p_callback_array[timx].p_tim_cc4_interrupt = p_callback;
        }
    }
}


/**
  * @brief get timer conter value
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_timer_conter_get(const timer_t timx)
{
	return TIMER_ARRAY_MAP[timx]->CNT;
}


/**
  * @brief  timer input capture
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_cap_init(timer_t timx,timer_cap_init_t timer_cap_init_struct )
{
    drv_timer_init(timx,timer_cap_init_struct.psc,timer_cap_init_struct.load_value);
    if(timer_cap_init_struct.channel == channel1)
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0003);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= (uint16_t)timer_cap_init_struct.input_cap_selection;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x00f0) ;
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_cap_init_struct.filter_prama)<<4;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x000c) ;
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_cap_init_struct.input_div)<<2;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0002);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)timer_cap_init_struct.edge)<<1;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0001);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<0;
    }
    else if(timer_cap_init_struct.channel == channel2)
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0300);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_cap_init_struct.input_cap_selection) << 8;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0xf000);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_cap_init_struct.filter_prama)<<12;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0c00);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_cap_init_struct.input_div)<<10;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0020);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)timer_cap_init_struct.edge)<<5;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0010);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<4;
    }
    else if(timer_cap_init_struct.channel == channel3)
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0003);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= (uint16_t)timer_cap_init_struct.input_cap_selection;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x00f0) ;
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_cap_init_struct.filter_prama)<<4;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x000c) ;
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_cap_init_struct.input_div)<<2;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0200);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)timer_cap_init_struct.edge)<<9;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0100);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<8;
        
    }
    else
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0300);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_cap_init_struct.input_cap_selection) << 8;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0xf000);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_cap_init_struct.filter_prama)<<12;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0c00);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_cap_init_struct.input_div)<<10;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x2000);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)timer_cap_init_struct.edge)<<13;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x1000);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<12;
    }
}


/**
  * @brief  set timer way of input capture trigger
  * @note   None
  * @param  
  * @retval None
 */
void drv_set_timer_cap_edge(timer_t timx,map_channel_t channel,cap_valid_edge_t edge)
{
    if(channel == channel1)
    {
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0002);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)edge)<<1;
    }
    else if(channel == channel2)
    {
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0020);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)edge)<<5;
    }
    else if(channel == channel3)
    {
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0200);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)edge)<<9;
    }
    else
    {
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x2000);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)edge)<<13;
    
    }
}


/**
  * @brief  get timer value of input capture trigger
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_get_timer_cap_value(timer_t timx,map_channel_t channel)
{
    if(channel == channel1)
    {
        return TIMER_ARRAY_MAP[timx]->CCR1;
    }
    else if(channel == channel2)
    {
        return TIMER_ARRAY_MAP[timx]->CCR2;
    
    }
    else if(channel == channel3)
    {
        return TIMER_ARRAY_MAP[timx]->CCR3;
    }
    else 
    {
        return TIMER_ARRAY_MAP[timx]->CCR4;
    }
}


/**
  * @brief  get timer value SR register of input capture trigger
  * @note   None
  * @param  
  * @retval None
 */
uint16_t drv_get_timer_sr_value(timer_t timx)
{
    return TIMER_ARRAY_MAP[timx]->SR;
}


/**
  * @brief  pwm init
  * @note   None
  * @param  
  * @retval None
 */
void drv_timer_pwm_init(timer_t timx,timer_pwm_init_t timer_pwm_init_struct)
{
    drv_timer_init(timx,timer_pwm_init_struct.psc,timer_pwm_init_struct.load_value);
    if(timer_pwm_init_struct.channel == channel1)
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0070);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)timer_pwm_init_struct.oc_mode)<<4;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0008);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)0x01) << 3;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0002);
        TIMER_ARRAY_MAP[timx]->CCER |= (uint16_t)timer_pwm_init_struct.pwm_valid_level << 1;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0001);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<0;
        
        
    }
    else if(timer_pwm_init_struct.channel == channel2)
    {
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x7000);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= timer_pwm_init_struct.oc_mode<<12;
        TIMER_ARRAY_MAP[timx]->CCMR1 &= ~((uint16_t)0x0800);
        TIMER_ARRAY_MAP[timx]->CCMR1 |= ((uint16_t)0x01) << 11;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0020);
        TIMER_ARRAY_MAP[timx]->CCER |= (uint16_t)timer_pwm_init_struct.pwm_valid_level << 5;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0010);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<4;
        
    }
    else if(timer_pwm_init_struct.channel == channel3)
    {
        TIMER_ARRAY_MAP[timx]->CCMR2 &= ~((uint16_t)0x0070);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_pwm_init_struct.oc_mode)<<4;
        TIMER_ARRAY_MAP[timx]->CCMR2 &= ~((uint16_t)0x0008);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)0x01) << 3;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0200);
        TIMER_ARRAY_MAP[timx]->CCER |= (uint16_t)timer_pwm_init_struct.pwm_valid_level << 9;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x0100);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<8;
        
    }
    else
    {
        TIMER_ARRAY_MAP[timx]->CCMR2 &= ~((uint16_t)0x7000);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)timer_pwm_init_struct.oc_mode)<<12;
        TIMER_ARRAY_MAP[timx]->CCMR2 &= ~((uint16_t)0x0800);
        TIMER_ARRAY_MAP[timx]->CCMR2 |= ((uint16_t)0x01) << 11;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x2000);
        TIMER_ARRAY_MAP[timx]->CCER |= (uint16_t)timer_pwm_init_struct.pwm_valid_level << 13;
        TIMER_ARRAY_MAP[timx]->CCER &= ~((uint16_t)0x1000);
        TIMER_ARRAY_MAP[timx]->CCER  |= ((uint16_t)0x01)<<12;
        
    }
    TIMER_ARRAY_MAP[timx]->CR1 &= ~((uint16_t)0x0080);
    TIMER_ARRAY_MAP[timx]->CR1 |= ((uint16_t)0x0080);
    if((timx == TIMER1) || (timx == TIMER8))
    {
        TIMER_ARRAY_MAP[timx]->BDTR &= ~(((uint16_t)0x01)<<15);
        TIMER_ARRAY_MAP[timx]->BDTR |= (((uint16_t)0x01)<<15);
    }
}


/**
  * @brief  set pwm duty cycle 
  * @note   None
  * @param  
  * @retval None
 */
void drv_set_duty_cycle(timer_t timx,map_channel_t channel,uint8_t percent)
{
    
    uint16_t arr = 0;
    uint16_t duty_cycle = 0;
    arr = TIMER_ARRAY_MAP[timx]->ARR;
    duty_cycle = ((arr+1) * percent) /100;
    if(channel == channel1)
    {
        TIMER_ARRAY_MAP[timx]->CCR1 = duty_cycle;
    }
    else if(channel == channel2)
    {
        TIMER_ARRAY_MAP[timx]->CCR2 = duty_cycle;
    }
    else if(channel == channel3)
    {
        TIMER_ARRAY_MAP[timx]->CCR3 = duty_cycle;
    }
    else
    {
        TIMER_ARRAY_MAP[timx]->CCR4 = duty_cycle;
    }  
    
}



/**
  * @brief  TIM1 UP overflow interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM1_UP_IRQHandler()
{
    if(p_callback_array[TIMER1].p_tim_overflow_interrupt != NULL)
    {
        p_callback_array[TIMER1].p_tim_overflow_interrupt();
    }
    TIM1->SR &= ~((uint16_t)0x01 << 0);
}


/**
  * @brief  TIM1 CC interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM1_CC_IRQHandler()
{
    if((TIM1->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER1].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER1].p_tim_cc1_interrupt();
        }
        TIM1->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM1->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER1].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER1].p_tim_cc2_interrupt();
        }
        TIM1->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM1->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER1].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER1].p_tim_cc3_interrupt();
        }
        TIM1->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM1->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER1].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER1].p_tim_cc4_interrupt();
        }
        TIM1->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM2 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM2_IRQHandler()
{
    if((TIM2->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER2].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER2].p_tim_overflow_interrupt();
        }
        TIM2->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM2->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER2].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER2].p_tim_cc1_interrupt();
        }
        TIM2->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM2->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER2].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER2].p_tim_cc2_interrupt();
        }
        TIM2->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM2->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER2].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER2].p_tim_cc3_interrupt();
        }
        TIM2->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM2->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER2].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER2].p_tim_cc4_interrupt();
        }
        TIM2->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM3 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM3_IRQHandler()
{
    if((TIM3->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER3].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER3].p_tim_overflow_interrupt();
        }
        TIM3->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM3->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER3].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER3].p_tim_cc1_interrupt();
        }
        TIM3->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM3->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER3].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER3].p_tim_cc2_interrupt();
        }
        TIM3->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM3->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER3].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER3].p_tim_cc3_interrupt();
        }
        TIM3->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM3->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER3].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER3].p_tim_cc4_interrupt();
        }
        TIM3->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM4 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM4_IRQHandler()
{
    if((TIM4->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER4].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER4].p_tim_overflow_interrupt();
        }
        TIM4->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM4->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER4].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER4].p_tim_cc1_interrupt();
        }
        TIM4->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM4->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER4].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER4].p_tim_cc2_interrupt();
        }
        TIM4->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM4->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER4].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER4].p_tim_cc3_interrupt();
        }
        TIM4->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM4->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER4].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER4].p_tim_cc4_interrupt();
        }
        TIM4->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM5 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM5_IRQHandler()
{
    if((TIM5->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER5].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER5].p_tim_overflow_interrupt();
        }
        TIM5->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM5->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER5].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER5].p_tim_cc1_interrupt();
        }
        TIM5->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM5->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER5].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER5].p_tim_cc2_interrupt();
        }
        TIM5->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM5->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER5].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER5].p_tim_cc3_interrupt();
        }
        TIM5->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM5->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER5].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER5].p_tim_cc4_interrupt();
        }
        TIM5->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM6 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM6_IRQHandler()
{
    if((TIM6->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER6].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER6].p_tim_overflow_interrupt();
        }
        TIM6->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM6->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER6].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER6].p_tim_cc1_interrupt();
        }
        TIM6->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM6->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER6].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER6].p_tim_cc2_interrupt();
        }
        TIM6->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM6->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER6].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER6].p_tim_cc3_interrupt();
        }
        TIM6->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM6->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER6].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER6].p_tim_cc4_interrupt();
        }
        TIM6->SR &= ~((uint16_t)0x01 << 4);
    }
   
}


/**
  * @brief  TIM7 interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM7_IRQHandler()
{
    if((TIM7->SR & (uint16_t)0x0001) != 0)
    {
        if(p_callback_array[TIMER7].p_tim_overflow_interrupt != NULL)
        {
            p_callback_array[TIMER7].p_tim_overflow_interrupt();
        }
        TIM7->SR &= ~((uint16_t)0x01 << 0);
    }
    
    if((TIM2->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER7].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER7].p_tim_cc1_interrupt();
        }
        TIM7->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM7->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER7].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER7].p_tim_cc2_interrupt();
        }
        TIM7->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM7->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER7].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER7].p_tim_cc3_interrupt();
        }
        TIM7->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM7->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER7].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER7].p_tim_cc4_interrupt();
        }
        TIM7->SR &= ~((uint16_t)0x01 << 4);
    }
    
}


/**
  * @brief  TIM8 UP overflow interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM8_UP_IRQHandler()
{
    if(p_callback_array[TIMER8].p_tim_overflow_interrupt != NULL)
    {
        p_callback_array[TIMER8].p_tim_overflow_interrupt();
    }
    TIM8->SR &= ~((uint16_t)0x01 << 0);
}


/**
  * @brief  TIM8 CC interrupt service 
  * @note   None
  * @param  
  * @retval None
 */
void TIM8_CC_IRQHandler()
{
    if((TIM8->SR & (uint16_t)0x0002) != 0)
    {
        if(p_callback_array[TIMER8].p_tim_cc1_interrupt != NULL)
        {
            p_callback_array[TIMER8].p_tim_cc1_interrupt();
        }
        TIM8->SR &= ~((uint16_t)0x01 << 1);
    }
    
    if((TIM8->SR & (uint16_t)0x0004) != 0)
    {
        if(p_callback_array[TIMER8].p_tim_cc2_interrupt != NULL)
        {
            p_callback_array[TIMER8].p_tim_cc2_interrupt();
        }
        TIM8->SR &= ~((uint16_t)0x01 << 2);
    }
    
    if((TIM8->SR & (uint16_t)0x0008) != 0)
    {
        if(p_callback_array[TIMER8].p_tim_cc3_interrupt != NULL)
        {
            p_callback_array[TIMER8].p_tim_cc3_interrupt();
        }
        TIM8->SR &= ~((uint16_t)0x01 << 3);
    }
    
    if((TIM8->SR & (uint16_t)0x0010) != 0)
    {
        if(p_callback_array[TIMER8].p_tim_cc4_interrupt != NULL)
        {
            p_callback_array[TIMER8].p_tim_cc4_interrupt();
        }
        TIM8->SR &= ~((uint16_t)0x01 << 4);
    }
   
}



