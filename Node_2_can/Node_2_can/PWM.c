#ifndef F_CPU 
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "PWM.h"

		//MACROS for PWM
		
#define DUTY OCR2B
#define FREQUENCY OCR2A


	/************************************************************************/
	/* PWM FUCNTIONS explanations;
		
		configuration function;
		
		Phase and frequency correct non-inverted PWM, Pre-scalar=1024, Frequency=50 Hz, TOP Value =156 (OCR2A)
		
		PWM Fucntion
		
		check for max and min and set duty cycle
		
			                                                                 */
	/************************************************************************/
	
	
void pwm_config()
{
	DDRH |= (1<<PH6);												//MAKE OCR2B as output
	TCCR2A|= (1<<COM2B1) | (1<<WGM20) ;								//non-inverted PWM, Phase correct mode
	TCCR2B |= (1<< CS22) | (1<< CS21) | (1<< CS20) | (1<< WGM22);	//pre scalar is 1024, TOP is defined by OCR2A	
	FREQUENCY = 156;													//50,08Hz frequency value
}

void pwm_funct(uint8_t duty)
{

if(duty> PWM_MAX_DUTY || duty<PWM_MIN_DUTY)
	{
	DUTY=PWM_MIN_DUTY;
	}	
	else
	DUTY=duty;
}