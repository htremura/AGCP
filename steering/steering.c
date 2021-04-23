/*
 * steering.c
 *
 * 
 *  Author: heitor tremura
 */ 

#include <avr/interrupt.h>		// 
#include "steering.h"			// 

// For steering
volatile int steps = 0;			// Global steps used to control how many pulses should come from pin
volatile int infinite = 0;		// Global infinite flag for when pulsing should continue infinitely

/////

// TIMERS FUNCTIONS

// For Steering

// Overflow Interrupt
ISR(TIMER1_COMPA_vect) {											// Interrupt Service Routine for whenever TCNT = OCR1A
	pulse_timer_isr();														// Call pulse_timer_isr
}																	// 

void pulse_timer_init(){													// Initialization value for pwm signal using TIMER1
	// pwm at PB1													// 
	DDRB |= (1 << DDB1);											// Set PB1 to output, else it will not be driven
	TCCR1A = (1 << COM1A0);											// Enable COM1A0 to signify that the output on OC1A(PB1) should be toggled whenever TCNT1 = OCR1A
	TCCR1B = (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);		// Set speed to clk/8 (prescaler)
	TIMSK1 = (1 << OCIE1A);											// Toggle output compare interrupt enable
}																	// 

void pulse_timer_isr() {													// The function that gets called by the TIMER1_COMPA_vect
	if (steps > 0) {												// If the steps are greater than 0 then we will
		steps--;													// subtract 1 from the steps, notating one toggle of voltage
	}																// 
	else if (!infinite) {											// If we reach 0 and the infinite flag is not set we will
		DDRB &= (0<<DDB1);											// Turn off DDB1 thereby stopping the driving of that pin by the timer
	}																// 
}																	// 

void pulse_timer_set_spd(int freq) {										// Function that sets the speed of the PWM to a given frequency
	OCR1A = (FOSC/(freq * 16))-3;									// Performs calculation on what OCR1A must be to produce frequency at clk/8 prescaling
//	DDRB |= (1 << DDB1);											// Enables DDB1, where the pulsing will be happening
}																	// 

void pulse(int p) {													// Function to pulse the motor a given number of times
	steps = 2*p;													// Multiply the input by 2 because we must toggle on AND off that many times
	if ((p == -1)) {steps = 0; infinite = 1;}						// If steps = -1 then we set steps to 0 and enable the infinite flag
	else {infinite = 0;}											// Turn off the infinite flag otherwise
	if (p != 0) DDRB |= (1 << DDB1);								// if steps != 0
	else if (p == 0) {
		DDRB &= (0 << DDB1);
	}
}																	// If we are not asking for 0 steps enable the output pin