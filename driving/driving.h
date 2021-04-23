/*
 * driving.h
 *
 * 
 *  Author: heitor tremura
 */ 

#ifndef DRIVING_H_
#define DRIVING_H_

#include "comms.h"

// TIMER1/PULSING/PWM
void pwm_init();
extern void pwm_isr();
void pwm_set_duty(int d);

#endif /* DRIVING_H_ */