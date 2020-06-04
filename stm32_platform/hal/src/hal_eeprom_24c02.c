/**
  ******************************************************************************
  * @file    hal_eeprom_24c02.c
  * @author  gray
  * @version V1.0.0
  * @date    14-May-2020
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
*/
#include "hal_eeprom_24c02.h"


/**
  * @brief  hal_eeprom_24c02_ck_ok
  * @note   
  * @param  
  * @retval TRUE IS OK ,FALSE IS NOT OK
 */
bool hal_eeprom_24c02_ck_ok(void)
{
    bool status = TRUE;
    status = drv_iic_check_device(EEPROM_24C02_ADDR);
    if(status == TRUE)
    {
        return status;
    }
    else
    {
        drv_iic_stop_signal();
        return status;
    }
}


/**
  * @brief  hal_eeprom_24c02_read
  * @note   
  * @param  p_data_buffer : read buff
  *         phy_start_addr : start addr (range is 0~255)
  *         length: read length
  * @retval TRUE IS OK ,FALSE IS NOT OK
 */
bool hal_eeprom_24c02_read(uint8_t *p_data_buffer, uint16_t phy_start_addr, uint16_t length)
{   
    bool status = TRUE;
    bool wait_ack_status;
    uint16_t i = 0 ;
    if((phy_start_addr+length)>PAGE_MAX)
    {
        status = FALSE;
        return status;
    }
    /*1. start signal*/
    drv_iic_start_signal();
    /*2. send device addr ,enter write mode*/
    drv_iic_send_byte((EEPROM_24C02_ADDR | W_CMD));
    /*3. wait slave ack*/
    wait_ack_status = drv_iic_wait_ack_signal();
    if(wait_ack_status == FALSE)
    {
        status = FALSE;
        drv_iic_stop_signal();
        return status;
    }
    /*4. send read addr,*/
    drv_iic_send_byte(phy_start_addr);
    /*5. wait slave ack*/
    wait_ack_status = drv_iic_wait_ack_signal();
    if(wait_ack_status == FALSE)
    {
        status = FALSE;
        drv_iic_stop_signal();
        return status;
    }
    /*6. start signal*/
    drv_iic_start_signal();
    /*2. send device addr,,enter read mode*/
    drv_iic_send_byte((EEPROM_24C02_ADDR | R_CMD));
    /*5. wait slave ack*/
    wait_ack_status = drv_iic_wait_ack_signal();   
    if(wait_ack_status == FALSE)
    {
        status = FALSE;
        drv_iic_stop_signal();
        return status;
    }
    /*8.ready receive*/
    for(i=0; i<length; i++)
    {
        /*feed dog*/
        hal_watchdog_feed();
        if(i != (length-1))
        {
            p_data_buffer[i] = drv_iic_receive_byte(1);
        }
        else
        {
            /*the last byte no ack*/
            p_data_buffer[i] = drv_iic_receive_byte(0);
        }
        
    }
    drv_iic_stop_signal();
    status = TRUE;
    return status;
}


/**
  * @brief  hal_eeprom_24c02_write
  * @note   page write ,each page 8 bytes , you must send stop signal after each page finished to write
  * @param  p_data_buffer : write buff
  *         phy_start_addr : start addr (range is 0~255)
  *         length: write length
  * @retval TRUE IS OK ,FALSE IS NOT OK
 */
bool hal_eeprom_24c02_write(const uint8_t p_data_buffer[], uint16_t phy_start_addr, uint16_t length)
{
    bool write_status = TRUE;
    bool wait_ack_status;
    uint16_t i = 0;
    hal_watchdog_feed();
    if((phy_start_addr+length)>PAGE_MAX)
    {
        write_status = FALSE;
        return write_status;
    }
    /*1. start signal*/
    drv_iic_start_signal();
    /*2. send device addr ,enter write mode*/
    drv_iic_send_byte((EEPROM_24C02_ADDR | W_CMD));
    /*3. wait slave ack*/
    wait_ack_status = drv_iic_wait_ack_signal();
    if(wait_ack_status == FALSE)
    {
        write_status = FALSE;
        drv_iic_stop_signal();
        return write_status;
    }
    /*4. send write addr,*/
    drv_iic_send_byte(phy_start_addr);
    /*5. wait slave ack*/
    wait_ack_status = drv_iic_wait_ack_signal();
    if(wait_ack_status == FALSE)
    {
        write_status = FALSE;
        drv_iic_stop_signal();
        return write_status;
    }
    for(i=0; i<length; i++)
    { 
        /*feed dog*/
        hal_watchdog_feed();
        /*6. send write addr,*/
        drv_iic_send_byte(p_data_buffer[i]);
        /*7. wait slave ack*/
        wait_ack_status = drv_iic_wait_ack_signal();
        if(wait_ack_status == FALSE)
        {
            write_status = FALSE;
            drv_iic_stop_signal();
            return write_status;
        }
        phy_start_addr++;
        if(((phy_start_addr&0x07) == 0)&&(i!=(length-1)))
        {
            /*send stop signal*/
            drv_iic_stop_signal();
            drv_delay_ms(10);
            /*restart*/
            /*1. start signal*/
            drv_iic_start_signal();
            /*2. send device addr ,enter write mode*/
            drv_iic_send_byte((EEPROM_24C02_ADDR | W_CMD));
            /*3. wait slave ack*/
            wait_ack_status = drv_iic_wait_ack_signal();
            if(wait_ack_status == FALSE)
            {
                write_status = FALSE;
                drv_iic_stop_signal();
                return write_status;
            }
            /*4. send write addr,*/
            drv_iic_send_byte(phy_start_addr);
            /*5. wait slave ack*/
            wait_ack_status = drv_iic_wait_ack_signal();
            if(wait_ack_status == FALSE)
            {
                write_status = FALSE;
                drv_iic_stop_signal();
                return write_status;
            }
            
        }
        
    }
    /*send stop signal*/
    drv_iic_stop_signal();
    drv_delay_ms(10);
    write_status = TRUE;
    return write_status;
}


/**
  * @brief  hal_eeprom_24c02_erase
  * @note   
  * @param  
  * @retval TRUE IS OK ,FALSE IS NOT OK
 */
bool hal_eeprom_24c02_erase(uint16_t phy_start_addr, uint16_t length)
{
    bool status =TRUE;
    uint8_t buff[256];
    uint16_t i = 0;
    if((phy_start_addr+length)>PAGE_MAX)
    {
        status = FALSE;
        return status;
    }
    for(i=0; i<length; i++)
    {
        buff[i] = 0x55;
    }
    status = hal_eeprom_24c02_write(buff,phy_start_addr,length);
    return status;
}


/**
  * @brief  hal_eeprom_24c02_erase
  * @note   
  * @param  
  * @retval TRUE IS empty ,FALSE IS NOT OK
 */
bool hal_eeprom_24c02_ck_empty(uint16_t phy_start_addr, uint16_t length)
{
    bool is_empty =TRUE;
    uint8_t receive[1];
    uint16_t i = 0;
    if((phy_start_addr+length)>PAGE_MAX)
    {
        is_empty = FALSE;
        return is_empty;
    }
    for (i = 0; (i < length) && (TRUE == is_empty); i++)
    {
        hal_eeprom_24c02_read(receive,phy_start_addr,1);
        if (0x55 != receive[0])
        {
            is_empty = FALSE;
        }
        phy_start_addr++;
    }
    return is_empty;
}
