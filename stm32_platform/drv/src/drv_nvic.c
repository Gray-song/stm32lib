/**
  ******************************************************************************
  * @file    drv_nvic.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_nvic.h"


/**
  * @brief nvic init 
  * @note   None
  * @param  
  * @retval None
 */
void drv_nvic_init(nvic_init_t *nvic_init_struct)
{
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
    if (nvic_init_struct->NVIC_IRQChannelCmd != DISABLE_DEF)
    {
        /* Compute the Corresponding IRQ Priority --------------------------------*/    
        tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
        tmppre = (0x4 - tmppriority);
        tmpsub = tmpsub >> tmppriority;

        tmppriority = (uint32_t)nvic_init_struct->NVIC_IRQChannelPreemptionPriority << tmppre;
        tmppriority |=  nvic_init_struct->NVIC_IRQChannelSubPriority & tmpsub;
        tmppriority = tmppriority << 0x04;

        NVIC->IP[nvic_init_struct->NVIC_IRQChannel] = tmppriority;

        /* Enable the Selected IRQ Channels --------------------------------------*/
        NVIC->ISER[nvic_init_struct->NVIC_IRQChannel >> 0x05] =
        (uint32_t)0x01 << (nvic_init_struct->NVIC_IRQChannel & (uint8_t)0x1F);
    }
    else
    {
        /* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC->ICER[nvic_init_struct->NVIC_IRQChannel >> 0x05] =
        (uint32_t)0x01 << (nvic_init_struct->NVIC_IRQChannel & (uint8_t)0x1F);
    }
}


/**
  * @brief nvic priority_group config
  * @note   None
  * @param  
  * @retval None
 */
void drv_nvic_priority_group_config(nvic_priority_group_t nvic_priority_group)
{
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = (uint32_t)0x05FA0000 | nvic_priority_group;
}

