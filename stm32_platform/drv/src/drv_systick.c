/**
  ******************************************************************************
  * @file    drv_systick.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_systick.h"

static u8  s_sys_tick_us=0;
static u16 s_sys_tick_ms=0;


/**
  * @brief  Systick Clk  Config
  * @note   None
  * @param  
  * @retval None
 */
void drv_systick_clk_source_config(uint32_t systick_clk_source)
{
    
    if (systick_clk_source == SYSTICK_CLK_SOURCE_HCLK)
    {
        SysTick->CTRL |= SYSTICK_CLK_SOURCE_HCLK;
    }
    else
    {
        SysTick->CTRL &= SYSTICK_CLK_SOURCE_HCLK_DIV8;
    }
}


/**
  * @brief  delay init
  * @note   None
  * @param  sys_clk_freq
  * @retval None
 */
void drv_delay_init(uint32_t sys_clk_freq)
{
    drv_systick_clk_source_config(SYSTICK_CLK_SOURCE_HCLK_DIV8);	//systick clk freq = 72000000/8 = 9000000
	s_sys_tick_us = sys_clk_freq/8000000;		    
	s_sys_tick_ms = (u16)s_sys_tick_us*1000;
}


/**
  * @brief  ms delay 
  * @note   None
  * @param  
  * @retval None
 */
void drv_delay_ms(uint16_t ms)
{
    u32 temp;		   
	SysTick->LOAD=(u32)ms*s_sys_tick_ms;
	SysTick->VAL =0x00;          
	SysTick->CTRL=0x01 ;          
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); 
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00; 
}


/**
  * @brief  us delay 
  * @note   None
  * @param  
  * @retval None
 */
void drv_delay_us(uint32_t us)
{
    u32 temp;	    	 
	SysTick->LOAD=us*s_sys_tick_us;   		 
	SysTick->VAL=0x00;        
	SysTick->CTRL=0x01 ;      
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       
}
