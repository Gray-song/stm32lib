/**
  ******************************************************************************
  * @file    gpio_test_demo.h
  * @author  gray
  * @version V1.0.0
  * @date    4-June-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "headers.h"
uint8_t update_mcu_vesion[6] = {MCU_VERSION_A,MCU_VERSION_B,MCU_VERSION_C,YEAR,MONTH,DAY};
uint8_t eeprom_mcu_vesion[6]= {0};


/**
  * @brief  sync_mcu_version_info
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void sync_mcu_version_info(void)
{
    uint8_t i = 0;
    hal_eeprom_24c02_read(eeprom_mcu_vesion,0,6);
    for(i=0; i<6; i++)
    {
        if(eeprom_mcu_vesion[i]!=update_mcu_vesion[i])
        {
            hal_eeprom_24c02_write(update_mcu_vesion,0,6);
            hal_eeprom_24c02_read(eeprom_mcu_vesion,0,6);
            break;
        }
    }
   
}


/**
  * @brief  send_mcu_version_info
  * @note   
  * @param  
  * @retval None
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
 */
void send_mcu_version_info(void)
{
    static uint8_t i=0;
    if(i>VERSION_SEND_PERIOD_200MS)
    {
        hal_usart_send(USART_1,eeprom_mcu_vesion,6);
        i=0;
    }
    else
    {
        i++;
    }
    
}
