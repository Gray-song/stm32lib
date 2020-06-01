/**
  ******************************************************************************
  * @file    hal_nvic.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   This file provides all the RTC firmware functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_nvic.h"
static const nvic_init_t nvic_config_array[NVIC_IRQ_COUNT] =
{
    NVIC_IRQ_CONFIG
};


/**
  * @brief  hal_nvic_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_nvic_init(void)
{
    uint8_t i;
    nvic_init_t nvic_init_struct;
    drv_nvic_priority_group_config(NVIC_PriorityGroup_2);
    for(i=0; i<NVIC_IRQ_COUNT; i++)
    {
        nvic_init_struct.NVIC_IRQChannel = nvic_config_array[i].NVIC_IRQChannel;
        nvic_init_struct.NVIC_IRQChannelCmd = nvic_config_array[i].NVIC_IRQChannelCmd;
        nvic_init_struct.NVIC_IRQChannelPreemptionPriority = nvic_config_array[i].NVIC_IRQChannelPreemptionPriority;
        nvic_init_struct.NVIC_IRQChannelSubPriority = nvic_config_array[i].NVIC_IRQChannelSubPriority;
        drv_nvic_init(&nvic_init_struct);
    }
}
