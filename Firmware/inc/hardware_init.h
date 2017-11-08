/* 
 * File:   hardware_init.h
 * Author: kolijn
 *
 * Created on 20 April 2017, 6:48 AM
 */

#include <xc.h>

#ifndef HARDWARE_INIT_H
#define	HARDWARE_INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

void oscillator_init(void);
void hardware_setup(void);
void led_write_num(int num);
void add_char_to_usb(char data);
void add_buffer_to_usb(char buffer[], int length);

#define LED0 LATCbits.LATC0
#define LED1 LATCbits.LATC1
#define LED2 LATBbits.LATB3
#define LED3 LATBbits.LATB4

#define BAT_SENS_DISABLE LATCbits.LATC2
#define SENSORS_ENABLE LATBbits.LATB5

#define BUTTON0 PORTBbits.RB0
#define BUTTON1 PORTBbits.RB6

#define RADIO_RESET LATAbits.LATA5

typedef enum  
{
    STARTUP=1,
    INIT_HARDWARE=2,
    FIRST_RUN=3,
    WAKE_UP_RADIO=4,
    GET_MAC_STATUS=5,
    JOIN_NETWORK=6,
    SET_DR_VALUE=7,
    SEND_VALUES=8,
    SEND_VALUES_RESPONSE=9,
    GOTO_SLEEP=10,
    SLEEP=0,
    HANDLE_ISSUE=14,
    RESET_RADIO=15,
}states_t;

states_t CURRENT_STATE = STARTUP;

struct {
    unsigned int join_status : 1;
    unsigned int mac_state : 3;
} mac_state;

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_INIT_H */

