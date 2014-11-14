#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "ENCODER_NODE_2.h"


		//MACROS FOR ENCODER
#define ENABLE_ENCODER			(PORTF &=~(1<<PF2))		//pin 3 on both connector is PF7, set low to enable
#define DISABLE_ENCODER			(PORTF |=(1<<PF2))
#define RESET_ENCODER			(PORTF &=~(1<<PF3))		//pin 4 on both connector is PF6, TOGGLE to RESET!!!!
#define NORMAL_ENCODER			(PORTF |=(1<<PF3))		//normally RESET should be high
#define HIGH_BYTE				(PORTF &=~(1<<PF4))		//SEL is low to get high byte
#define LOW_BYTE				(PORTF |=(1<<PF4))		//SEL is high to get low byte



void encoder_read(char *upper_byte,char *lower_byte)
{

	
	int temp_1=0,temp_2=0,loop_var=0;
	uint8_t read;
	 
	ENABLE_ENCODER;
	
			//read upper byte and process
			
	HIGH_BYTE;
	
	_delay_us(30);
	
	temp_1=PINK;
	
	for(loop_var=0;loop_var<= 7;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |=  1<< (8-loop_var);
		} else
		{
		temp_2 &= ~((1<<(8-loop_var)));
		}
	
	}
	
	*upper_byte=temp_2;
	
			//read lower byte and process
	
	LOW_BYTE;					
	
	_delay_us(30);
	
	temp_1=PINK;
	
	for(loop_var=0;loop_var<= 7;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |=  1<< (8-loop_var);
		} else
		{
			temp_2 &= ~((1<<(8-loop_var)));
		}
	
	}
	
	*lower_byte=temp_2;
	
	//RESET_ENCODER;
	
	//NORMAL_ENCODER;		//toggle for resetting
	
	NORMAL_ENCODER;		//Ensure reset is high at end of function
	
	DISABLE_ENCODER;	//Disable for further reading......
	
	}
	
	
	
	
	void encoder_init()
	{

				//RESET at extreme end
		ENABLE_ENCODER;
				
		RESET_ENCODER;
		
		NORMAL_ENCODER;		//toggle for resetting
		
		RESET_ENCODER;		
		
		NORMAL_ENCODER;		//Ensure reset is high at end of function
		
		DISABLE_ENCODER;	//Disable for further reading....
	}





/************************************************************************/
/* copy of actual read function                                         */
/************************************************************************/

/*
void encoder_read(char *upper_byte,char *lower_byte)
{

	
	int temp_1=0,temp_2=0,loop_var=0;
	uint8_t read;
	
	ENABLE_ENCODER;
	
	//read upper byte and process
	
	HIGH_BYTE;
	
	_delay_us(30);
	
	temp_1=PINK;
	
	for(loop_var=1;loop_var<= 8;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |= (1<< 8)-loop_var;
		} else
		{
			temp_2 &= ~((1<<8)-loop_var);
		}
		
	}
	
	*upper_byte=temp_2;
	
	//read lower byte and process
	
	LOW_BYTE;
	
	_delay_us(30);
	
	temp_1=PINK;
	
	for(loop_var=1;loop_var<= 8;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |= (1<< 8)-loop_var;
		} else
		{
			temp_2 &= ~((1<<8)-loop_var);
		}
		
	}
	
	*lower_byte=temp_2;
	
	RESET_ENCODER;
	
	NORMAL_ENCODER;		//toggle for resetting
	
	DISABLE_ENCODER;
	
	NORMAL_ENCODER;		//Ensure reset is high at end of function
	
}

*/