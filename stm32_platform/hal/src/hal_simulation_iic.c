/**
  ******************************************************************************
  * @file    hal_simulation_iic.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_simulation_iic.h"
/**
  * @brief  hal_simulation_iic_init
  * @note   None
  * @param  
  * @retval None
 */
void hal_simulation_iic_init(void)
{
    simulation_iic_init_t iic_init_struct;
    iic_init_struct.scl_pin = GPIO_Pin_12;
    iic_init_struct.scl_port = GPIO_C;
    iic_init_struct.sda_pin = GPIO_Pin_11;
    iic_init_struct.sda_port = GPIO_C;
    iic_init_struct.simu_iic_time.t_aa_us = 1;
    iic_init_struct.simu_iic_time.t_hd_sta_us = 4;
    iic_init_struct.simu_iic_time.t_high_us = 2;
    iic_init_struct.simu_iic_time.t_low_us = 4;
    iic_init_struct.simu_iic_time.t_su_sta_us = 4;
    iic_init_struct.simu_iic_time.t_su_sto_us = 4;
    drv_iic_simulation_init(&iic_init_struct);
}
