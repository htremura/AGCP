/*
 * driving.h
 *
 * 
 *  Author: heitor tremura
 */ 

#ifndef DRIVING_H_
#define DRIVING_H_

#include "../comms/comms.h"

// TIMER1/PULSING/PWM
void pwm_init();
extern void pwm_isr();
void pwm_set_duty(int d);
void pwm_start_motor();
void pwm_stop_motor();

#endif /* DRIVING_H_ */