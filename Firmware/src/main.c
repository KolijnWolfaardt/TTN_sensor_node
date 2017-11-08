/*
 * File:   newmain.c
 * Author: kolijn
 *
 * Created on 10 April 2017, 9:45 PM
 */
#include <stdio.h>
#include <string.h>

#include <xc.h>
#include "usb.h"
#include "analog.h"
#include "usb_config.h"
#include "usb_ch9.h"
#include "usb_cdc.h"
#include "config_bits.h"
#include "timer.h"
#include "uart.h"
#include "rtc.h"
#include "hardware_init.h"

#ifdef MULTI_CLASS_DEVICE
static uint8_t cdc_interfaces[] = { 0 };
#endif
char HEX_LOOKUP[] = {'0', '1', '2', '3', '4','5','6','7','8','9','A','B','C','D','E','F'};

// Constants that
char TRANSMIT_STRING[] = "mac tx uncnf 1 00000000\r\n";
char TRANSMIT_STR_LEN = 25;
char MAC_STATUS_STRING[] = "mac get status\r\n";
char MAC_STATUS_STR_LEN = 16;
char MAC_JOIN_STRING[] = "mac join abp\r\n";
char MAC_JOIN_STR_LEN = 14;
char RADIO_SLEEP_STRING[] = "sys sleep 1000\r\n";
char RADIO_SLEEP_STR_LEN = 17;
char MAC_CONFIG_STRING[] = "mac set dr 5\r\n";
char MAC_CONFIG_STR_LEN = 14;

//Possible response
char RESPONSE_INVALID[] = "invalid_param";
char RESPONSE_INVALID_LEN = 13;
char RESPONSE_OK[] = "ok";
char RESPONSE_OK_LEN = 2;
char RESPONSE_TX_OK[] = "mac_tx_ok";
char RESPONSE_TX_OK_LEN = 9;

int usb_counter = 0;
int usb_read_pos = 0;
char usb_buffer[256];

// Sensor values
int battery_voltage = 0;
int ldr_value = 0;
int temp_sens = 0;
int light_sensor = 0;

extern states_t CURRENT_STATE;

void reset_radio();
void turn_on_radio();
void add_atoi_to_usb(int num);
void sample_sensor_values();
void update_mac_status();
void send_values();
void sleep_radio();
int receive_value_matches();
int message_matches(char response_string[], int string_length);

char uart_message_buffer[16];

static void send_string_sync(uint8_t endpoint, const char *str)
{
	char *in_buf = (char*) usb_get_in_buffer(endpoint);

	while (usb_in_endpoint_busy(endpoint));

	strcpy(in_buf, str);
	/* Hack: Get the length from strlen(). This is inefficient, but it's
	 * just a demo. strlen()'s return excludes the terminating NULL. */
	usb_send_in_buffer(endpoint, strlen(in_buf));
}

int main(void)
{
    CURRENT_STATE = STARTUP;
    
    int counter = 0;
    // Load the settings from the sleep registers    
    if (CURRENT_STATE == STARTUP)
    {
        // Set up oscillators
        oscillator_init();

        // Set up peripherals like uart
        hardware_setup();
        // usb_init();
        
        setup_adc();
        setup_timer_0();
        
        led_write_num(0xF);
        delayms(3000);
        led_write_num(0x0);
                
        CURRENT_STATE = FIRST_RUN;
        // init_rtc();
        
        //SENSORS_ENABLE = 0;
    }
    
    RCONbits.IPEN = 1; // Enable priority on interrupts
    INTCONbits.GIE = 1; //Enable all interrupts
    INTCONbits.PEIE = 1; // Enable all peripheral interrupts
    
    
    SENSORS_ENABLE = 1;   
    
    while(1)
    {  
        int response_length = 0;
        
        led_write_num(CURRENT_STATE);

        if (CURRENT_STATE == FIRST_RUN)
        {
            // Not much here, just need to reset the radio
            CURRENT_STATE = RESET_RADIO;   
        }
        else if (CURRENT_STATE == WAKE_UP_RADIO)
        {
            // Re-sync the radio with a U character
            // This will also bring it out of sleep if it was
            //LED3 = 1;
            SENSORS_ENABLE = 0;
            turn_on_radio();
            delayms(1000);
            clear_message();
            CURRENT_STATE = GET_MAC_STATUS;
        }
        else if (CURRENT_STATE == GET_MAC_STATUS)
        {
            // Send a get mac status message. This is a special case, as the response should be parsed.
            response_length = send_and_wait(MAC_STATUS_STRING, MAC_STATUS_STR_LEN, 500);   

            if (response_length == -1)
            {
                CURRENT_STATE = HANDLE_ISSUE;
            }
            else
            {
                if (message_matches(RESPONSE_INVALID, RESPONSE_INVALID_LEN))
                {
                    CURRENT_STATE = HANDLE_ISSUE;
                    clear_message();
                }
                else
                {
                    // It was a valid message, and can be parsed
                    update_mac_status();
                    if (mac_state.join_status == 1)
                        CURRENT_STATE = SET_DR_VALUE;
                    else
                        CURRENT_STATE = JOIN_NETWORK;
                    clear_message();
                }
            }
        }
        else if (CURRENT_STATE == JOIN_NETWORK)
        {
            response_length = send_and_wait(MAC_JOIN_STRING, MAC_JOIN_STR_LEN, 500);

            if (response_length == -1)
            {
                CURRENT_STATE = HANDLE_ISSUE;
                clear_message();
            }
            else 
            {
                if (message_matches(RESPONSE_OK, RESPONSE_OK_LEN))
                {
                    // Wait for the accepted message
                    CURRENT_STATE = SET_DR_VALUE;
                    clear_message();
                }
                else
                {
                    CURRENT_STATE = HANDLE_ISSUE;
                    clear_message();
                }
            }
        }
        else if (CURRENT_STATE == SET_DR_VALUE)
        {
            response_length = send_and_wait(MAC_CONFIG_STRING, MAC_CONFIG_STR_LEN, 500);
            
            if (response_length == -1)
            {
                CURRENT_STATE = HANDLE_ISSUE;
            }
            else
            {
                if (message_matches(RESPONSE_OK, RESPONSE_OK_LEN)) 
                {
                    CURRENT_STATE = SEND_VALUES;
                    clear_message();
                }
                else
                {
                    CURRENT_STATE = HANDLE_ISSUE;
                    clear_message();
                }
            }
        }
        else if (CURRENT_STATE == SEND_VALUES)
        {
            sample_sensor_values();
            response_length = send_and_wait(TRANSMIT_STRING, TRANSMIT_STR_LEN, 500);

            if (response_length == -1)
            {
                CURRENT_STATE = HANDLE_ISSUE;
            }
            else
            {
                if (message_matches(RESPONSE_OK, RESPONSE_OK_LEN)) 
                {
                    CURRENT_STATE = GOTO_SLEEP;
                }
                else
                {
                    CURRENT_STATE = HANDLE_ISSUE;
                }
            }
        }
        else if (CURRENT_STATE == SEND_VALUES_RESPONSE)
        {
            // We are receiving the ok and mac_tx_ok directly after
            //each other , making it difficult to parse. For now, skip this step.
            //Wait for the secondary response
            uart1_wait_for_message(5000);

            if (message_matches(RESPONSE_TX_OK, RESPONSE_TX_OK_LEN)) 
            {
                CURRENT_STATE = GOTO_SLEEP;
            }
            else
            {
                // Why are we getting double 'ok' s?
                if (message_matches(RESPONSE_OK, RESPONSE_OK_LEN))
                {
                    CURRENT_STATE = GOTO_SLEEP;
                }
                else
                {
                    CURRENT_STATE = HANDLE_ISSUE;
                    //CURRENT_STATE = GOTO_SLEEP;
                }
            }
        }
        else if (CURRENT_STATE == GOTO_SLEEP)
        {
            clear_message();
            uart_send_message(RADIO_SLEEP_STRING, RADIO_SLEEP_STR_LEN, UART1);
            CURRENT_STATE = SLEEP;
            //LED3 = 0;
        }
        else if (CURRENT_STATE == SLEEP)
        {
            
            SENSORS_ENABLE = 1;
            // We have received an 'ok' from the radio, go to sleep
//            DSWAKEL = CURRENT_STATE;
//            WDTCONbits.REGSLP = 1;
//            OSCCONbits.IDLEN = 1;
//            while (RTCCFGbits.RTCSYNC == 1);
//            DSCONHbits.DSEN = 1;
//            asm("sleep");
            delayms(1000);
            delayms(1000);
            delayms(1000);
            delayms(1000);
            delayms(1000);
            CURRENT_STATE = WAKE_UP_RADIO;
            
        }
        else if (CURRENT_STATE == HANDLE_ISSUE)
        {
            // This will really depend on the issue. For now just reset,
            // And hope that that solves it
            //add_buffer_to_usb("Error, restarting\r\n", 19);
            CURRENT_STATE = RESET_RADIO;
        }
        else if (CURRENT_STATE == RESET_RADIO)
        {
            RADIO_RESET = 0;
            delayms(100);
            RADIO_RESET= 1;
            CURRENT_STATE = WAKE_UP_RADIO;
        }
        
        // Sends a message to the PC 
        if (usb_is_configured() && !usb_in_endpoint_halted(2) && !usb_in_endpoint_busy(2))
        {
            if (usb_counter != 0)
            {
                int i;
                unsigned char *buf = usb_get_in_buffer(2);
                
                if (usb_counter - usb_read_pos> 64)
                {
                    for (i=0; i < 64; i++)
                    {
                        buf[i] = usb_buffer[usb_read_pos+i];
                    }
                    usb_read_pos += 64;
                }
                else
                {
                    for (i=0; i < usb_counter - usb_read_pos; i++)
                    {
                        buf[i] = usb_buffer[usb_read_pos+i];
                    }
                    usb_read_pos = usb_counter;
                }

                usb_send_in_buffer(2, i);
                
                if (usb_counter == usb_read_pos)
                {  
                    usb_counter = 0;
                    usb_read_pos = 0;
                }
            }
        }
        
        // Handle data received from the host 
		if (usb_is_configured() && !usb_out_endpoint_halted(2) && usb_out_endpoint_has_data(2))
        {
			const unsigned char *out_buf;
			size_t out_buf_len;

			// Check for an empty transaction. 
			out_buf_len = usb_get_out_buffer(2, &out_buf);
			usb_arm_out_endpoint(2);
		}
        
        #ifndef USB_USE_INTERRUPTS
        usb_service();
		#endif
    }
}


void reset_radio()
{
    // Make the TX pin a output
    TXSTA1bits.TXEN = 0;
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 0;
    
    // Small Delay
    delayms(20);
    
    // Re-enable the transmitter
    TXSTA1bits.TXEN = 1;
    char temp_buffer[] = {0x55};
    uart_send_message(temp_buffer,1, UART1);
    while (uart1_busy() != true);
    clear_message();
}


void turn_on_radio()
{    
    // Awaken! Awaken!
    reset_radio();
//    add_char_to_usb('\r');
//    add_char_to_usb('\n');
    
    // Send some blank lines to clear the buffer
    uart_send_message("\r\n", 2, UART1);
    delayms(200);
    
    // Clear any possible message that might have been received
    clear_message();
}


void update_mac_status()
{
    // Check that the received message is long enough
    if (uart1_rx_buf_write_pos > 4){
        mac_state.join_status = 0;
    }
}


void sample_sensor_values()
{
//    add_buffer_to_usb("Sampling ADC values\r\n", 21);
    // Wait for the values to stabilize    
    battery_voltage = sample_value(BATTERY);
    ldr_value = sample_value(LDR);
    temp_sens = sample_value(TEMPERATURE);   
    light_sensor = sample_value(AMBIENT);
        
    TRANSMIT_STRING[15] = HEX_LOOKUP[(battery_voltage>>12) & 0xF];
    TRANSMIT_STRING[16] = HEX_LOOKUP[(battery_voltage>>8) & 0xF];
    TRANSMIT_STRING[17] = HEX_LOOKUP[(battery_voltage>>4) & 0xF];
    TRANSMIT_STRING[18] = HEX_LOOKUP[battery_voltage & 0xF];
    TRANSMIT_STRING[19] = HEX_LOOKUP[(temp_sens>>12) & 0xF];
    TRANSMIT_STRING[20] = HEX_LOOKUP[(temp_sens>>8) & 0xF];
    TRANSMIT_STRING[21] = HEX_LOOKUP[(temp_sens>>4) & 0xF];
    TRANSMIT_STRING[22] = HEX_LOOKUP[temp_sens & 0xF];
    
    // uart_send_message(TRANSMIT_STRING, 24, UART2);
}

interrupt high_priority void high_isr() {
#ifdef USB_USE_INTERRUPTS
    usb_service();
#endif

    if (INTCONbits.TMR0IF == 1) {
        timer_0_callback();
        INTCONbits.TMR0IF = 0;
    }
    if (PIR3bits.RTCCIF == 1) {
        rtc_interrupt();
        PIR3bits.RTCCIF = 0;
    }
    if (PIR1bits.TX1IF == 1) {
        uart_1_tx_interrupt();
    }
    if (PIR1bits.RC1IF == 1) {
        uart_1_rx_interrupt();
        PIR1bits.RC1IF = 0;
    }
    if (PIR3bits.TX2IF == 1) {
        uart_2_tx_interrupt();
    }
    if (PIR3bits.RC2IF == 1) {
        uart_2_rx_interrupt();
        PIR3bits.RC2IF = 0;
    }
}


void add_char_to_usb(char data)
{
    usb_buffer[usb_counter] = data;
    usb_counter++;
    if (usb_counter == 512)
    {
        usb_counter = 0;
    }
}

void add_buffer_to_usb(char buffer[], int length)
{
    int i = 0;
    for (i=0; i<length; i++)
    {
        usb_buffer[usb_counter] = buffer[i];
        usb_counter++;
    }
}

void add_atoi_to_usb(int num)
{
    char aString[4];

    // convert 123 to string [buf]
    
    aString[0] = (num/1000 % 10) + 0x30;
    aString[1] = (num/100 % 10) + 0x30;
    aString[2] = (num/10 % 10) + 0x30;
    aString[3] = (num % 10)  + 0x30;
    
    add_buffer_to_usb(aString, 4);
}

int message_matches(char response_string[], int string_length)
{
    // check that the received string is long enough
    if (uart1_rx_buf_write_pos < string_length)
    {
        return 0;
    }
    
    int i = 0;
    char matches = 1;
    for (i = 0; i < string_length; i++) {
        if (response_string[i] != uart_1_rx_buffer[i])
        {
            matches = 0;
        }
    }
    return matches;
}