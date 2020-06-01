/**
  ******************************************************************************
  * @file    hal_timer.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_timer.h"
#include "hal_gpio.h"


/**
  * @brief  hal_timer_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_timer_init()
{
    drv_timer_init(TIMER3,7200-1,5000-1); /*500ms*/
    drv_timer_it_config(TIMER3,TIM_INTERRUPT_OVER_FLOW,ENABLE_DEF);
    drv_timer_isr_config(TIMER3,hal_tim3_over_flow_isr,TIM_INTERRUPT_OVER_FLOW);
    drv_timer_cmd(TIMER3,ENABLE_DEF);
}


/**
  * @brief  hal_tim3_over_flow_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_tim3_over_flow_isr()
{
    g_do_signal_led1 = !g_do_signal_led1;
    hal_do_output();
}


/**
  * @brief  hal_timer_cap_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_timer_cap_init(void)
{
    timer_cap_init_t timer_cap_init_struct;
    timer_cap_init_struct.channel = channel1;
    timer_cap_init_struct.edge = RISING;
    timer_cap_init_struct.filter_prama = SAMPLING_CK_INT_N_2;
    timer_cap_init_struct.input_cap_selection = IC_DIREACT_TI;
    timer_cap_init_struct.input_div = IC_NO_DIV;
    timer_cap_init_struct.psc = 72-1;
    timer_cap_init_struct.load_value = 65535;
    drv_timer_cap_init(TIMER2,timer_cap_init_struct);
    drv_timer_it_config(TIMER2,TIM_INTERRUPT_CC1,ENABLE_DEF);
    drv_timer_isr_config(TIMER2,hal_tim2_cc1_isr,TIM_INTERRUPT_CC1);
    drv_timer_cmd(TIMER2,ENABLE_DEF);

}


/**
  * @brief  hal_tim2_cc1_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_tim2_cc1_isr(void)
{
    static bool status = FALSE;
    static uint32_t over_flow_count = 0;
    uint16_t tem_up = 0;
    uint16_t tem_fall = 0;
    static uint32_t result = 0;
    if(status == FALSE)
    {
        tem_up = drv_get_timer_cap_value(TIMER2,channel1);
        status = TRUE;
        drv_set_timer_cap_edge(TIMER2,channel1,FALLING);
        if((drv_get_timer_sr_value(TIMER2) &((uint16_t )0x01)) != 0 )
        {
            over_flow_count++;
        }
    }
    else
    {
        tem_fall = drv_get_timer_cap_value(TIMER2,channel1);
        status = FALSE;
        drv_set_timer_cap_edge(TIMER2,channel1,RISING);
        if(tem_fall > tem_up)
        {
            result =  (tem_fall-tem_up) + (over_flow_count*65535);
        }
        else
        {
            result = ((65535 - tem_up) + tem_fall)+((over_flow_count-1)*65535);
        
        }
        over_flow_count =0;
    }
}


/**
  * @brief  hal_timer_pwm_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_timer_pwm_init(void)
{
    timer_pwm_init_t timer_pwm_init_struct;
    timer_pwm_init_struct.channel = channel1;
    timer_pwm_init_struct.load_value = 899;
    timer_pwm_init_struct.psc = 0;
    timer_pwm_init_struct.oc_mode = PWM_MODE_2;
    timer_pwm_init_struct.pwm_valid_level = HIGH_VALID;
    drv_timer_pwm_init(TIMER1,timer_pwm_init_struct);
    drv_timer_cmd(TIMER1,ENABLE_DEF);
    drv_set_duty_cycle(TIMER1,channel1,15);
}

