/* 
 * File:   analog.h
 * Author: kolijn
 *
 * Created on 15 August 2017, 10:30 PM
 */

#ifndef ANALOG_H
#define	ANALOG_H

enum analog_inputs{
    BATTERY = 0,
    LDR,
    TEMPERATURE,
    AMBIENT,
    CALIBRATE
};

void setup_adc();
int sample_value(int source);

#endif	/* ANALOG_H */

