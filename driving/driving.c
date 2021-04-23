/*
 * driving.c
 *
 * 
 *  Author: heitor tremura
 */ 

#include <stdio.h>				// Include standard libraries
#include <avr/io.h>				// 
#include <avr/interrupt.h>		// 
#include "driving.h"				// 

// Create filestreams for usart_output and usart_input
FILE usart_output = FDEV_SETUP_STREAM(usart_printchar, NULL, _FDEV_SETUP_WRITE);
FILE usart_input = FDEV_SETUP_STREAM(NULL, usart_scanchar, _FDEV_SETUP_READ);

// For communication system
char rxbuf[BUFLEN];				// Declare globally used buffer of text
int iend = 0;					// Declare globally used location of character in buffer

// For driving
volatile int steps = 0;			// Global steps used to control how many pulses should come from pin
volatile int infinite = 0;		// Global infinite flag for when pulsing should continue infinitely

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PORT initialization functions

// PORTB
void portb_init(){					// Initialization function which sets DDRB to 0xff, i.e. all output
	DDRB = 0xff;					// 
}									//
void portb_write(uint8_t x){		// Function to hard write to PORTB
	PORTB = x;						//
}									//
void portb_bit_set(uint8_t x){		// Function to OR into PORTB
	PORTB |= (1 << x);				//
}									//
void portb_bit_clear(uint8_t x){	// Function to AND into PORTB
	PORTB &= ~(1 << x);				// 
}									// 
									// 
// PORTD							// 
void portd_init(){					// Initialization function which sets DDRD to 0xff, i.e. all output
	DDRD = 0xff;					// 
}									// 
void portd_write(uint8_t x){		// Function to write to PORTD
	PORTD = x;						// 
}									//
void portd_bit_set(uint8_t x){		// Function to OR into PORTD
	PORTD |= (1 << x);				//
}									//
void portd_bit_clear(uint8_t x){	// Function to AND into PORTD
	PORTD &= ~(1 << x);				//
}									//

///////////////////////////////////////////////////////////////////////////////////////////////////////

//ISR(BADISR_vect){
//__asm__ __volatile__("nop");
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////

// USART FUNCTIONS
ISR(USART_RX_vect) {								// Interrupt on receive complete handler
	usart_isr();									// Perform the desired function defined in usart_isr()
};

void usart_init(){									// Initialize the USART communication
	unsigned int ubrr = MYUBRR;						// Set ubrr variable to value defined in driving.h
	UBRR0H = (unsigned char)(ubrr>>8);				// Set the high byte of our UBRR value to the one defined in driving.h
	UBRR0L = (unsigned char)(ubrr);					// Set the low byte of our UBRR value to the one defined in driving.h
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);					// Enable Receiving and Transmitting on Serial Line
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);				// Set frame format: 8bit data with 1 stopbit
													//
	UCSR0B |= (1 << RXCIE0);						// Enable interrupt on USART receive complete
} 

void usart_tx( unsigned char data ) {				// Use the serial buffer to transmit data to the PC
	while ( !( UCSR0A & (1<<UDRE0)) );				// Wait until data buffer is empty
	UDR0 = data;									// Transmit the `data` local variable through the serial port
}

unsigned char usart_rx( void ) {					// Use the serial buffer to read data that has come from the PC
	while ( !(UCSR0A & (1<<RXC0)) );				// Wait until data buffer is empty
	return UDR0;									// Get and return the received data
}													//

// used for printf
void usart_printchar(char c, FILE *stream) {		// Function to use in redirecting printf to send characters out via USART
	if (c == '\n') usart_printchar('\r', stream);	// Change if we have a NEW LINE, make sure to perform a CR first
	while ( !( UCSR0A & (1<<UDRE0)) );				// Wait for UDRE0 to be be 1, signifying that UDR0 is empty
	UDR0 = c;										// Send character 'c' to UDR0
}													// 

// used for scanf
char usart_scanchar(FILE *stream) {					// Function to use in redirecting scanf to read characters from USART
	while ( !(UCSR0A & (1<<RXC0)) );				// Wait for RXC0 to be 1, indicating an unread character in UDR0
	return UDR0;									// Return UDR0
}													// 
	
void usart_redir() {								// Here we redirect the std functions to their equivalent in AVR
	stdin  = &usart_input;							// Redirect stdin to usart_input
	stdout = &usart_output;							// Redirect stdout to usart_output
}

void usart_isr() {									// ISR function to be called whenever a character is received
													// 
	char c;											// Define a character to be used locally
	c = usart_rx();									// Set this character to whatever is in the UDR0
	rxbuf[iend] = c;								// Place this character in the buffer at position iend
													// 
	if (c == END_OF_MSG_CHAR) {						// If c is the END_OF_MSG_CHR 
		rxbuf[iend] = 0;							// Replace END_OF_MSG_CHR with '0'
		rx_done_callback(&rxbuf);					// Call user-defined callback function
		iend = 0;									// Reset buffer pointer
	}												// 
	else if (c == RESET_MSG_CHAR) {					// If c is the RESET_OF_MSG_CHR 
		rxbuf[iend] = 0;							// Replace the last character in the message with '0'
		iend = 0;									// Reset buffer
//		printf("RESETTING BUFFER\n");				// Reply with confirmation of reset
//		printf("|\n");								// Reply with the reset character
	}												// 
	else {											// Otherwise
		iend++;										// Increment the pointer in the buffer
		if (iend >= BUFLEN)							// If we have gone past the maximum for the buffer
			rx_error_callback(&rxbuf);				// Call the error callback function
	}												// 
}													// 


///////////////////////////////////////////////////////////////////////////////////////////////////////
