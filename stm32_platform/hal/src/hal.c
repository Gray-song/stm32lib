/**
  ******************************************************************************
  * @file    hal.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal.h"
sys_clk_freq_info_t g_sys_clock_info;


/**
  * @brief  hal init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_init(void)
{
      
    hal_clock_init();
    hal_watchdog_init(1000);
    hal_gpio_init();
    hal_get_clock_freq_info(&g_sys_clock_info);
    hal_delay_init(g_sys_clock_info.system_clk_freq);
    hal_timer_init();
    hal_timer_cap_init();
    hal_timer_pwm_init();
    hal_adc_init();
    hal_dac_init();
    hal_nvic_init();
   // hal_gpio_exti_isr_config(GPIO_Pin_5,hal_exti5_isr);
   // hal_gpio_exti_config(GPIO_C,GPIO_Pin_5,EXTI_ModeInterrupt,EXTI_Trigger_Falling,ENABLE_DEF);
    hal_simulation_iic_init();
    hal_usart_init(USART_1);
}
