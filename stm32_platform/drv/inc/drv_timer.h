/**
  ******************************************************************************
  * @file    drv_timer.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_
#include "stm32f103rc.h"
#define TIMER_COUNT 8
typedef enum
{
    TIM_INTERRUPT_OVER_FLOW = 0,
    TIM_INTERRUPT_CC1,
    TIM_INTERRUPT_CC2,
    TIM_INTERRUPT_CC3,
    TIM_INTERRUPT_CC4,
} timer_it_source_t;

typedef struct
{
    voidfunc_t p_tim_cc1_interrupt ;
    voidfunc_t p_tim_cc2_interrupt ;
    voidfunc_t p_tim_cc3_interrupt ;
    voidfunc_t p_tim_cc4_interrupt ;
    voidfunc_t p_tim_overflow_interrupt ;
}timer_interrupt_fun_point_t;

typedef enum
{
    TIMER1 = 0,
    TIMER2 ,
    TIMER3 ,
    TIMER4 ,
    TIMER5 ,
    TIMER6 ,
    TIMER7 ,
    TIMER8 
}timer_t;

typedef enum
{
    RISING = 0,
    FALLING ,

}cap_valid_edge_t;

typedef enum
{
    channel1 = 0,
    channel2,
    channel3,
    channel4
}map_channel_t;

typedef enum
{
    NO_FILTER = 0X0000,
    SAMPLING_CK_INT_N_2 = 0x0001,
    SAMPLING_CK_INT_N_4 = 0x0010,
    SAMPLING_CK_INT_N_8 = 0x0011,
    SAMPLING_DST_2_N_6 = 0x0100,
    SAMPLING_DST_2_N_8 = 0x0101,
    SAMPLING_DST_4_N_6 = 0x0110,
    SAMPLING_DST_4_N_8 = 0x0111,
    SAMPLING_DST_8_N_6 = 0x1000,
    SAMPLING_DST_8_N_8 = 0x1001,
    SAMPLING_DST_16_N_5 = 0x1010,
    SAMPLING_DST_16_N_6 = 0x1011,
    SAMPLING_DST_16_N_8 = 0x1100,
    SAMPLING_DST_32_N_5 = 0x1101,
    SAMPLING_DST_32_N_6 = 0x1110,
    SAMPLING_DST_32_N_8 = 0x1111,
}filter_prama_t;

typedef enum
{
    IC_NO_DIV = 0,
    IC_DIV_2,
    IC_DIV_4,
    IC_DIV_8
}input_div_t;

typedef enum
{
    IC_DIREACT_TI = 0x0001,
    IC_INDIREACT_TI = 0x0002,
    IC_TRC = 0x003
}input_cap_selection_t;

typedef struct
{
    cap_valid_edge_t edge;
    map_channel_t  channel;
    filter_prama_t filter_prama;
    input_div_t input_div;
    input_cap_selection_t input_cap_selection;
    uint16_t psc;
    uint16_t load_value;

}timer_cap_init_t;

typedef enum 
{
    FREEZE = 0,
    OUTPUT_STATE,
    OUTPUT_N_STATE,
    TURN,
    ENFORCE_VALID,
    ENFORCE_INVALID,
    PWM_MODE_1,
    PWM_MODE_2   
    
}timer_output_compare_mode_t;

typedef enum 
{
    HIGH_VALID,
    LOW_VALID    
    
}timer_pwm_valid_level_t;

typedef struct
{
    uint16_t psc;
    uint16_t load_value;
    map_channel_t  channel;
    timer_output_compare_mode_t oc_mode;
    timer_pwm_valid_level_t pwm_valid_level;
    
}timer_pwm_init_t;


extern void drv_timer_init(timer_t timx,uint16_t psc, uint16_t load_value);
extern void drv_timer_cmd(timer_t timx,bool status);
extern void drv_timer_it_config(timer_t timx,timer_it_source_t it_source,bool status);
extern void drv_timer_isr_config(const timer_t timx,voidfunc_t p_callback,timer_it_source_t callback_type);
extern uint16_t drv_timer_conter_get(const timer_t timx);
extern uint16_t drv_get_timer_sr_value(timer_t timx);

/*input capture. TIM6,TIM7 cannot*/
extern void drv_timer_cap_init(timer_t timx,timer_cap_init_t timer_cap_init_struct );
extern void drv_set_timer_cap_edge(timer_t timx,map_channel_t channel,cap_valid_edge_t edge);
extern uint16_t drv_get_timer_cap_value(timer_t timx,map_channel_t channel);

/*pwm */
extern void drv_timer_pwm_init(timer_t timx,timer_pwm_init_t timer_pwm_init_struct);
extern void drv_set_duty_cycle(timer_t timx,map_channel_t channel,uint8_t percent);
#endif
