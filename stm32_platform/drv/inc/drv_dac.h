/**
  ******************************************************************************
  * @file    drv_dac.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_DAC_H_
#define _DRV_DAC_H_
#include "stm32f103rc.h"

#define DAC_TRIG_NONE                   ((uint32_t)0x00000000)
#define DAC_TRIG_T6_TRGO                ((uint32_t)0x00000004) 
#define DAC_TRIG_T8_TRGO                ((uint32_t)0x0000000C) 
#define DAC_TRIG_T7_TRGO                ((uint32_t)0x00000014)
#define DAC_TRIG_T5_TRGO                ((uint32_t)0x0000001C) 
#define DAC_TRIG_T2_TRGO                ((uint32_t)0x00000024) 
#define DAC_TRIG_T4_TRGO                ((uint32_t)0x0000002C) 
#define DAC_TRIG_Ext_IT9                ((uint32_t)0x00000034)
#define DAC_TRIG_SOFTWARE               ((uint32_t)0x0000003C)

#define DAC_WAVE_NONE           ((uint32_t)0x00000000)
#define DAC_WAVE_NOISE           ((uint32_t)0x00000040)
#define DAC_WAVE_TRIANGLE        ((uint32_t)0x00000080)

#define DAC_LFSR_0_TRIANGLE_1                ((uint32_t)0x00000000)
#define DAC_LFSR_1_0_TRIANGLE_3             ((uint32_t)0x00000100) 
#define DAC_LFSR_2_0_TRIANGLE_7             ((uint32_t)0x00000200)
#define DAC_LFSR_3_0_TRIANGLE_15             ((uint32_t)0x00000300)
#define DAC_LFSR_4_0_TRIANGLE_31             ((uint32_t)0x00000400)
#define DAC_LFSR_5_0_TRIANGLE_63             ((uint32_t)0x00000500) 
#define DAC_LFSR_6_0_TRIANGLE_127             ((uint32_t)0x00000600)
#define DAC_LFSR_7_0_TRIANGLE_255             ((uint32_t)0x00000700)
#define DAC_LFSR_8_0_TRIANGLE_511             ((uint32_t)0x00000800) 
#define DAC_LFSR_9_0_TRIANGLE_1023             ((uint32_t)0x00000900)
#define DAC_LFSR_10_0_TRIANGLE_2047            ((uint32_t)0x00000A00) 
#define DAC_LFSR_11_0_TRIANGLE_4095            ((uint32_t)0x00000B00)

#define DAC_OUTPUT_BUFF_ENABLE            ((uint32_t)0x00000000)
#define DAC_OUTPUT_BUFF_DISABLE           ((uint32_t)0x00000002)

#define DAC_DMA_ENABLE            ((uint32_t)0x00000000)
#define DAC_DMA_DISABLE           ((uint32_t)0x00001000)

typedef enum
{
    DAC_CHANNEL_1 = 0,
    DAC_CHANNEL_2
}dac_channel_t;

typedef enum
{
    DAC_ALIGN_12_R = 0,
    DAC_ALIGN_12_L,
    DAC_ALIGN_8_R
}dac_mode_t;

typedef struct
{
    uint32_t dac_trigger;                      
    uint32_t dac_wave_generation;
    uint32_t dac_LFSR_unmask_triangle_amplitude; 
    uint32_t dac_output_buffer;
    uint32_t dac_dma;    

}dac_init_t;
extern void drv_dac_init(dac_channel_t channel, dac_init_t dac_init_struct);
extern void drv_dac_set_vol(dac_channel_t channel,dac_mode_t mode,uint16_t vol);
extern void drv_dac_cmd(dac_channel_t channel,bool status);
#endif
