/**
  ******************************************************************************
  * @file    hal_clock.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/

#include "hal_clock.h"


/**
  * @brief  Initialize sys clock
  * @note   
  * @param  clk_init_structure
  * @retval None
  ******************************************************************************
  * @attention
  *
  * When sys_clock_source is PLL and pll_clock_source is HSI_DIV2_PLL, the real
  * of sys_clock_freq is 64Mhz.
  * the fouction  achieve only hse_freq is 8Mhz.
  ******************************************************************************
 */
void hal_clock_init(void)
{
    clk_init_t clk_init_struct;

    clk_init_struct.pll_clk_source = HSE_8M;
    clk_init_struct.sys_clk_source = PLL_SYS;
    clk_init_struct.sys_clock_freq = SYSCLK_FREQ_72MHz;
    drv_clock_init(&clk_init_struct);
}


/**
  * @brief  hal_get_clock_freq_info
  * @note   
  * @param  clk_init_structure
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void  hal_get_clock_freq_info(sys_clk_freq_info_t *sys_clk_freq_info) 
{
    drv_get_clock_freq_info(sys_clk_freq_info);
}
