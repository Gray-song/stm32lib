/**
  ******************************************************************************
  * @file    drv_dac.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_dac.h"

/**
  * @brief  dac init
  * @note   None
  * @param  
  * @retval None
 */
 void drv_dac_init(dac_channel_t channel, dac_init_t dac_init_struct)
 {
     uint32_t temp = 0;
     
     RCC->APB1ENR |= ((uint32_t)0x01)<< 29;
     temp = (dac_init_struct.dac_dma | dac_init_struct.dac_LFSR_unmask_triangle_amplitude
             | dac_init_struct.dac_output_buffer | dac_init_struct.dac_trigger
             | dac_init_struct.dac_wave_generation);
     if(channel == DAC_CHANNEL_1)
     {
         DAC->CR &=~((uint32_t)0x00001fff);
         DAC->CR |= temp;
     }
     else
     {
         DAC->CR &=~(((uint32_t)0x00001fff) << 16);
         DAC->CR |= temp << 16;
     }
 }
 
 
 /**
  * @brief  dac enable/disable
  * @note   None
  * @param  
  * @retval None
 */
 void drv_dac_cmd(dac_channel_t channel,bool status)
 {
     if(status == ENABLE_DEF)
     {
         if(channel == DAC_CHANNEL_1)
         {
             DAC->CR &= ~ ((uint32_t)0x01);
             DAC->CR |= ((uint32_t)0x01);
         }
         else
         {
             DAC->CR |= (((uint32_t)0x01)<<16);
         }
         
         
     }
     else
     {
         if(channel == DAC_CHANNEL_1)
         {
             DAC->CR &= ~ ((uint32_t)0x01);
             
         }
         else
         {
             DAC->CR &= ~ (((uint32_t)0x01)<<16);
         }
     }
 }
 

 /**
  * @brief  set voltage value
  * @note   None
  * @param  
  * @retval None
 */
void drv_dac_set_vol(dac_channel_t channel,dac_mode_t mode,uint16_t vol)
{
     
    if(channel == DAC_CHANNEL_1)
    {
        if(mode == DAC_ALIGN_12_R)
        {
            DAC->DHR12R1 = vol;
        }
        else if(mode == DAC_ALIGN_12_L)
        {
            DAC->DHR12L1 = vol;
        }
        else
        {
            DAC->DHR8R1 = vol;
        }
    }
    else
    {
        if(mode == DAC_ALIGN_12_R)
        {
            DAC->DHR12R2 = vol;
        }
        else if(mode == DAC_ALIGN_12_L)
        {
            DAC->DHR12L2 = vol;
        }
        else
        {
            DAC->DHR8R2 = vol;
        }
    }
}







