/**
  ******************************************************************************
  * @file    hal_dac.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_dac.h"
#include "platform_config.h"


/**
  * @brief  hal_dac_init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_dac_init(void)
{
    dac_init_t dac_init_struct;
    dac_init_struct.dac_dma = DAC_DMA_DISABLE;
    dac_init_struct.dac_LFSR_unmask_triangle_amplitude = DAC_LFSR_0_TRIANGLE_1;
    dac_init_struct.dac_output_buffer = DAC_OUTPUT_BUFF_DISABLE;
    dac_init_struct.dac_trigger = DAC_TRIG_NONE;
    dac_init_struct.dac_wave_generation = DAC_WAVE_NONE;
    drv_dac_init(DAC_CHANNEL_1,dac_init_struct);
    drv_dac_cmd(DAC_CHANNEL_1,ENABLE_DEF);
    drv_dac_set_vol(DAC_CHANNEL_1,DAC_ALIGN_12_R,0);
}


/**
  * @brief  hal_set_vol_for_12b
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  * vol is real*1000
  ******************************************************************************
 */
void hal_set_vol_for_12b(uint16_t vol)
{
    float temp=vol;
	temp/=1000;
	temp=temp*4096/DAC_MAX_VOLTAGE;
    drv_dac_set_vol(DAC_CHANNEL_1,DAC_ALIGN_12_R,temp);
}


/**
  * @brief  hal_set_vol_for_8b
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  * vol is real*1000
  ******************************************************************************
 */
void hal_set_vol_for_8b(uint16_t vol)
{
    float temp=vol;
	temp/=1000;
	temp=temp*256/DAC_MAX_VOLTAGE;
    drv_dac_set_vol(DAC_CHANNEL_1,DAC_ALIGN_8_R,temp);
}
