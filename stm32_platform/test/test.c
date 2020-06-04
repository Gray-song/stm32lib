#include "headers.h" 

uint8_t rx_buff[8] = {0};
uint8_t rx_len = 0;
bool status = FALSE;
int main()
{		 
    hal_init();
    sync_mcu_version_info();    
	while(1)
    {
        hal_watchdog_feed();
       
        test_key();       
        adc_dac_test();
        adc_result_response();
        send_mcu_version_info();
        send_pwm_info();
        rx_len = hal_usart_read(USART_1,rx_buff,8,8);    
        
        hal_delay_ms(10);               
		 
	}
   
	 
} 





