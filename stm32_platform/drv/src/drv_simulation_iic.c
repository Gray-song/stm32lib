/**
  ******************************************************************************
  * @file    drv_simulation_iic.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/

#include "drv_simulation_iic.h"
static  uint8_t s_su_sta_time;   /*Start Setup Time*/
static  uint8_t s_hd_sta_time;  /*Start Hold Time*/
static  uint8_t s_low_time;     /*Clock Pulse Width Low*/
static  uint8_t s_high_time;    /*Clock Pulse Width High*/
static  uint8_t s_su_sto_time;  /*Stop Setup Time*/
//static  uint8_t s_buff_time;    /*Time the bus must be free before a new transmission can start(*/
static  uint8_t s_aa_time;      /*Clock Low to Data Out Valid*/

static gpio_port_t s_sda_port;
static gpio_port_t s_scl_port;
static gpio_pin_t s_sda_pin;
static gpio_pin_t s_scl_pin;

static gpio_init_t s_gpio_sda_init;
static gpio_init_t s_gpio_scl_init;

static void drv_sda_dir_set(sda_dir_t dir);
static void drv_iic_sda_set(void);
static void drv_iic_sda_reset(void);
static void drv_iic_scl_set(void);
static void drv_iic_scl_reset(void);


/**
  * @brief  drv_sda_dir_set
  * @note   None
  * @param  
  * @retval None
 */
static void drv_sda_dir_set(sda_dir_t dir)
{
    
    if(dir == INPUT)
    {
        s_gpio_sda_init.mode = GPIO_Mode_IN_PULL_UP;
        drv_gpio_init(&s_gpio_sda_init);
    }
    else
    {
        s_gpio_sda_init.mode = GPIO_Mode_Out_PUSH_PULL;
        drv_gpio_init(&s_gpio_sda_init);
    }
}


/**
  * @brief  drv_iic_sda_set
  * @note   None
  * @param  
  * @retval None
 */
static void drv_iic_sda_set(void)
{
    drv_gpio_write_bit(s_sda_port,s_sda_pin,TRUE);
}


/**
  * @brief  drv_iic_sda_reset
  * @note   None
  * @param  
  * @retval None
 */
static void drv_iic_sda_reset(void)
{
    drv_gpio_write_bit(s_sda_port,s_sda_pin,FALSE);
}


/**
  * @brief  drv_iic_scl_set
  * @note   None
  * @param  
  * @retval None
 */
static void drv_iic_scl_set(void)
{
    drv_gpio_write_bit(s_scl_port,s_scl_pin,TRUE);
}


/**
  * @brief  drv_iic_scl_reset
  * @note   None
  * @param  
  * @retval None
 */
static void drv_iic_scl_reset(void)
{
    drv_gpio_write_bit(s_scl_port,s_scl_pin,FALSE);
}


/**
  * @brief  drv_iic_simulation_init
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_simulation_init(simulation_iic_init_t *simulation_iic_init_struct)
{
    
    s_su_sta_time = simulation_iic_init_struct->simu_iic_time.t_su_sta_us;
    s_hd_sta_time = simulation_iic_init_struct->simu_iic_time.t_hd_sta_us;
    s_low_time = simulation_iic_init_struct->simu_iic_time.t_low_us;
    s_high_time = simulation_iic_init_struct->simu_iic_time.t_high_us;
    s_su_sto_time = simulation_iic_init_struct->simu_iic_time.t_su_sto_us;
   // s_buff_time = simulation_iic_init_struct->simu_iic_time.t_buff_us;
    s_aa_time = simulation_iic_init_struct->simu_iic_time.t_aa_us;
    s_sda_port = simulation_iic_init_struct->sda_port;
    s_scl_port = simulation_iic_init_struct->scl_port;
    s_sda_pin = simulation_iic_init_struct->sda_pin;
    s_scl_pin = simulation_iic_init_struct->scl_pin;
    s_gpio_sda_init.port = s_sda_port;
    s_gpio_sda_init.pin = s_sda_pin;
    s_gpio_sda_init.mode = GPIO_Mode_Out_PUSH_PULL;
    s_gpio_sda_init.speed = GPIO_Speed_50MHz;
    drv_gpio_init(&s_gpio_sda_init);
    s_gpio_scl_init.port = s_scl_port;
    s_gpio_scl_init.pin = s_scl_pin;
    s_gpio_scl_init.mode = GPIO_Mode_Out_PUSH_PULL;
    s_gpio_scl_init.speed = GPIO_Speed_50MHz;
    drv_gpio_init(&s_gpio_scl_init);
    drv_iic_sda_set();
    drv_iic_scl_set();
    
}


/**
  * @brief  drv_iic_start_signal
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_start_signal(void)
{
    drv_sda_dir_set(OUTPUT);
    drv_iic_scl_set();
    drv_iic_sda_set();
    drv_delay_us((uint32_t)s_su_sta_time);
    drv_iic_sda_reset();
    drv_delay_us((uint32_t)s_hd_sta_time);
    drv_iic_scl_reset();
}


/**
  * @brief  drv_iic_stop_signal
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_stop_signal(void)
{
    drv_sda_dir_set(OUTPUT);
    drv_iic_sda_reset();
    drv_iic_scl_reset();
    drv_delay_us((uint32_t)s_su_sto_time);/*s_su_sto_time */
    drv_iic_scl_set();
    drv_delay_us((uint32_t)s_su_sto_time);
    drv_iic_sda_set();
}


/**
  * @brief  drv_iic_wait_ack_signal
  * @note   None
  * @param  
  * @retval None
 */
bool drv_iic_wait_ack_signal(void)
{
    uint8_t iic_error_cnt = 0;
    bool sda_status = FALSE; 
    drv_sda_dir_set(INPUT);
    drv_iic_sda_set();
    drv_delay_us((uint32_t)s_aa_time);
    drv_iic_scl_set();
    drv_delay_us((uint32_t)s_aa_time);
    sda_status = drv_gpio_read_bit(s_sda_port,s_sda_pin);
    while(sda_status)
    {
        iic_error_cnt++;
        if(iic_error_cnt > WAIT_ACK_TIMEOUT)
        {
            drv_iic_stop_signal();
            return FALSE;
        }
    }
    drv_iic_scl_reset();
    return TRUE;
}


/**
  * @brief  drv_iic_wait_ack_signal
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_ack_signal(void)
{
    drv_iic_scl_reset();
    drv_sda_dir_set(OUTPUT);
    drv_iic_sda_reset();
    drv_delay_us(s_low_time/2);
    drv_iic_scl_set();
    drv_delay_us(s_high_time);
    drv_iic_scl_reset();
    
}


/**
  * @brief  drv_iic_no_ack_signal
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_no_ack_signal(void)
{
    drv_iic_scl_reset();
    drv_sda_dir_set(OUTPUT);
    drv_iic_sda_set();
    drv_delay_us(s_low_time/2);
    drv_iic_scl_set();
    drv_delay_us(s_high_time);
    drv_iic_scl_reset();
}


/**
  * @brief  drv_iic_send_byte
  * @note   None
  * @param  
  * @retval None
 */
void drv_iic_send_byte(uint8_t data)
{
    uint8_t i = 0;
    drv_sda_dir_set(OUTPUT);
    drv_iic_scl_reset();
    for(i=0; i<8; i++)
    {
        if((((data&0x80)>>7) == 1))
        {
            drv_iic_sda_set();
        }
        else
        {
            drv_iic_sda_reset();
        }
        data <<=1;
        drv_delay_us(s_low_time/2);
        drv_iic_scl_set();
        drv_delay_us(s_high_time);
        drv_iic_scl_reset();
        drv_delay_us(s_low_time/2);
    }
}


/**
  * @brief  drv_iic_send_byte
  * @note   when ack is 1, MCU has ack.when ack is 0,MCU do not have ack 
  * @param  
  * @retval 
 */
uint8_t drv_iic_receive_byte(uint8_t ack)
{
    uint8_t i = 0;
    uint8_t receive = 0;
    bool sda_status = FALSE; 
    drv_sda_dir_set(INPUT);
    for(i=0;i<8;i++)
    {
        drv_iic_scl_reset();
        drv_delay_us(s_low_time/2);
        drv_iic_scl_set();
        receive <<= 1;
        sda_status = drv_gpio_read_bit(s_sda_port,s_sda_pin);
        if(sda_status == 1)
        {
            receive++;
        }
        else
        {
            
        }
        drv_delay_us(s_aa_time);
    }
    if(!ack )
    {
        drv_iic_no_ack_signal();
        
    }
    else
    {
        drv_iic_ack_signal();
    }
    return receive;
    
}


/**
  * @brief  drv_iic_check_device
  * @note   
  * @param  
  * @retval 
 */
bool drv_iic_check_device(uint8_t devie_address)
{
    bool status = TRUE;
    drv_iic_start_signal();
    drv_iic_send_byte(devie_address);
    status = drv_iic_wait_ack_signal();
    drv_iic_stop_signal();
    return status;
    
}
