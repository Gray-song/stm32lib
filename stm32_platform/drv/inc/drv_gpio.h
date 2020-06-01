/**
  ******************************************************************************
  * @file    drv_clock.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_
#include "stm32f103rc.h"

typedef enum
{
    GPIO_Mode_ANALOG_IN = 0x0,          /**/
    GPIO_Mode_IN_FLOATING = 0x04,       /**/
    GPIO_Mode_IN_PULL_DOWN = 0x28,      /**/
    GPIO_Mode_IN_PULL_UP = 0x48,        /**/
    GPIO_Mode_Out_OPEN_DRAIN = 0x14,    /**/
    GPIO_Mode_Out_PUSH_PULL = 0x10,     /**/
    GPIO_Mode_AF_OPEN_DRAIN = 0x1C,             /**/
    GPIO_Mode_AF_PUSH_PULL = 0x18              /**/
}gpio_mode_t;

typedef enum 
{
    GPIO_Speed_10MHz = 1,
    GPIO_Speed_20MHz, 
    GPIO_Speed_50MHz
}gpio_speed_t;

typedef enum
{
    GPIO_A = 0,
    GPIO_B,
    GPIO_C,
    GPIO_D,
    GPIO_E,
    GPIO_F,
    GPIO_G   
}gpio_port_t;

typedef enum
{
    GPIO_Pin_0 = 0x0001 , /*!< Pin 0 selected */
    GPIO_Pin_1 = 0x0002 , /*!< Pin 1 selected */
    GPIO_Pin_2 = 0x0004 , /*!< Pin 2 selected */
    GPIO_Pin_3 = 0x0008 ,  /*!< Pin 3 selected */
    GPIO_Pin_4 = 0x0010 ,  /*!< Pin 4 selected */
    GPIO_Pin_5 = 0x0020 ,  /*!< Pin 5 selected */
    GPIO_Pin_6 = 0x0040 ,  /*!< Pin 6 selected */
    GPIO_Pin_7 = 0x0080 ,  /*!< Pin 7 selected */
    GPIO_Pin_8 = 0x0100 ,  /*!< Pin 8 selected */
    GPIO_Pin_9 = 0x0200 ,  /*!< Pin 9 selected */
    GPIO_Pin_10 = 0x0400 ,   /*!< Pin 10 selected */
    GPIO_Pin_11 = 0x0800 ,   /*!< Pin 11 selected */
    GPIO_Pin_12 = 0x1000 ,  /*!< Pin 12 selected */
    GPIO_Pin_13 = 0x2000 ,  /*!< Pin 13 selected */
    GPIO_Pin_14 = 0x4000 ,  /*!< Pin 14 selected */
    GPIO_Pin_15 = 0x8000 ,  /*!< Pin 15 selected */
    GPIO_Pin_All = 0xFFFF ,  /*!< All pins selected */
}gpio_pin_t;

typedef struct
{
    gpio_port_t port;
    gpio_pin_t pin;
    gpio_speed_t speed;
    gpio_mode_t mode;
}gpio_init_t;

typedef enum
{
    EXTI_ModeInterrupt = 0,
    EXTI_Mode_Event
}exit_mode_t;

typedef enum
{
  EXTI_Trigger_Rising = 0,
  EXTI_Trigger_Falling ,  
  EXTI_Trigger_Rising_Falling 
}exti_trigger_t;


extern void drv_gpio_init(const gpio_init_t *gpio_init_struct);
extern bool drv_gpio_read_bit(gpio_port_t port,gpio_pin_t pin);
extern uint16_t drv_gpio_read_port(gpio_port_t port);
extern void drv_gpio_write_bit(gpio_port_t port,gpio_pin_t pin, uint8_t bit_status);
extern void drv_gpio_write_port(gpio_port_t port,uint16_t bits);
extern void drv_gpio_exti_config(gpio_port_t port,gpio_pin_t pin,exit_mode_t mode,exti_trigger_t trigger,bool status);
extern void drv_gpio_exti_isr_config(gpio_pin_t pin,voidfunc_t p_callback);
#endif
