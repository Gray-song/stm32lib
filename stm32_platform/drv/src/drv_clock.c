/**
  ******************************************************************************
  * @file    drv_clock.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/

#include "drv_clock.h"

static void set_sys_clk_to_HSE(void);
static void set_sys_clk_to_HSI(void);
static void set_sys_clk_to_24M(pll_clk_source_t pll_clk);
static void set_sys_clk_to_36M(pll_clk_source_t pll_clk);
static void set_sys_clk_to_48M(pll_clk_source_t pll_clk);
static void set_sys_clk_to_56M(pll_clk_source_t pll_clk);
static void set_sys_clk_to_64M(pll_clk_source_t pll_clk);
static void set_sys_clk_to_72M(pll_clk_source_t pll_clk);
static uint32_t s_sys_clock = 0;

const p_set_clk_func_t sys_clk_freq_table[MAX_SYS_CLOCK_FREQ]= 
{
    SYS_CLK_FREQ_CONFIG
};

static uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4 };
static uint8_t ADCPrescTable[4] = {2,4,6,8};


/**
  * @brief  get sys_clock_freq
  * @note   None
  * @param  None
  * @retval g_sys_clock
 */
void drv_get_clock_freq_info(sys_clk_freq_info_t* sys_clk_freq_info)
{
    uint32_t tmp = 0 ;
    uint32_t presc = 0;
    sys_clk_freq_info->system_clk_freq = s_sys_clock;
    /* Get HCLK prescaler */
    tmp = RCC->CFGR & (uint32_t)0x000000F0;
    tmp = tmp >> 4;
    presc = AHBPrescTable[tmp];
    sys_clk_freq_info->hclk_clk_freq = sys_clk_freq_info->system_clk_freq >> presc;
    /* Get PCLK1 prescaler */
    tmp = RCC->CFGR & (uint32_t)0x00000700;
    tmp = tmp >> 8;
    presc = APBPrescTable[tmp];
    /* PCLK1 clock frequency */
    sys_clk_freq_info->pclk1_clk_freq = sys_clk_freq_info->hclk_clk_freq >> presc;
     /* Get PCLK2 prescaler */
    tmp = RCC->CFGR & (uint32_t)0x00003800;
    tmp = tmp >> 11;
    presc = APBPrescTable[tmp];
    /* PCLK2 clock frequency */
    sys_clk_freq_info->pclk2_clk_freq = sys_clk_freq_info->hclk_clk_freq >> presc;
    /* Get ADCCLK prescaler */
    tmp = RCC->CFGR & (uint32_t)0x0000C000;
    tmp = tmp >> 14;
    presc = ADCPrescTable[tmp];
    /* ADCCLK clock frequency */
    sys_clk_freq_info->adcclk_clk_freq = sys_clk_freq_info->pclk2_clk_freq / presc;
}


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
void drv_clock_init(const clk_init_t* clk_init_structure)
{
    volatile uint16_t t_startup_counter = 0;
    volatile uint8_t t_hsestatus = 0;
    p_set_clk_func_t pfunc = NULL;
    drv_rcc_register_reset();
    if(clk_init_structure->sys_clk_source == HSE_SYS)
    {
        /*HSE_SYS is sys clock*/
        set_sys_clk_to_HSE();

   
    }
    else if(clk_init_structure->sys_clk_source == PLL_SYS)
    {
        /*PLL is sys clock*/
        if(clk_init_structure->pll_clk_source != HSI_DIV2_4M)
        {
            /*HSE_PLL is pll clock*/         
            /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
            /* Enable HSE */    
            RCC->CR |= ((uint32_t)RCC_CR_HSEON); 
            /* Wait till HSE is ready and if Time out is reached exit */
            do
            {
                t_hsestatus = RCC->CR & RCC_CR_HSERDY;
                t_startup_counter++;  
            } while((t_hsestatus == 0) && (t_startup_counter != HSE_STARTUP_TIMEOUT));
            if ((RCC->CR & RCC_CR_HSERDY) != RESET)
            {
                t_hsestatus = CLOCK_READY_SUCCESS;
            }
            else
            {
                t_hsestatus = CLOCK_READY_FAILED ;
            }
            
            
        }
        else
        {
            /*HSI_DIV2_PLL is pll clock*/
            RCC->CR |= ((uint32_t)RCC_CR_HSION); 
            /* Wait till HSE is ready and if Time out is reached exit */
            do
            {
                t_hsestatus = RCC->CR & RCC_CR_HSIRDY;
                t_startup_counter++;  
            } while((t_hsestatus == 0) && (t_startup_counter != HSE_STARTUP_TIMEOUT));
            if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
            {
                t_hsestatus = CLOCK_READY_SUCCESS;
            }
            else
            {
                t_hsestatus = CLOCK_READY_FAILED ;
            }
            
        }

        if(t_hsestatus == CLOCK_READY_SUCCESS)
        {
            pfunc = sys_clk_freq_table[clk_init_structure->sys_clock_freq];
            if(NULL != pfunc)
            {
                pfunc(clk_init_structure->pll_clk_source);

            }
           
        }
    }
    else
    {
        /*HSI_SYS is sys clock*/
        /*sys resert default*/
        set_sys_clk_to_HSI();
        //g_sys_clock = 8000000;
    }
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif 

}


/**
  * @brief  reset rcc register
  * @note   This function should be used only in drv_clock_init.
  * @param  None
  * @retval None
 */
void drv_rcc_register_reset(void)
{
	  /* Reset APB1RSTR, APB2RSTR ,AHBENR and APB1ENR  */
    RCC->APB1RSTR = (uint32_t)0x00000000;
    RCC->APB2RSTR = (uint32_t)0x00000000;
    RCC->AHBENR = (uint32_t)0x00000014;
    RCC->APB2ENR = (uint32_t)0x00000000;
    RCC->APB1ENR = (uint32_t)0x00000000;
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;
    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t)0xF8FF0000;
    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;
    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFEFBFFFF;
    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;
    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000;
	
}


/**
  * @brief  set sys clock freq is equal HSE
  * @note   This function should be used only in drv_clock_init.
  * @param  None
  * @retval None
 */
static void set_sys_clk_to_HSE(void)
{
    
    volatile uint16_t t_startup_counter = 0;
    volatile uint8_t t_hsestatus = 0;
  
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
    /* Enable HSE */    
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);
    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        t_hsestatus = RCC->CR & RCC_CR_HSERDY;
        t_startup_counter++;  
    } while((t_hsestatus == 0) && (t_startup_counter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET)
    {
        t_hsestatus = CLOCK_READY_SUCCESS;
    }
    else
    {
        t_hsestatus = CLOCK_READY_FAILED;
        
    }  

    if (t_hsestatus == CLOCK_READY_SUCCESS)
    {
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        /* Flash 0 wait state */
        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
        /* Select HSE as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    

        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
        {
        }
        s_sys_clock = 8000000;
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
    }  
}


/**
  * @brief  set sys clock freq is equal HSI
  * @note   This function should be used only in drv_clock_init.
  * @param  None
  * @retval None
 */
static void set_sys_clk_to_HSI(void)
{
    
    volatile uint16_t t_startup_counter = 0;
    volatile uint8_t t_hsestatus = 0;
  
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
    /* Enable HSE */    
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        t_hsestatus = RCC->CR & RCC_CR_HSIRDY;
        t_startup_counter++;  
    } while((t_hsestatus == 0) && (t_startup_counter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
    {
        t_hsestatus = CLOCK_READY_SUCCESS;
    }
    else
    {
        t_hsestatus = CLOCK_READY_FAILED;
        
    }  

    if (t_hsestatus == CLOCK_READY_SUCCESS)
    {
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        /* Flash 0 wait state */
        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
        /* Select HSE as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        //RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    

        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x00)
        {
        }
        s_sys_clock = 8000000;
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
    }  
}


/**
  * @brief  set sys clock freq 24M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M
  * @retval None
 */
static void set_sys_clk_to_24M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    if(pll_clk == HSE_8M)
    {   
        /*pll clk source is HSE_8M*/
        /*  PLL configuration:  = (HSE / 2) * 6 = 24 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);
    }
    else
    {
        /*pll clk source is HSI_DIV2*/
        /*  PLL configuration:  = (HSI / 2) * 6 = 24 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC_HSE));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLMULL6);
        
    }
    
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 24000000;
}


/**
  * @brief  set sys clock freq 36M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M
  * @retval None
 */
static void set_sys_clk_to_36M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    if(pll_clk == HSE_8M)
    {
        /*  PLL configuration: PLLCLK = (HSE / 2) * 9 = 36 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL9);

    }
    else
    {
        /*pll clk source is HSI_DIV2*/
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC_HSE));
        RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLMULL9);
    }
    
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 36000000;
}


/**
  * @brief  set sys clock freq 48M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M
  * @retval None
 */
static void set_sys_clk_to_48M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    if(pll_clk == HSE_8M)
    {
        /*  PLL configuration: PLLCLK = HSE * 6 = 48 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL6);
    }
    else
    {
        /*pll clk source is HSI_DIV2*/
        /*  PLL configuration: PLLCLK = HSI_DIV2 * 12 = 48 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC_HSE));
        RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLMULL12);
    }
     
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 48000000;
}


/**
  * @brief  set sys clock freq 56M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M
  * @retval None
*/
static void set_sys_clk_to_56M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    if(pll_clk == HSE_8M)
    {
        /* PLL configuration: PLLCLK = HSE * 7 = 56 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL7);
        
    }
    else
    {
        /*pll clk source is HSI_DIV2*/
        /* PLL configuration: PLLCLK = HSI_DIV2 * 14 = 56 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC_HSE));
        RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLMULL14);
        
    }
    
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 56000000;
}


/**
  * @brief  set sys clock freq 64M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M
  * @retval None
 */
void set_sys_clk_to_64M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    if(pll_clk == HSE_8M)
    {
        /*  PLL configuration: PLLCLK = HSE * 8 = 64 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL8);
    }
    else
    {
        
        /*pll clk source is HSI_DIV2*/
        /* PLL configuration: PLLCLK = HSI_DIV2 * 16 = 64 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL));
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC_HSE));
        RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLMULL16);
    }
    
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 64000000;
}



/**
  * @brief  set sys clock freq 72M
  * @note   This function should be used only in drv_clock_init.
  * @param  pll_clk ,value of pll_clk is HSE_8M or HSI_DIV2_4M 
  * @retval None
   ******************************************************************************
  * @attention
  * It is invalid that value of pll_clk is HSI_DIV2_4M,becuase  the max of PLLMULL
  * DIV is 16 ,so the max of sys clock freq is 64M when pll_clk is HSI_DIV2_4M
  ******************************************************************************
*/
static void set_sys_clk_to_72M(pll_clk_source_t pll_clk)
{
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    

 
    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
    
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    s_sys_clock = 72000000;
}
