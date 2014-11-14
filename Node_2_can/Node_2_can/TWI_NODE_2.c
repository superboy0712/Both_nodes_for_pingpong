#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include "TWI_NODE_2.h"

#define ADDR 0x50				//in binary 01010000 

/************************************************************************/
/*		TWI Initialization function
     
	 Clock rate to be decided, pre-scalar is 16. use minimum value for TWBR
	 Formula is;
	 CPU Clock frequency/(16 + 2(TWBR).4.TWPS)                          */
/************************************************************************/


void TWI_init(void)
{
	
	TWSR |= (1<<TWPS1);									//divide by 4 (changed ffrom 16 i.i. PS1)
	TWBR = 4;											//to be decided!!!!
	//enable TWI
	TWCR = (1<<TWEN);
}

/************************************************************************/
/*	start condition function
	flag check can be omitted
	Remember; flag is cleraed by writing 1 in register!!!! 
	(As per data-sheet, page 262)
                                                                     */
/************************************************************************/


void TWI_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);			//enable TW interface and send start bit
	while ((TWCR & (1<<TWINT)) == 0);				//wait for flag to set, indicating start bit has transmitted
	TWCR |= (1<<TWINT);								//clear flag writing 1!!!
}

/************************************************************************/
/*	TWI stop condition function, only write one bit to TWCR register    */
/************************************************************************/

void TWI_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

/************************************************************************/
/*	TWI data transmission function;

	Load data in TWDR, enable TWI using TWCR register
	wait for interrupt to occur. Finally clear interrupt flag           */
/************************************************************************/

void TWI_write(uint8_t data)
{
	TWDR = data;							//write data into data register
	TWCR = (1<<TWINT)|(1<<TWEN);			//enable TW interface
	while ((TWCR & (1<<TWINT)) == 0);		//wait for interrupt to set
	TWCR |= (1<<TWINT);						//clear flag writing 1!!!
}

/************************************************************************/
/*	High level function to implement TWI
	Use this function directly by providing address of slave and data
	Suitable for one byte of data transfer for a specified slave        */
/************************************************************************/

void TWI_funct(uint8_t addr, uint8_t data)
{	
	addr=(uint8_t)ADDR;
	
		//Send start condition 
	TWI_start();
		
		//send address of device
	TWI_write(addr);
		
		//send desired data
	TWI_write(data);
	
		//send stop condition
	TWI_stop();
	
}
