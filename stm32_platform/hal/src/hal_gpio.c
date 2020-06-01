/**
  ******************************************************************************
  * @file    hal_gpio.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_gpio.h"
bool g_do_signal_led0 = 1;
bool g_do_signal_led1 = 1;
bool g_di_signal_key0 = 1;
bool g_di_signal_key1 = 1;
bool g_di_signal_wk_up = 0;
static void hal_dio_init_signal(io_signal_name_t signl);
static const io_config_t io_config_array[IO_SIGNALS_COUNT] = 
{
    IO_SIGNALS_CONFIG
};

static const digital_output_temp_t digital_output_cfg_array[DO_SIGNALS_COUNT] = 
{
    APP_DO_CONFIG
};

static  digital_input_temp_t digital_input_cfg_array[DI_SIGNALS_COUNT] = 
{
    APP_DI_CONFIG
};

static void hal_dio_init_signal(io_signal_name_t signl)
{
    gpio_init_t gpio_init_struct;
    uint8_t i;

    i = (uint8_t)signl;       
    if(i < IO_SIGNALS_COUNT)
    {
      gpio_init_struct.mode = io_config_array[i].mode;
      gpio_init_struct.pin = io_config_array[i].pin;
      gpio_init_struct.port = io_config_array[i].port;
      gpio_init_struct.speed = io_config_array[i].speed;
      drv_gpio_init(&gpio_init_struct);            
    }
} 


/**
  * @brief  gpio init
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_gpio_init(void)
{
    uint8_t i=0;
    for(i=0 ; i<IO_SIGNALS_COUNT; i++)
    {
        hal_dio_init_signal((io_signal_name_t)i);
    }
    hal_do_output();
}


/**
  * @brief  hal_do_write_signal
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_do_write_signal(io_signal_name_t signal, bool status)
{
    if((uint8_t)signal < IO_SIGNALS_COUNT)
    {
        if (DIO_ACTIVE_LOW == io_config_array[(uint8_t)signal].active_level)
        {
            /* toggle target level for active low */
            if (TRUE == status)
            {
                status = FALSE;
            }
            else
            {
                status = TRUE;
            }
        }
        drv_gpio_write_bit(io_config_array[(uint8_t)signal].port
                           ,io_config_array[(uint8_t)signal].pin,status);
    }
}    


/**
  * @brief  hal_di_read_signal
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
io_err_t hal_di_read_signal(io_signal_name_t signal, bool *p_status)
{
    io_err_t io_err = IO_ERR_OK;
    bool status;
    uint8_t i;
    i = (uint8_t)signal; 
    if(i < IO_SIGNALS_COUNT)
    {
        status = drv_gpio_read_bit(io_config_array[i].port,io_config_array[i].pin);
        if (DIO_ACTIVE_LOW == io_config_array[i].active_level)
        {
            /* toggle target level for active low */
            if (TRUE == status)
            {
                status = FALSE;
            }
            else
            {
                status = TRUE;
            }
        }
        *p_status = status;
        io_err = IO_ERR_OK;
    }
    else
    {
        io_err = IO_ERR_PARAMETER_ERROR;
    }
    return io_err;
}


/**
  * @brief  hal_do_output
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_do_output(void)
{
    uint8_t i;
    for(i=0; i<DO_SIGNALS_COUNT; i++)
    {
        hal_do_write_signal(digital_output_cfg_array[i].sig_name,*(digital_output_cfg_array[i].p_var));
    }
}


/**
  * @brief  hal_di_input
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_di_input(void)
{
    uint8_t i;
    bool cur_status;
    for(i=0; i<DI_SIGNALS_COUNT; i++)
    {
        hal_di_read_signal(digital_input_cfg_array[i].sig_name,&cur_status);
        if(cur_status == digital_input_cfg_array[i].original_status )
        {
            if(digital_input_cfg_array[i].key_delay_remain > 0)
            {
                digital_input_cfg_array[i].key_delay_remain-- ;
            }
            else
            {
            
            }
            
            if(digital_input_cfg_array[i].key_delay_remain == 0)
            {
                *(digital_input_cfg_array[i].p_var) = cur_status;
            }
            else
            {
            
            }
        }
        else
        {
            if (0 == digital_input_cfg_array[i].key_delay_init)
            {
                *(digital_input_cfg_array[i].p_var) = cur_status;
            }
            else
            {
                digital_input_cfg_array[i].key_delay_remain = digital_input_cfg_array[i].key_delay_init;
                digital_input_cfg_array[i].original_status = cur_status;
            }
        }
    }
}


/**
  * @brief  hal_exti5_isr
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void hal_exti5_isr(void)
{
     hal_di_input();
    if(g_di_signal_key0 == FALSE)
    {
        g_do_signal_led0 =!g_do_signal_led0;
         
    }
    
    hal_do_output();
}
