/*
 * main.c
 *
 * 
 * Author : heitor tremura
 */ 

#include "driving.h"		// Include definitions of all vars
#include <avr/interrupt.h>	// Include interrupts library
#include <stdio.h>			// Include library for C terminal communication
#include <string.h>			// Include library for working with strings.
#include <stdlib.h>			// Include standard library.

// PINS!!!
// 2/PORTD2 IS DISABLE
// 7/PORTD7 IS DIRECTION
// 9/PORTB1 IS PULSING
#define ReverseSwitch 4
#define ForwardSwitch 5
#define direction 7
#define pulsing 9
int ovf;
void pcint_init() {
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1 << PCINT20) | (1 << PCINT21);
}

ISR(PCINT20_vect) {
	if(~(PIND == (PIND & ~((1<<ForwardSwitch) | (1<<ReverseSwitch))))) {
		pwm_set_duty(0);
		DDRB &= ~(1 << pulsing-8);
		portd_bit_clear(direction);
	}
}

ISR(PCINT21_vect) {
	if(~(PIND == (PIND & ~((1<<ForwardSwitch) | (1<<ReverseSwitch))))) {
		pwm_set_duty(0);
		DDRB &= ~(1 << pulsing-8);
		portd_bit_clear(direction);
	}
}

// TIMER0/DEBUG
void timerdel_init(){
	TIMSK0 |= (1 << TOIE0);
}
void timerdel_start(){
	TCCR0B |= (1 << CS00);
}
void timerdel_stop(){
	TCCR0B &= ~(1 << CS00);
}
void timerdel_print(){
	printf("%d", (TCNT0+256*ovf));
}
void timerdel_rst(){
	ovf = 0;
	TCNT0 = 0;
}
ISR(TIMER0_OVF_vect) {
	ovf++;
}

// Message Parsing

int findnum(char *rxbuf) {							// Function for selecting the number that comes after a 2 char message
	int num;
	if (len == 2) {									// If string is only 2 chars we know to pulse infinitely
		num = -1;
	}
	else if (len > 3) {								// If string is more than 3 chars we know we will need to determine the number of pulses
		char s1 [80];								// Create character buffers we will use to concatenate the integers
		char s2 [80];								//
		sprintf(s1, "%d", rxbuf[2]-'0');			// Store the first number in the buffer as a string
		for (int i = 3; i <= len-1; ++i)			// For loop to find other numbers if there are any
		{
			sprintf(s2, "%d", rxbuf[i]-'0');		// Store the next number in its own buffer
			strcat(s1, s2);							// Concatenate the buffers
		}
		num = atoi(s1);								// Convert the string back to an integer
	}
	else {
		num = rxbuf[2]-'0';							// If the string is exactly 3 chars we will only return the third as an integer
	}
	return num;
}

void rx_done_callback(char *rxbuf) {										// Arduino will perform some functionality when END_OF_MSG char is detected
	timerdel_start();														// Start the delay timer
	cli();																	// Disable interrupts while processing is happening
//	printf("MCU received %s\n", rxbuf);										// Reply to received message with the information in the buffer (The received message)
//	printf("=\n");															// Send confirmation of message receive
	len = strlen(rxbuf);													// Save length of string to determine behavior
	
	if((rxbuf[0] == 'S' || rxbuf[0] == 's' )) {								// If the first char is 'S' we will STOP the driving motor
		pwm_set_duty(0);
		DDRB &= ~(1 << pulsing-8);
		portd_bit_clear(direction);
	}																		//
	else if(PIND == (PIND & ~((1<<ForwardSwitch) | (1<<ReverseSwitch)))) {	//
		if(rxbuf[0] == 'F' || rxbuf[0] == 'f') {							// If the first char is 'F' we will be driving forward
			int d = findnum(rxbuf);											//
			if (d > 80) d = 80;												//
			if (d < 20) d = 20;												//
			pwm_set_duty(d);												//
			portd_bit_clear(direction);										//
			DDRB |= (1 << pulsing-8);										//
		}																	//
		else if(rxbuf[0] == 'R' || rxbuf[0] == 'r') {						// If the first char is 'R' we will be reversing
			int d = findnum(rxbuf);											//
			if (d > 80) d = 80;												//
			if (d < 20) d = 20;												//
			pwm_set_duty(d);												//
			portd_bit_set(direction);										//
			DDRB |= (1 << pulsing-8);										//
		}																	//
	}																		//
	else {																	//
//		printf("Invalid Message\n");										//
	}																		//
	sei();																	// Enable interrupts
	timerdel_stop();														// Stop the delay timer
	timerdel_print();														// Print the delay timer
	timerdel_rst();															// Reset the delay timer
																			//
}																			//

// Return a ? if the message in the buffer is too large.
void rx_error_callback(char *rxbuf) {
	
	// error handler	
	printf("?\n");	// error
}

// main()
int main(void)
{
	
	// Initialize digital I/O init PORTB
	portb_init();
	portb_write(0x00);
	
	portd_init();
	portd_write(0x00);
	DDRD &= ~((1<<ForwardSwitch) | (1<<ReverseSwitch));
	
	// Initialize UART
	usart_init();
	usart_redir();
	pwm_init();
	pcint_init();
	pwm_set_duty(0);											// Turn off driving when init
	timerdel_init();											// Enable Delay Timer
	sei();														// Enable interrupts
	DDRB &= ~(1 << pulsing-8);
	
    while (1);
	
}

