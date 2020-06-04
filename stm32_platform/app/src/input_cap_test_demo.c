/**
  ******************************************************************************
  * @file    input_cap_test_demo.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "headers.h"
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
void send_pwm_info(void)
{
    uint32_t high_time =0;
    uint32_t period = 0;
    uint8_t duty_cycle = 0 ;
    uint8_t send_buff[11]={0};
    static uint8_t i = 0;
    period = hal_get_pwm_period();
    high_time = hal_get_high_time();
    duty_cycle = (uint8_t)(((float)high_time/period)*100);
    if(i>SEND_PERIOD_50MS)
    {
        send_buff[0] = 0xfb;
        send_buff[1] = (uint8_t)((0xff000000&high_time)>>24);
        send_buff[2] = (uint8_t)((0x00ff0000&high_time)>>16);
        send_buff[3] = (uint8_t)((0x0000ff00&high_time)>>8);
        send_buff[4] = (uint8_t)((0x000000ff&high_time)>>0);
        send_buff[5] = (uint8_t)((0xff000000&period)>>24);
        send_buff[6] = (uint8_t)((0x00ff0000&period)>>16);
        send_buff[7] = (uint8_t)((0x0000ff00&period)>>8);
        send_buff[8] = (uint8_t)((0x000000ff&period)>>0);
        send_buff[9] = duty_cycle;
        hal_usart_send(USART_1,send_buff,11);
        i = 0;
    }
    else
    {
        i++;
    }
    
    
    
}
