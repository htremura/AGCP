/*
 * main.c
 *
 * 
 * Author : heitor tremura
 */ 

#include "steering.h"			// Include definitions of all vars
#include <avr/interrupt.h>	// Include interrupts library
#include <stdio.h>			// Include library for C terminal communication
#include <string.h>			// Include library for working with strings.
#include <stdlib.h>			// Include standard library.

// PINS!!!
// 2/PORTD2 IS DISABLE
// 7/PORTD7 IS DIRECTION
// 9/PORTB1 IS PULSING
#define disable 2
#define direction 7
#define pulsing 9
int ovf;

// TIMER0/DEBUG
void timerdel_init(){
	TIMSK0 |= (1 << TOIE0);
}
void timerdel_start(){
	ovf = 0;
	TCCR0B |= (1 << CS00);
}
void timerdel_stop(){
	TCCR0B &= ~(1 << CS00);
}
void timerdel_print(){
	printf("%d", (TCNT0+256*ovf));
}
void timerdel_rst(){
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

void rx_done_callback(char *rxbuf) {									// Arduino will perform some functionality when END_OF_MSG char is detected
	timerdel_start();													// Start the delay timer
	cli();																// Disable interrupts while processing is happening
//	printf("MCU received %s\n", rxbuf);									// Reply to received message with the information in the buffer (The received message)
//	printf("=\n");														// Send confirmation of message receive
	len = strlen(rxbuf);												// Save length of string to determine behavior
	
	if(rxbuf[0] == 'S' || rxbuf[0] == 's' ) {							// If the first char is 'S' we will STOP the steering motor
		portd_bit_clear(direction);										// DIRECTION LOW
		portd_bit_write(disable);										// DISABLE HIGH
		pulse(0);														// Stops Pulsing
		sei();															// Enable interrupts
//		printf("STOPPING\n");											// Print Message to serial saying we are STOPPING
	}
	
	else if(rxbuf[0] == 'T' || rxbuf[0] == 't' ) {						// If the first char is 'T' we will pulse to turn
																		//
		if(rxbuf[1] == 'R' || rxbuf[1] == 'r' ) {						// If the second char is R we will turn Right
			portd_bit_clear(disable);									// DISABLE LOW
			portd_bit_clear(direction);									// DIRECTION LOW
			pulse(findnum(rxbuf));										// Pulses a number of times
			sei();														// Enable interrupts
//			printf("TURNING RIGHT %d PULSES\n", (findnum(rxbuf)));		// Print Message to serial saying we are turning right
		}																//
																		//
		else if(rxbuf[1] == 'L' || rxbuf[1] == 'l' ) {					// If the second char is L we will turn Left
			portd_bit_clear(disable);									// DISABLE LOW
			portd_bit_write(direction);									// DIRECTION HIGH
			pulse(findnum(rxbuf));										// Pulses a number of times
			sei();														// Enable interrupts
//			printf("TURNING LEFT %d PULSES\n", (findnum(rxbuf)));		// Print Message to serial saying we are turning right
		}																//
		else {
			sei();														// Enable interrupts
//			printf("Invalid Turning Message\n");						// Print Message to serial saying we are going VERY FAST
		}
	}																	//
																		//
	else if(rxbuf[0] == 'V' || rxbuf[0] == 'v' ) {						// If the first char is 'V' we will be changing the speed of rotation
																		//
		if(rxbuf[1] == 'F' || rxbuf[1] == 'f' ) {						// If the next char is 'F' we will be changing to a fast speeds
			if (len == 2) {												// If the message is 2 characters we are going FAST
				pulse_timer_set_spd(1500);										// Spd = 1.5khz
				sei();													// Enable interrupts
//				printf("GOING FAST\n");									// Print Message to serial saying we are going FAST
			}															//
			else if(rxbuf[2] == 'F' || rxbuf[2] == 'f' ) {				// If the there are not 2 characters and the third character is F we are going very fast
				pulse_timer_set_spd(2000);										// Spd = 2khz
				sei();													// Enable interrupts
//				printf("GOING VERY FAST\n");							// Print Message to serial saying we are going VERY FAST
			}															//
			else {														//
				sei();													// Enable interrupts
//				printf("Invalid Fast Velocity Message\n");				// Print Message to serial saying we are going VERY FAST
			}
		}																//
																		//
		else if(rxbuf[1] == 'M' || rxbuf[1] == 'm' ) {					// If the next char is 'M' we will be changing to MEDIUM rotation
			pulse_timer_set_spd(1000);											// Spd = 1khz
			sei();														// Enable interrupts
//			printf("GOING MEDIUM\n");									// Print Message to serial saying we are going medium
		}																//
																		//
		else if(rxbuf[1] == 'S' || rxbuf[1] == 's' ) {					// If the next char is 'S' we will be changing to SLOW rotation
			pulse_timer_set_spd(500);											// Spd = 500hz
			sei();														// Enable interrupts
//			printf("GOING SLOW\n");										// Print Message to serial saying we are going slow
		}																//
																		//
		else if(rxbuf[1] == 'C' || rxbuf[1] == 'c' ) {					// If the next char is 'C' we will be changing to a CUSTOM rotation
			int spd = findnum(rxbuf);									// Find the desired frequency
			if (spd < 16) {												// If the frequency is below 16, set it to 16
				spd = 16;												// 
			}															// 
			else if (spd > 2047) {										// If the frequency is above 2047 reset it to 2047
				spd = 2047;												// 
			}															// 
			pulse_timer_set_spd(spd);											// spd = custom
			sei();														// Enable interrupts
//			printf("GOING %d hz\n", spd);								// Print Message to serial saying we are going slow
		}																//
		else {															//
			sei();														// Enable interrupts
//			printf("Invalid Velocity Message\n");						// Print a message if an invalid velocity message is received
		}																//
	}																	//
	else {																//
		sei();															// Enable interrupts
//		printf("Invalid Message\n");									//
	}																	//
	timerdel_stop();													// Stop the delay timer
	timerdel_print();													// Print the delay timer
	timerdel_rst();														// Reset the delay timer
																		//
}																		//

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
	
	// Initialize UART
	usart_init();
	usart_redir();
	pulse_timer_init();
	pulse_timer_set_spd(1000);											// Spd = 1khz
	timerdel_init();											// Enable Delay Timer
	sei();														// Enable interrupts
	portd_bit_write(disable);
	
    while (1);
	
}

