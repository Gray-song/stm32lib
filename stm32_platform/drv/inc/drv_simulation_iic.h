/**
  ******************************************************************************
  * @file    drv_simulation_iic.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_SIMULATION_IIC_H_
#define _DRV_SIMULATION_IIC_H_
#include "stm32f103rc.h"
#include "drv_gpio.h"
#include "drv_systick.h"
#define WAIT_ACK_TIMEOUT 250

typedef enum
{
    INPUT = 0,
    OUTPUT ,    
}sda_dir_t;

typedef struct
{
    uint8_t t_su_sta_us;  /*Start Setup Time*/
    uint8_t t_hd_sta_us;  /*Start Hold Time*/
    uint8_t t_low_us;     /*Clock Pulse Width Low*/
    uint8_t t_high_us;    /*Clock Pulse Width High*/
    uint8_t t_su_sto_us;  /*Stop Setup Time*/
    //uint8_t t_buff_us;    /*Time the bus must be free before a new transmission can start(*/
    uint8_t t_aa_us;      /*Clock Low to Data Out Valid*/
}simu_iic_time_param_t;

typedef struct
{
    gpio_port_t scl_port;
    gpio_port_t sda_port;
    gpio_pin_t scl_pin;
    gpio_pin_t sda_pin;
    simu_iic_time_param_t simu_iic_time;
    
}simulation_iic_init_t;

extern void drv_iic_simulation_init(simulation_iic_init_t *simulation_iic_init_struct);
extern void drv_iic_start_signal(void);
extern void drv_iic_stop_signal(void);
extern bool drv_iic_wait_ack_signal(void);
extern void drv_iic_ack_signal(void);
extern void drv_iic_no_ack_signal(void);
extern void drv_iic_send_byte(uint8_t data);
extern uint8_t drv_iic_receive_byte(uint8_t ack);
extern bool drv_iic_check_device(uint8_t devie_address);
#endif
