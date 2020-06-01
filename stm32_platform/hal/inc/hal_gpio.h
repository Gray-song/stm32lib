/**
  ******************************************************************************
  * @file    hal_gpio.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _HAL_GPO_H_
#define _HAL_GPO_H_
#include "drv_gpio.h"
#include "platform_config.h"
#define hal_gpio_exti_config(port,pin,mode,trigger,status) drv_gpio_exti_config(port,pin,mode,trigger,status)
#define hal_gpio_exti_isr_config(pin,callback) drv_gpio_exti_isr_config(pin,callback)

typedef enum
{
    DIO_ACTIVE_LOW  = 0,     //?o???-
    DIO_ACTIVE_HIGH          //?y???-
}gpio_active_level_t;

typedef struct 
{
    io_signal_name_t sig_name;
    bool *p_var;
    
}digital_output_temp_t;

typedef struct
{
    io_signal_name_t sig_name;
    bool *p_var;
    bool original_status;
    uint8_t key_delay_remain;   //°'?ü????
    uint8_t key_delay_init;
}digital_input_temp_t;

typedef struct 
{
    gpio_mode_t mode;    
    gpio_port_t port;
    gpio_pin_t  pin;
    gpio_speed_t speed;  
    gpio_active_level_t active_level;
}io_config_t;

extern bool g_do_signal_led0 ;
extern bool g_do_signal_led1 ;
extern bool g_di_signal_key0 ;
extern bool g_di_signal_key1 ;
extern void hal_do_write_signal(io_signal_name_t signl, bool status);
extern io_err_t hal_di_read_signal(io_signal_name_t signl, bool *p_status);
extern void hal_gpio_init(void);
extern void hal_do_output(void);
extern void hal_di_input(void);
extern void hal_exti5_isr(void);
#endif
