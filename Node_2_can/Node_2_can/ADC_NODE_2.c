#ifndef F_CPU
#define  F_CPU 16000000UL
#endif

#include <avr/io.h>
#include "ADC_NODE_2.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///////////// initialize ADC in free running mode					//////
////////////clock divided by 128,non-interrupt mode					//////
///////////channel 0 is selected as default but can be changed later /////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void adc_init(void)
{
	DDRF &=~(1<<PF0);												//edited for IR!!!!
	ADMUX |=(1<<REFS0);												//internal AVCC as reference, channel 0 is selected as default
	ADCSRA|=(1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);		//enable internal ADC, pre-scalar for clock is 128!!!
	
}

int adc_read(void)
{
	ADCSRA|=(1<<ADSC);				//start conversion
	while(!(ADCSRA & (ADIF)))		//wait for interrupt to occur i.e. conversion complete	
	 ;
	 ADCSRA &=~(1<<ADIF);			//clear flag
	 return ADC;
	
}