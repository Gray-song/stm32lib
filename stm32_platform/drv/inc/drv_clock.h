/**
  ******************************************************************************
  * @file    drv_clock.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
 */

#ifndef _DRV_CLOCK_H_
#define _DRV_CLOCK_H_

#include "stm32f103rc.h"
#define CLOCK_READY_SUCCESS 0x01
#define CLOCK_READY_FAILED 0x00

#define VECT_TAB_OFFSET  0x0


#define MAX_SYS_CLOCK_FREQ 6


#define SYS_CLK_FREQ_CONFIG \
/*SYSCLK_FREQ_24MHz*/ set_sys_clk_to_24M,\
/*SYSCLK_FREQ_36MHz*/ set_sys_clk_to_36M,\
/*SYSCLK_FREQ_48MHz*/ set_sys_clk_to_48M,\
/*SYSCLK_FREQ_56MHz*/ set_sys_clk_to_56M,\
/*SYSCLK_FREQ_64MHz*/ set_sys_clk_to_64M,\
/*SYSCLK_FREQ_72MHz*/ set_sys_clk_to_72M

//extern uint32_t g_sys_clock;
typedef enum
{
    HSE_SYS = 0,
    HSI_SYS,
    PLL_SYS,
}sys_clk_source_t;


typedef enum
{
    HSE_8M = 0,
    HSI_DIV2_4M,
}pll_clk_source_t;

typedef enum
{
    SYSCLK_FREQ_24MHz = 0,
    SYSCLK_FREQ_36MHz,
    SYSCLK_FREQ_48MHz,
    SYSCLK_FREQ_56MHz,
    SYSCLK_FREQ_64MHz,
    SYSCLK_FREQ_72MHz,
}sys_clock_freq_t;

typedef struct
{
    sys_clk_source_t sys_clk_source;
    pll_clk_source_t pll_clk_source;
    sys_clock_freq_t sys_clock_freq;

}clk_init_t;

typedef struct
{
    uint32_t system_clk_freq ;
    uint32_t hclk_clk_freq ;
    uint32_t pclk1_clk_freq ;
    uint32_t pclk2_clk_freq ;
    uint32_t adcclk_clk_freq ;

}sys_clk_freq_info_t;

typedef void (*p_set_clk_func_t)(pll_clk_source_t pll_clk);
extern void drv_clock_init(const clk_init_t* clk_init_structure);
extern void drv_get_clock_freq_info(sys_clk_freq_info_t* sys_clk_freq_info);
extern void drv_rcc_register_reset(void);
#endif 
