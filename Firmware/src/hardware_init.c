#include <xc.h>
#include "hardware_init.h"
#include "uart.h"

void oscillator_init(void);
void hardware_setup(void);
void setup_uart_1(void);
void led_write_num(int num);

void oscillator_init(void)
{
    unsigned int pll_startup = 600;
	OSCTUNEbits.PLLEN = 1;
	while (pll_startup--);
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void hardware_setup(void)
{
    // Setup LED pins as Outputs
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    
    // Setup the Buttons as inputs
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB6 = 1;
    
    // Set sensor enable pin as output
    TRISBbits.TRISB5 = 0;
    
    // disable analog on LED pin
    ANCON1bits.PCFG9 = 0x1;
    
    // disable analog on button pin
    ANCON1bits.PCFG12 = 0x1;
    
    // Setup uart 1 - for communicating with the radio IC
    setup_uart_1();
    setup_uart_2();
    
    // Set the radio Reset pin
    TRISAbits.TRISA5 = 0;
    ANCON0bits.PCFG4 = 0x1;
    RADIO_RESET = 1;
    
    // Set the analog pins tris bits to input
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    ANCON0bits.PCFG0 = 0;
    ANCON0bits.PCFG1 = 0;
    ANCON0bits.PCFG2 = 0;
    ANCON0bits.PCFG3 = 0;
    
        
    led_write_num(0);
}

void led_write_num(int num)
{
    LED0 = (num) & 0x1;
    LED1 = (num >> 1) & 0x1;
    LED2 = (num >> 2) & 0x1;
    LED3 = (num >> 3) & 0x1;
}

