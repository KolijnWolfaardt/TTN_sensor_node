#include <xc.h>
#include <string.h>
#include <pic18f26j50.h>

#include "hardware_init.h"
#include "timer.h"

#define timer_0_reset 23

int timer0_counter = 0;
int timer_0_cycle_counter = 0;

void setup_timer_0(void)
{
    // FOSC is 48 MHz
    T0CONbits.T08BIT = 1;   // Set 8 bit mode
    T0CONbits.T0CS = 0;     // Use Fosc/4
    T0CONbits.PSA = 0;      // Enable prescaler
    T0CONbits.T0PS = 0b000; // 32 prescalar
    
    INTCONbits.TMR0IF = 0; //Clear the flag
    INTCONbits.TMR0IE = 1; // Enable the timer interrupt
    
    T0CONbits.TMR0ON = 0; //Turn it on
    
}

void timer_0_callback()
{
    timer0_counter--;
    if (timer0_counter == 0)
    {
        timer_0_cycle_counter--;
        timer0_counter = timer_0_reset;
        if (timer_0_cycle_counter == 0)
        {
            T0CONbits.TMR0ON = 0;
        }
    }
}


void delayms(int num)
{
    timer_0_cycle_counter = num;
    timer0_counter = timer_0_reset;
    T0CONbits.TMR0ON = 1;
    while (timer_0_cycle_counter != 0);
}


void delay_ms_no_wait(int num)
{
    timer_0_cycle_counter = num;
    timer0_counter = timer_0_reset;
    T0CONbits.TMR0ON = 1;
}

void stop_timer()
{
    T0CONbits.TMR0ON = 0;
    timer_0_cycle_counter = 0;
    timer0_counter = timer_0_reset;
}
