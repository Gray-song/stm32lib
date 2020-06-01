/**
  ******************************************************************************
  * @file    drv_gpio.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "drv_gpio.h"
static voidfunc_t p_exti0_interrupt = NULL;
static voidfunc_t p_exti1_interrupt = NULL;
static voidfunc_t p_exti2_interrupt = NULL;
static voidfunc_t p_exti3_interrupt = NULL;
static voidfunc_t p_exti4_interrupt = NULL;
static voidfunc_t p_exti5_interrupt = NULL;
static voidfunc_t p_exti6_interrupt = NULL;
static voidfunc_t p_exti7_interrupt = NULL;
static voidfunc_t p_exti8_interrupt = NULL;
static voidfunc_t p_exti9_interrupt = NULL;
static voidfunc_t p_exti10_interrupt = NULL;
static voidfunc_t p_exti11_interrupt = NULL;
static voidfunc_t p_exti12_interrupt = NULL;
static voidfunc_t p_exti13_interrupt = NULL;
static voidfunc_t p_exti14_interrupt = NULL;
static voidfunc_t p_exti15_interrupt = NULL;

static volatile GPIO_TypeDef* const PORT[]=
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    
};


/**
  * @brief  drv_gpio_init
  * @note   None
  * @param  gpio_init_struct
  * @retval None
 */
void drv_gpio_init(const gpio_init_t *gpio_init_struct)
{
    uint32_t t_current_mode = 0x00;
    uint32_t t_current_pin = 0x00;
    uint32_t t_pin_pos = 0x00;
    uint32_t t_pos = 0x00;
    uint32_t t_pin_mask = 0x00;
    uint32_t t_config = 0x00;
    /*3?ê??¯??ó|???úµ?íaéèê±?ó*/
    RCC->APB2ENR |= (uint32_t)0x01 << (gpio_init_struct->port + 2);
    
    t_current_mode  = (uint32_t)gpio_init_struct->mode & (uint32_t)0x0f;
    if(((uint32_t)gpio_init_struct->mode & (uint32_t) 0x10) != 0x00)
    {
        /*ê?3???ê?*/
        t_current_mode |= (uint32_t)gpio_init_struct->speed;
    }
    
    /*µí8??*/
    if(((uint32_t)gpio_init_struct->pin & (uint32_t)0x00ff) != 0x00)
    {
        t_config = PORT[gpio_init_struct->port]->CRL;
        for(t_pin_pos=0; t_pin_pos<8; t_pin_pos++)
        {
            t_pos = (uint32_t)0x01 << t_pin_pos;
            t_current_pin = t_pos & ((uint32_t)gpio_init_struct->pin);
            if(t_current_pin == t_pos)
            {
                
                t_pos = t_pin_pos << 2;
                /* Clear the corresponding low control register bits */
                t_pin_mask = ((uint32_t)0x0F) << t_pos;
                t_config &= ~t_pin_mask;
                t_config |= (t_current_mode << t_pos);
                if (gpio_init_struct->mode == GPIO_Mode_IN_PULL_DOWN)
                {
                    /* reset the corresponding ODR bit */
                    PORT[gpio_init_struct->port]->BRR = (((uint32_t)0x01) << t_pin_pos);
                }
                else
                {
                     /* Set the corresponding ODR bit */
                     if (gpio_init_struct->mode == GPIO_Mode_IN_PULL_UP)
                     {
                         PORT[gpio_init_struct->port]->BSRR = (((uint32_t)0x01) << t_pin_pos);
                     }
                }
            }
        }
        PORT[gpio_init_struct->port]->CRL = t_config;
    }
    
    /*??8??*/
    if(((uint32_t)gpio_init_struct->pin) >  0x00FF)
    {
        t_config = PORT[gpio_init_struct->port]->CRH;
        for(t_pin_pos=0; t_pin_pos<8; t_pin_pos++)
        {
            t_pos = (uint32_t)0x01 << (t_pin_pos + 8);
            t_current_pin = t_pos & ((uint32_t)gpio_init_struct->pin);
            if(t_current_pin == t_pos) /*?òµ?òy??????*/
            {
                
                t_pos = t_pin_pos << 2;
                /* Clear the corresponding low control register bits */
                t_pin_mask = ((uint32_t)0x0F) << t_pos;
                t_config &= ~t_pin_mask;
                t_config |= (t_current_mode << t_pos);
                if (gpio_init_struct->mode == GPIO_Mode_IN_PULL_DOWN)
                {
                    /* reset the corresponding ODR bit */
                    PORT[gpio_init_struct->port]->BRR = (((uint32_t)0x01) << (t_pin_pos + 8));
                }
                else
                {
                     /* Set the corresponding ODR bit */
                     if (gpio_init_struct->mode == GPIO_Mode_IN_PULL_UP)
                     {
                         PORT[gpio_init_struct->port]->BSRR = (((uint32_t)0x01) << (t_pin_pos + 8));
                     }
                }
            }
        }
        PORT[gpio_init_struct->port]->CRH = t_config;
    }
}


/**
  * @brief  get drv_gpio_read_bit
  * @note   None
  * @param  port,pin
  * @retval bit_status
 */
bool drv_gpio_read_bit(gpio_port_t port,gpio_pin_t pin)
{
    bool bit_status = FALSE;
    if((PORT[port]->IDR & (uint32_t)pin) != (uint32_t)FALSE)
    {
        bit_status = TRUE;
    }
    else
    {
        bit_status = FALSE;
    }
    return bit_status;
}


/**
  * @brief  drv_gpio_read_port
  * @note   None
  * @param  port
  * @retval 
 */
uint16_t drv_gpio_read_port(gpio_port_t port)
{
    return (uint16_t)PORT[port]->IDR;
}


/**
  * @brief  drv_gpio_write_bit
  * @note   None
  * @param  port,pin,bit_status
  * @retval None
 */
void drv_gpio_write_bit(gpio_port_t port,gpio_pin_t pin, uint8_t bit_status)
{
    if(bit_status == TRUE)
    {
        PORT[port]->BSRR |= (uint32_t)pin;
    }
    else
    {
        PORT[port]->BRR |= (uint32_t)pin;
    }
}


/**
  * @brief  drv_gpio_write_port
  * @note   None
  * @param  port,bits
  * @retval None
 */
void drv_gpio_write_port(gpio_port_t port,uint16_t bits)
{ 
    PORT[port]->ODR = bits;
}


/**
  * @brief  drv_gpio_exti_isr_config
  * @note   None
  * @param  pin,p_callback
  * @retval None
 */
void drv_gpio_exti_isr_config(gpio_pin_t pin,voidfunc_t p_callback)
{
    uint16_t t_current_pin = 0x00;
    uint8_t t_pin_pos = 0x00;
    uint16_t t_pos = 0x00;
    uint16_t pin_pos_confirm = 0x00;
    for(t_pin_pos = 0; t_pin_pos<16; t_pin_pos++ )
    {
        t_pos = (uint16_t)0x01 << t_pin_pos;
        t_current_pin = t_pos & ((uint16_t)pin);
        if(t_current_pin == t_pos)
        {
            pin_pos_confirm = t_pin_pos;
            break;
        }
    }
    
    switch(pin_pos_confirm)
    {
        case 0:
            p_exti0_interrupt = p_callback;
            break;
        case 1:
            p_exti1_interrupt = p_callback;
            break;
        case 2:
            p_exti2_interrupt = p_callback;
            break;
        case 3:
            p_exti3_interrupt = p_callback;
            break;
        case 4:
            p_exti4_interrupt = p_callback;
            break;
        case 5:
            p_exti5_interrupt = p_callback;
            break;
        case 6:
            p_exti6_interrupt = p_callback;
            break;
        case 7:
            p_exti7_interrupt = p_callback;
            break;
        case 8:
            p_exti8_interrupt = p_callback;
            break;
        case 9:
            p_exti9_interrupt = p_callback;
            break;
        case 10:
            p_exti10_interrupt = p_callback;
            break;
        case 11:
            p_exti11_interrupt = p_callback;
            break;
        case 12:
            p_exti12_interrupt = p_callback;
            break;
        case 13:
            p_exti13_interrupt = p_callback;
            break;
        case 14:
            p_exti14_interrupt = p_callback;
            break;
        case 15:
            p_exti15_interrupt = p_callback;
            break;
        default:
            break;
    
    }
}


/**
  * @brief  GPIO External Interrupt config
  * @note   None
  * @param  
  * @retval None
 */
void drv_gpio_exti_config(gpio_port_t port,gpio_pin_t pin,exit_mode_t mode,exti_trigger_t trigger,bool status)
{
    uint8_t exti_adrr = 0;
    uint8_t exti_offset = 0;
    uint16_t t_current_pin = 0x00;
    uint8_t t_pin_pos = 0x00;
    uint16_t t_pos = 0x00;
    if(status == ENABLE_DEF)
    {
        /*enable AFIOê±?ó*/
        RCC->APB2ENR |= (uint32_t)0x01;
        for(t_pin_pos = 0; t_pin_pos<16; t_pin_pos++ )
        {
            t_pos = (uint16_t)0x01 << t_pin_pos;
            t_current_pin = t_pos & ((uint16_t)pin);
            if(t_current_pin == t_pos)
            {
                exti_adrr = t_pin_pos/4;
                exti_offset = (t_pin_pos%4)*4;
                break;
            }
        }
        AFIO->EXTICR[exti_adrr] &= ~(0X000F << exti_offset);
        AFIO->EXTICR[exti_adrr] |= port << exti_offset;
        if(mode == EXTI_ModeInterrupt)
        {
            EXTI->IMR &= ~(pin);
            EXTI->IMR |= pin;
        }
        else
        {
            EXTI->EMR &= ~(pin);
            EXTI->EMR |= pin;
        }
        
        if(trigger == EXTI_Trigger_Rising_Falling)
        {
            EXTI->RTSR &= ~pin;
            EXTI->RTSR |= pin;
            EXTI->FTSR &= ~pin;
            EXTI->FTSR |= pin;
        }
        else if(trigger == EXTI_Trigger_Rising)
        {
            EXTI->RTSR &= ~pin;
            EXTI->RTSR |= pin;
        }
        else
        {
            EXTI->FTSR &= ~pin;
            EXTI->FTSR |= pin;
        }
    }
    else
    {
        if(mode == EXTI_ModeInterrupt)
        {
            EXTI->IMR &= ~(pin);
            
        }
        else
        {
            EXTI->EMR &= ~(pin);
            
        }
    }   
}


/**
  * @brief  GPIO External Interrupt line 0 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI0_IRQHandler(void)
{
    if(NULL != p_exti0_interrupt )
    {
        p_exti0_interrupt();
        
    }
    EXTI->PR |= ((uint32_t)0x01) << 0;
}


/**
  * @brief  GPIO External Interrupt line 1 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI1_IRQHandler(void)
{
    if(NULL != p_exti1_interrupt )
    {
        p_exti1_interrupt();
    }
    EXTI->PR |= ((uint32_t)0x01) << 1;
}


/**
  * @brief  GPIO External Interrupt line 2 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI2_IRQHandler(void)
{
    if(NULL != p_exti2_interrupt )
    {
        p_exti2_interrupt();
    }
    EXTI->PR |= ((uint32_t)0x01) << 2;
}


/**
  * @brief  GPIO External Interrupt line 3 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI3_IRQHandler(void)
{
    if(NULL != p_exti3_interrupt )
    {
        p_exti3_interrupt();
    }
    EXTI->PR |= ((uint32_t)0x01) << 3;
}


/**
  * @brief  GPIO External Interrupt line 4 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI4_IRQHandler(void)
{
    if(NULL != p_exti4_interrupt )
    {
        p_exti4_interrupt();
    }
    EXTI->PR |= ((uint32_t)0x01) << 4;
}


/**
  * @brief  GPIO External Interrupt line 5-9 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI9_5_IRQHandler(void)
{
    if((EXTI->PR & (uint32_t)0x0020)!= 0)
    {
        /*pin 5*/
        if(NULL != p_exti5_interrupt )
        {
            p_exti5_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 5;
    }
    
    if((EXTI->PR & (uint32_t)0x0040)!= 0)
    {
        /*pin 6*/
        if(NULL != p_exti6_interrupt )
        {
            p_exti6_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 6;
    }
    
    if((EXTI->PR & (uint32_t)0x0080)!= 0)
    {
        /*pin 7*/
        if(NULL != p_exti7_interrupt )
        {
            p_exti7_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 7;
    }
    
    if((EXTI->PR & (uint32_t)0x0100)!= 0)
    {
        /*pin 8*/
        if(NULL != p_exti8_interrupt )
        {
            p_exti8_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 8;
    }
    
    if((EXTI->PR & (uint32_t)0x0200)!= 0)
    {
        /*pin 9*/
        if(NULL != p_exti9_interrupt )
        {
            p_exti9_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 9;
    }
    
}


/**
  * @brief  GPIO External Interrupt line 10-15 Service
  * @note   None
  * @param  
  * @retval None
 */
void EXTI15_10_IRQHandler(void)
{
    if((EXTI->PR & (uint32_t)0x0400)!= 0)
    {
        /*pin 10*/
        if(NULL != p_exti10_interrupt )
        {
            p_exti10_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 10;
    }
    
    if((EXTI->PR & (uint32_t)0x0800)!= 0)
    {
        /*pin 11*/
        if(NULL != p_exti11_interrupt )
        {
            p_exti11_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 11;
    }
    
    if((EXTI->PR & (uint32_t)0x1000)!= 0)
    {
        /*pin 12*/
        if(NULL != p_exti12_interrupt )
        {
            p_exti12_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 12;
    }
    
    if((EXTI->PR & (uint32_t)0x2000)!= 0)
    {
        /*pin 13*/
        if(NULL != p_exti13_interrupt )
        {
            p_exti13_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 13;
    }
    
    if((EXTI->PR & (uint32_t)0x4000)!= 0)
    {
        /*pin 14*/
        if(NULL != p_exti14_interrupt )
        {
            p_exti14_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 14;
    }
    
    if((EXTI->PR & (uint32_t)0x8000)!= 0)
    {
        /*pin 15*/
        if(NULL != p_exti15_interrupt )
        {
            p_exti15_interrupt();
        }
        EXTI->PR |= ((uint32_t)0x01) << 15;
    }
}
