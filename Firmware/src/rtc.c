#include <xc.h>
#include <string.h>
#include <pic18f26j50.h>

#include "hardware_init.h"
#include "uart.h"

void init_rtc();
void rtc_interrupt();

extern states_t CURRENT_STATE;

void init_rtc()
{    
    /*
     * The RTC module is used to generate an interrupt every minute, or 
     * whatever the sampling time is going to be. As such, we do not need
     * accurate time info, only accurate timing.
     */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    RTCCFGbits.RTCWREN = 1; //Enable writing to the registers
    
    ALRMCFGbits.CHIME = 1;      // Roll-over the counter
    ALRMCFGbits.AMASK = 0b0010; // Every minute, for debugging

    PIR3bits.RTCCIF = 0;        // Clear the interrupt
    IPR3bits.RTCCIP = 0;        // High priority
    PIE3bits.RTCCIE = 1;        // Enable the interrupt
    
    ALRMVALL = 0x02;
    ALRMRPT = 5;
    
    ALRMCFGbits.ALRMEN = 1;     // Enable the alarm
    RTCCFGbits.RTCEN = 1;       // Enable the RTC module
    
}

void rtc_interrupt()
{    
    PIR3bits.RTCCIF = 0;        // Clear the interrupt
    //Clear the flag
    CURRENT_STATE = WAKE_UP_RADIO;
}