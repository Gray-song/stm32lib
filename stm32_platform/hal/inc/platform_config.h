/**
  ******************************************************************************
  * @file    platform_config.h
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#define MCU_VERSION_A (uint8_t)1
#define MCU_VERSION_B (uint8_t)0
#define MCU_VERSION_C (uint8_t)2
#define YEAR (uint8_t)20
#define MONTH (uint8_t)6
#define DAY (uint8_t)4
#define DAC_MAX_VOLTAGE 3.3
#define IO_SIGNALS_COUNT 10
#define DO_SIGNALS_COUNT 2
#define DI_SIGNALS_COUNT 3
#define NVIC_IRQ_COUNT 4
#define ADC_SIGNALS_COUNT 2
#define  USART_CHANNEL_COUNT 5
typedef enum
{
    DO_LED0 = 0,
    DO_LED1 ,
    DI_KEY0 ,
    DI_KEY1 ,
    AF_PP_USART1_TX,
    DI_FLOAT_USART1_RX,
    DI_WK_UP,
    AI_ADC1_CH1,
    AI_ADC1_CH10,
    AI_DAC_CH1
}io_signal_name_t;
/*IO INIT CONFIG TABLE*/
#define IO_SIGNALS_CONFIG \
/*DO_LED0*/             {GPIO_Mode_AF_PUSH_PULL,GPIO_A,GPIO_Pin_8,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*DO_LED1*/             {GPIO_Mode_Out_PUSH_PULL,GPIO_D,GPIO_Pin_2,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*DO_KEY0*/             {GPIO_Mode_IN_PULL_UP,GPIO_C,GPIO_Pin_5,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*DO_KEY1*/             {GPIO_Mode_IN_PULL_UP,GPIO_A,GPIO_Pin_15,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*AF_PP_USART1_TX*/     {GPIO_Mode_AF_PUSH_PULL,GPIO_A,GPIO_Pin_9,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*DI_FLOAT_USART1_RX*/  {GPIO_Mode_IN_FLOATING,GPIO_A,GPIO_Pin_10,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*DI_WK_UP*/            {GPIO_Mode_IN_PULL_DOWN,GPIO_A,GPIO_Pin_0,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*AI_ADC1_CH1*/         {GPIO_Mode_ANALOG_IN,GPIO_A,GPIO_Pin_1,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*AI_ADC1_CH10*/        {GPIO_Mode_ANALOG_IN,GPIO_C,GPIO_Pin_0,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH},\
/*AI_DAC_CH10*/         {GPIO_Mode_ANALOG_IN,GPIO_A,GPIO_Pin_4,GPIO_Speed_50MHz,DIO_ACTIVE_HIGH}

/*IO DIGITAL OUTPUT CONFIG TABLE*/
#define APP_DO_CONFIG \
{DO_LED0,&g_do_signal_led0},\
{DO_LED1,&g_do_signal_led1}

/*IO DIGITAL INPUT CONFIG TABLE*/
#define APP_DI_CONFIG \
{DI_KEY0,&g_di_signal_key0,1,10,10},\
{DI_KEY1,&g_di_signal_key1,1,10,10},\
{DI_WK_UP,&g_di_signal_wk_up,0,10,10}

/*IQR PRIORITY CONFIG TABLE*/
#define NVIC_IRQ_CONFIG \
{USART1_IRQn,3,3,ENABLE_DEF},\
{TIM3_IRQn,1,3,ENABLE_DEF},\
{TIM2_IRQn,1,2,ENABLE_DEF},\
{ADC1_2_IRQn,1,1,ENABLE_DEF}
//{EXTI9_5_IRQn,2,2,ENABLE_DEF}

#define USART_CONFIG \
/*USART channel    bautrate      data_len          stop_bits         parity          hardware_flow_ctrl       mode */ \
/*USART1*/        {19200,     DATA_LEN_8BIT,    STOP_BITS_1,    USART_PARITY_NONE,    HW_FLOW_CTRL_None,    USART_MODE_RX_TX},\
/*USART2*/        {115200,    DATA_LEN_8BIT,    STOP_BITS_1,    USART_PARITY_NONE,    HW_FLOW_CTRL_None,    USART_MODE_RX_TX},\
/*USART3*/        {9600,      DATA_LEN_8BIT,    STOP_BITS_1,    USART_PARITY_NONE,    HW_FLOW_CTRL_None,    USART_MODE_RX_TX},\
/*USART4*/        {19200,     DATA_LEN_8BIT,    STOP_BITS_1,    USART_PARITY_NONE,    HW_FLOW_CTRL_None,    USART_MODE_RX_TX},\
/*USART5*/        {19200,     DATA_LEN_8BIT,    STOP_BITS_1,    USART_PARITY_NONE,    HW_FLOW_CTRL_None,    USART_MODE_RX_TX}




typedef enum 
{
   
    ADC_3V3,   /*AI_ADC1_CH1*/
    ADC_GND    /*AI_ADC1_CH10*/
     
}adc_signal_name_t;

#define ADC_SIGNALS_CONFIG \
/*ADC_3V3*/        {ADC_CHANNEL_1,0,adc_clk_239_5},\
/*ADC_GND*/        {ADC_CHANNEL_10,0,adc_clk_239_5}

#endif 
