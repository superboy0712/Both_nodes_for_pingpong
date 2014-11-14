#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "ADC.h"
#include "mem_map.h"

//#define ADC_ADDR_START 0x1400				//OFFSET address for ADC
#define X_CH 6
#define Y_CH 4

static uint8_t x_mean=0,y_mean=0;

void adc_init(void)
{
	MCUCR |= (1<<SRE);		//Enable XMEM interface
	SFIOR |= (1<<XMM2);		//Mask out higher 4 bits of address
	x_mean=adc_calibrate(X_CH);
	_delay_us(50);
	y_mean=adc_calibrate(Y_CH);
	_delay_us(50);
}

void adc_write(int channel)
{
	volatile uint8_t *p_write= (uint8_t *) (ADC_ADDR_START+channel);
	*p_write=channel;		//select Channel of ADC
}


uint8_t adc_read(int channel)
{
	uint8_t data=0;
	int i=0;
	
	volatile uint8_t *p_write= (uint8_t *) (ADC_ADDR_START+channel);
	volatile uint8_t *p_read= (uint8_t*) (ADC_ADDR_START);
	*p_write=channel;		//select Channel of ADC
	_delay_us(500);
	
	data=*p_read;		//read data from ADC
	i=data;
	_delay_us(500);
	data=*p_read;		//read data from ADC
	i+=data;
	_delay_us(500);
	data=*p_read;		//read data from ADC
	i+=data;
	_delay_us(500);

	data=(uint8_t) (i/3);		//Average 3 samples
	return data;
	
}

uint8_t adc_calibrate(int channel)
{
	uint8_t data=0;
	uint16_t i=0;
	
	
	volatile uint8_t *p_read= (uint8_t *) (ADC_ADDR_START);
	
	
	data=*p_read;		//read data from ADC
	int j;
	for(j = 0; j< 10; j++){
		data=*p_read;
		i+=data;
		_delay_ms(100);	
	}
	data=(uint8_t) (i/10);			//Average 10 samples
	return data;
	
}

void position(signed int *x_val,signed int *y_val,uint8_t x_mean,uint8_t y_mean)
{
	*x_val=(signed int) ((*x_val-x_mean)*50/128);
	*y_val=(signed int) ((*y_val-y_mean)*100/128);
}

//struct adc_pos {int16_t x; int16_t y;};
	
struct adc_pos adc_get_position(void){
	struct adc_pos pos;


	pos.x = adc_read(X_CH);
	pos.y = adc_read(Y_CH);//-y_mean;
	//
	return pos;
}