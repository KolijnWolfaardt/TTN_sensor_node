#include <xc.h>
#include <string.h>
#include <pic18f26j50.h>

#include "hardware_init.h"
#include "timer.h"
#include "analog.h"

void setup_adc()
{    
    ADCON0bits.VCFG0 = 0;       // Positive reference is AVdd
    ADCON0bits.VCFG1 = 0;       // Negative reference is AVss
    
    ADCON1bits.ADFM = 1;        // Right justified   
    ADCON1bits.ACQT = 0b110;    //16 T_AD
    ADCON1bits.ADCS = 0b110;    // Fosc/64
    
    ANCON1bits.VBGEN = 1;       // Turn on the bad gap reference
    
    ADCON0bits.ADON = 1; 
}


int sample_value(int source)
{
    if (source == BATTERY)
        ADCON0bits.CHS = 0b0000;
    if (source == LDR)
        ADCON0bits.CHS = 0b0001;
    if (source == TEMPERATURE)
        ADCON0bits.CHS = 0b0010;
    if (source == AMBIENT)
        ADCON0bits.CHS = 0b0011;
    if (source == CALIBRATE)
        ADCON1bits.ADCAL = 1;
    
    //Should wait a little bit
    delayms(1);
    
    // Enable the ADC convertor
    // Wait for the conversion to finish
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1);
    
    return ADRES;
}