/*
 * comms.h
 *
 * 
 *  Author: heitor tremura
 */ 

#ifndef COMMS_H_
#define COMMS_H_

#include <stdio.h>

// USER-MODIFIABLE CONSTANTS FOR USE IN LIBRARY
#define FOSC 16000000				// Clock Speed
#define BAUD 57600					// Desired Baud Rate
#define END_OF_MSG_CHAR	'='			// Character to end message
#define RESET_MSG_CHAR	'|'			// Character which resets message without processing it
#define BUFLEN 80					// Maximum size of message

// DO NOT CHANGE
#define MYUBRR (FOSC/16/BAUD-1)		// UBRR necessary for desired Baud Rate

// Redirect printf & scanf
extern FILE uart_input, uart_output;
extern void rx_done_callback(char *);
extern void rx_error_callback();

/////

// PORTB
void portb_init();
void portb_write(uint8_t x);
void portb_bit_set(uint8_t x);
void portb_bit_clear(uint8_t x);
// PORTD
void portd_init();
void portd_write(uint8_t x);
void portd_bit_set(uint8_t x);
void portd_bit_clear(uint8_t x);

/////

// TIMER1/PWM
void pulse_timer_init();
extern void pulse_timer_isr();

/////

// USART
void usart_init();
void usart_tx(unsigned char);
unsigned char usart_rx( void );
void usart_printchar(char, FILE *);
char usart_scanchar(FILE *);
void usart_redir();
void usart_isr();
int len;

/////

//Pulsing for steering
void pulse_timer_init();
void pulse_timer_set_spd(int);
void pulse_timer_isr();
volatile int steps;
void pulse();
volatile int infinite;

#endif /* COMMS_H_ */