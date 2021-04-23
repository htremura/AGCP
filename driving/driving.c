/*
 * driving.c
 *
 * 
 *  Author: heitor tremura
 */ 

#include <stdio.h>				// Include standard libraries
#include <avr/io.h>				// 
#include <avr/interrupt.h>		// 
#include "comms.h"				// 

// TIMERS FUNCTIONS

// For Driving

// Overflow Interrupt
ISR(TIMER1_OVF_vect) {
	pwm_isr();
}																// 

void pwm_init(){
	// pwm at PB1
	DDRB |= (1 << DDB1);
	// set PWM duty cycle as 10bit
	TCCR1A = (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);				// Set WGM0 and WGM1 to Fast PWM 10bit mode as well as clearing 0CR1A on compare match
	TCCR1B = (1 << WGM12)  | (0 << CS12) | (1 << CS11) | (0 << CS10);	// Set initial speed to clk/8 (prescaler)
	TIMSK1 = (1 << TOIE1);												// Toggle overflow interrupt enable bit
}																// 

void pwm_isr() {													// The function that gets called by the TIMER1_OVF_vect

}																	// 

void pwm_set_duty(int d) {
	int duty = (0x03FF/100)*d;
	OCR1A = duty & (0x03FF);
	DDRB |= (1 << DDB1);
}																	// 
