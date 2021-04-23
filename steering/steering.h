/*
 * steering.h
 *
 * 
 *  Author: heitor tremura
 */ 

#ifndef STEERING_H_
#define STEERING_H_

#include "../comms/comms.h"

// TIMER1/PULSING
void pulse_timer_init();
extern void pulse_timer_isr();
void pulse_timer_set_spd(int);
volatile int steps;
void pulse();
volatile int infinite;

#endif /* STEERING_H_ */