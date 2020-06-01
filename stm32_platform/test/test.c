#include "hal.h" 

void test_key(void);
void test_led(void);
void adc_test(void);
uint8_t tx_buff[8] = {0};
uint8_t rx_buff[8] = {0};
uint8_t rx_len = 0;
bool status = FALSE;
int main()
{		
    uint8_t i=0;    
    hal_init();
   
	while(1)
    {
 
        i++;
        test_key();
        //test_led();
        hal_watchdog_feed();
        if(i>20)
        {
            //test_led();
            tx_buff[0]=0x02;
            tx_buff[1]=(uint8_t)((hal_adc_signal_get(ADC_3V3)&0xff00)>>8);
            tx_buff[2]=(uint8_t)(hal_adc_signal_get(ADC_3V3)&0x00ff);
            tx_buff[3]=0xFF;
            tx_buff[4]=0x04;
            tx_buff[5]=(uint8_t)((hal_adc_signal_get(ADC_GND)&0xff00)>>8);
            tx_buff[6]=(uint8_t)(hal_adc_signal_get(ADC_GND)&0x00ff);
            tx_buff[7]=0xFF;
            
            status = hal_usart_send(USART_1,tx_buff,8);
            i = 0;
            
        }
        adc_test();
        hal_set_vol_for_12b(2500);
        rx_len = hal_usart_read(USART_1,rx_buff,8,8);
        
        hal_delay_ms(10);               
		 
	}
   
	 
} 


void test_key(void)
{
    hal_di_input();
    
    
    if(g_di_signal_key1 == FALSE)
    {
        g_do_signal_led1 = 0;
    }
    if(g_di_signal_key0 == FALSE)
    {
        g_do_signal_led1 = 1;
    }
    //hal_do_output();

}


void test_led()
{
    static uint8_t status = TRUE;
    if(status == TRUE)
    {
        g_do_signal_led0 = 0;
        hal_do_output();
        status = FALSE;
    }
    else
    {
        g_do_signal_led0 = 1;
        hal_do_output();
        status = TRUE;
    }
}

void adc_test()
{
    hal_ai_read();
    (void)hal_adc_sequence_trigger();
}



