#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"

void uart_config(void)
{
	UBRR0H = 00;									//BAUD RATE is 9600
	UBRR0L = 31;									//BAUD RATE is 9600
	UCSR0B = (1<<TXEN0);							//only TX mode enabled
	UCSR0C =(1<<0)|(1<<UCSZ00) | (1<<UCSZ01);	//8 Data bits, 1 Stop bit, no Parity
}


void uart_transmit(char data)
{
	while( !( UCSR0A & (1<<UDRE0)) )
	;
	UDR0 = data;//_dummy;								//DATA is sent in character format!!!!
}

