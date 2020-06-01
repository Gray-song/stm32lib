/**
  ******************************************************************************
  * @file    drv_iwdg.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_iwdg.h"


/**
  * @brief iwdg init
  * @note   None
  * @param  
  * @retval None
 */
void drv_iwdg_init(uint16_t ms)
{
    uint16_t reload_value=0;
    IWDG->KR = KR_KEY_WriteAccess_Enable;
    if(ms < 410)
    {
        /*div 4*/
        IWDG->PR = IWDG_Prescaler_4;
        reload_value = (ms*40)/4;
        
    }
    else if((ms >= 410)&&(ms < 820))
    {
        /*div 8*/
        IWDG->PR = IWDG_Prescaler_8;
        reload_value = (ms*40)/8;
    }
    else if((ms >= 820)&&(ms < 1639))
    {
        /*div 16*/
        IWDG->PR = IWDG_Prescaler_16;
        reload_value = (ms*40)/16;
    }
    else if((ms >= 1639)&&(ms < 3277))
    {
        /*div 32*/
        IWDG->PR = IWDG_Prescaler_32;
        reload_value = (ms*40)/32;
    }
    else if((ms >= 3277)&&(ms < 6554))
    {
        /*div 64*/
        IWDG->PR = IWDG_Prescaler_64;
        reload_value = (ms*40)/64;
    }
    else if((ms >= 6554)&&(ms < 13108))
    {
        /*div 128*/
        IWDG->PR = IWDG_Prescaler_128;
        reload_value = (ms*40)/128;
    }
    else
    {
        /*div 256*/
        IWDG->PR = IWDG_Prescaler_256;
        reload_value = (ms*40)/256;
    }
    IWDG->RLR = reload_value;
    IWDG->KR = KR_KEY_Reload;
    IWDG->KR = KR_KEY_Enable;
}



/**
  * @brief feed iwdg 
  * @note   None
  * @param  
  * @retval None
 */
void drv_iwdg_feed(void)
{
    IWDG->KR = KR_KEY_Reload;
}
