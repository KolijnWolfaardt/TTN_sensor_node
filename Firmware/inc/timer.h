/* 
 * File:   timer.h
 * Author: kolijn
 *
 * Created on 17 June 2017, 12:29 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

void setup_timer_0(void);
void timer_0_callback();
void delayms(int num);
void delay_ms_no_wait(int num);
void stop_timer();

extern int timer_0_cycle_counter;

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

