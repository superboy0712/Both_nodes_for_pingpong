/*
 * iA_lab_5.c
 *
 * Created: 30.09.2014 11:15:26
 *  Author: waseemh
 */ 

#define F_CPU 4195200UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"
#include "UART.h"

		//MACROS FOR CAN CONTROLLER MODES

#define NORMAL 0x00
#define LOOP_BACK 0x02



int rx_flag=0;

int main(void)
{	
	//INTERRUPT CONFIGURATION (INT2)
	
	
	sei();						//Enable Global Interrupts
	GICR |= (1<<INT2);			//Enable INT_2, falling edge (ISC2 is zero)
	
	//DATA VARIABLES
	
	char rx_data[8];
	char tx_data[8]="LAB_CAN";
	uint8_t rx_id[2], *rx_length=0;
	int loop_var=0;
	
	
	//UART functions
	
	uart_config();
	fdevopen(&uart_transmit, (void *) 0);		//UART write function
	
	//SPI and CAN functions
	
	spi_config();
	
	can_reset();
	
	can_config(LOOP_BACK);		//configure in loop back mode
	
    while(1)
    {
		can_transmit(0,0x05,8,tx_data);			//BUFFER 0, ID 0x05, 8 data bytes and data=stored string
		
		_delay_ms(500);
		
		if(rx_flag)
		{
			rx_flag=0;
			printf("\n\rINT\n\r");
			can_receive(rx_id,rx_length,rx_data);
			
		}
		//for(loop_var=0;loop_var<8;loop_var++)
		//{
		printf("\r\nRX_ID LOWER=%d,      RX_ID HIGHER=%d\n\r",rx_id[0],rx_id[1]);
		printf("\n\rRX_DATA LENGTH=%d\n\r",*rx_length>>5);
		spi_chipselect(1);
		spi_tx_rx(0b00000011);
		spi_tx_rx(0x2C);
		loop_var= spi_tx_rx(0);
		spi_chipselect(0);
		printf("\n\rCANITF=%x\n\r",loop_var);
		for(loop_var=0;loop_var<8;loop_var++)
		{
			
		printf("\n\r%c\n\r",rx_data[loop_var]);
		
		}
        //TODO:: Please write your application code 
    _delay_ms(100);
	for (loop_var=0;loop_var<8;loop_var++)
	{
		rx_data[loop_var]=0;
	}

	}

	}


ISR(INT2_vect)
{
	rx_flag=1;					//set receive flag for main function, flag will be cleared in main.
	GIFR &= ~(1<<INTF2);		//clear interrupt flag
}