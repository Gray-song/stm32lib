/**
  ******************************************************************************
  * @file    drv_nvic.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_NVIC_H_
#define _DRV_NVIC_H_
#include "stm32f103rc.h"

typedef struct
{
    uint8_t NVIC_IRQChannel;                    
    uint8_t NVIC_IRQChannelPreemptionPriority;  
    uint8_t NVIC_IRQChannelSubPriority;          
    bool NVIC_IRQChannelCmd;    
}nvic_init_t;

typedef enum
{
    NVIC_PriorityGroup_0 = (uint32_t)0x700,  /*cut from bit 7, 0 bit pre-emption priority , 4 bit subpriority priority*/
    NVIC_PriorityGroup_1 = (uint32_t)0x600,  /*cut from bit 6, 1 bit pre-emption priority , 3 bit subpriority priority*/
    NVIC_PriorityGroup_2 = (uint32_t)0x500,  /*cut from bit 5, 2 bit pre-emption priority , 2 bit subpriority priority*/
    NVIC_PriorityGroup_3 = (uint32_t)0x400,  /*cut from bit 4, 3 bit pre-emption priority , 1 bit subpriority priority*/
    NVIC_PriorityGroup_4 = (uint32_t)0x300   /*cut from bit 3, 4 bit pre-emption priority , 0 bit subpriority priority*/    
}nvic_priority_group_t;


extern void drv_nvic_init(nvic_init_t *nvic_init_struct);
extern void drv_nvic_priority_group_config(nvic_priority_group_t nvic_priority_group);

#endif
