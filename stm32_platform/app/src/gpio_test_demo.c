/**
  ******************************************************************************
  * @file    gpio_test_demo.c
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
  * @brief  test_key
  * @note   GPIOA15 GPIOAC5 GPIOD2
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void test_key(void)
{
    hal_di_input();
    
    
    if(g_di_signal_key1 == FALSE)
    {
        g_do_signal_led1 = 0;
    }
    if(g_di_signal_key0 == FALSE)
    {
        g_do_signal_led1 = 1;
    }
    //hal_do_output();

}


/**
  * @brief  test_led
  * @note   GPIOA8
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void test_led()
{
    static uint8_t status = TRUE;
    if(status == TRUE)
    {
        g_do_signal_led0 = 0;
        hal_do_output();
        status = FALSE;
    }
    else
    {
        g_do_signal_led0 = 1;
        hal_do_output();
        status = TRUE;
    }
}

