/*****************************************************************
 * USART.C
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/
#include <stdint.h>
#include <avr/io.h>
#include "usart.h"


/********************************************************************************
                                usart Related
********************************************************************************/
void usart_init( uint16_t ubrr) {
	// Set baud rate
	UBRR1H = (uint8_t)(ubrr>>8);
	UBRR1L = (uint8_t)ubrr;
	// Enable receiver and transmitter
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1) | (1<<RXCIE1);			//RX enable, TX enable in Interrupt mode!!!
	// Set frame format: 8data, 1stop bit	
	UCSR1C |= (1<<URSEL1)|(3<<UCSZ10);						
}

void usart_putchar(char data) {
	// Wait for empty transmit buffer
	while ( !(UCSR1A & (_BV(UDRE1))) );
	// Start transmission
	UDR1 = data;
}

char usart_getchar(void) {
	// Wait for incomming data
	while ( !(UCSR1A & (_BV(RXC1))) );
	// Return the data
	return UDR1;
}

void usart_pstr(char *s) {
    // loop through entire string
	while (*s) {
        usart_putchar(*s);
        s++;
    }
}

int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}




