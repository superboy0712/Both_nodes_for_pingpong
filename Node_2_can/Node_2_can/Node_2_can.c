/*
 * iA_lab_5.c
 *
 * Created: 30.09.2014 11:15:26
 *  Author: waseemh
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "CAN.h"
#include "PWM.h"
#include "ADC_NODE_2.h"
//#include "TWI_NODE_2.h"
#include "atmega_twi_driver.h"
#include "ENCODER_NODE_2.h"
#include <stdlib.h>


		//GENERAL PURPOSE MACRO (IR and other)

#define IR_TH 40

#define DAC_ADDR 0x50		//in binary it is 01010000
#define DAC_CMD 0x00		//PD and RST are zero, Output is at DAC channel 0
#define DAC_RESET 0x10		//Reset command!!!! 0b00010000

#define SLAVE_ADDRESS 0b0101000
#define MOTOR_ENABLE			(PORTF |=(1<<PF5))
#define MOTOR_DISABLE			(PORTF &=~(1<<PF5))
#define DIR_LEFT			(PORTF |=(1<<PF6))
#define DIR_RIGHT			(PORTF &=~(1<<PF6))




			//PI CONTROLLER MACROS *0.01 and 0.08*
/*
#define Kp	0.008
#define Ki	0.005
#define Kd  0.05
*/
#define CONTROL_STALL 150


		//MACROS FOR CAN CONTROLLER MODES

#define NORMAL 0x00
#define LOOP_BACK 0x02


		//FUNCTION DECLARATIONS
void position(char *x_val,char *y_val);
uint8_t adc_filter_read(void);


		//GLOBAL VARIABLES (for interrupts and ISRS etc)
		
int rx_flag=0;
float Kp = 0.008;
float Ki =	0.005;
float Kd =  0.05;



int main(void)
{
int i;	
	//INTERRUPT CONFIGURATION (INT2)
	
	
	sei();						//Enable Global Interrupts
	EIMSK |= (1<<INT2);			//Enable INT_2
	EICRA  |= (1<<ISC21);		//Falling Edge
	
	
	//DATA VARIABLES
	
	char rx_data[8];				//CAN RX data
	char tx_data[8];				//CAN TX data
	uint8_t rx_id[2], rx_length;	//CAN control field
	uint8_t servo_pwm_value=0;		//SERVO and PWM variables
	uint8_t score=0;				//IR and Score variables
	char enc_upper,enc_lower;				//variables for encoder
	
	
	
	//PI controller part
	uint8_t sp_var_1=0;
	int16_t set_point = 0;
	int16_t error_p=0,error_int=0,error_d=0,error_current=0,error_last=0;
	float control_output=0;
	
	//Encoder Variables
	int16_t encoder_value=0;			//Variable for Mathematical calculations

	int16_t encoder_current=0,encoder_start=0,encoder_end=0,encoder_range=0;		//Variables for reading encoder;
	
	
	
	
	
	//PWM, Internal ADC, TWI, SPI and CAN initialization functions

	pwm_config();

	adc_init();

	spi_config();
	
	can_reset();
		
	
	//Test
	//unsigned char Data[16];
	//unsigned char receivedData [16];
	//! Initialize the driver
	atmel_led_drvr_init();
	
	
		//CAN configuration function (normal mode)
			
	can_config(NORMAL);		//configure in loop back mode

	static uint8_t score_count = 0;
	
	/*
			//DAC RESET PORTION
	
	TWI_start();		//send servo value on I2C interface
	//_delay_ms(5);
	TWI_write((uint8_t)DAC_ADDR);		//send address of slave!!!
	_delay_us(10);
	TWI_write((uint8_t)DAC_RESET);
	_delay_us(10);
	
	TWI_stop();

	
	*/
	
	
			//Ports for encoder
			
	DDRF|=(1<<PF2) |(1<<PF3) |(1<<PF4) |(1<<PF5) |(1<<PF6);		//output port bits for encoder values
	
	DDRH |= (1<<PH1);											//MAKE PH 1 OUTPUT FOR TOGGLE AND TIMING

	DDRK=0x00;		//input port
	
	
	//unsigned int data = 0;
	uint8_t data_to_send = 0;
	

	
	
	
	MOTOR_ENABLE;
	DIR_LEFT;
	atmel_led_drvr_writeregister(SLAVE_ADDRESS, DAC_CMD, 120);
	_delay_ms(2000);
	MOTOR_DISABLE;
	PORTH &=~(1<<PH1);
	
	for(i=0;i<50;i++)
	{
		encoder_init();
	}
	
//	can_transmit(0,0x05,8,tx_data);
		
	
	
	MOTOR_ENABLE;
	DIR_RIGHT;
	atmel_led_drvr_writeregister(SLAVE_ADDRESS, DAC_CMD, 120);
	_delay_ms(2000);
	MOTOR_DISABLE;
	
	
	
	for(i=0;i<10;i++)
	{
		encoder_read(&enc_upper,&enc_lower);
		encoder_end+=(enc_upper*0x100)+enc_lower;
	}
	
	encoder_end=encoder_end/10;					//average 10 values!!!

	encoder_range=(encoder_end-encoder_start);
	

    while(1)
    {	
		
		
		PORTH |= (1<<(PH1));
		/************************************************************************/
		/* This commented part is before editions!!! 
		                                                                     */
		/************************************************************************/
		/*
		encoder_read(&enc_upper,&enc_lower);
		
		encoder_value=(enc_upper<<8)+enc_lower;
		
		encoder_value=encoder_value/4;
		*/
		
		
		encoder_read(&enc_upper,&enc_lower);
		
		encoder_current=((enc_upper*0x100)+enc_lower);
		
		
		encoder_value=(encoder_current);		//convert to percentage 0 to 100 is range of encoder value
		
		
		
		uint8_t val = adc_filter_read();
		
		if(val < IR_TH && val > 12)
		{
			score_count ++;
			//_delay_ms(2);
			
		}else score_count = 0;
		
		/*if(score_count > 50){
			is_score = 1;
		}else is_score = 0;
		*/
		if(score_count>20){
			score_count = 0;
			score++;
		}
		if(rx_flag)
		{
			can_rx_if_clear();
			rx_flag=0;
			can_receive(rx_id,&rx_length,rx_data);
			can_rx_if_clear();
			/************************************************************************/
			/*                    SERVO POSITION CONTROL EQUATION 
					
					100 is added at node 2, so subtract here!!!
					2 is offset (physical observation) so subtracted 
						  			 							                    */
			/************************************************************************/
			
			int16_t pos_x = (uint8_t)rx_data[2] - 117;
			if (pos_x>125)
			{
				pos_x = 125;
			}
			if (pos_x<-125)
			{
				pos_x = -125;
			}
			if( -3 < pos_x && pos_x < 3 )
				pos_x = 0;
			servo_pwm_value = (PWM_MIN_DUTY + PWM_MIN_DUTY)/2 - ((PWM_MAX_DUTY - PWM_MIN_DUTY)*(pos_x/256.0)) + 5;
		
			pwm_funct(servo_pwm_value);
			/************************************************************************/
			/*  dynamic tuning pid                                                  */
			/************************************************************************/
			Kp = rx_data[4]/1000.0;
			Ki = rx_data[5]/1000.0;
			Kd = rx_data[6]/100.0;
			/************************************************************************/
			
			tx_data[0] =servo_pwm_value;					//invert pattern, always remains constant
			tx_data[1] =score;							//SCORE (changed from DIODE VALUE val)
			tx_data[2] =rx_data[2];						//X 
			//tx_data[3] =rx_data[3];						//Y
			//tx_data[3] = data_to_send;
			//tx_data[4] =servo_pwm_value;				//servo value
 			tx_data[5]=enc_upper;						//encoder upper byte
			tx_data[6]=enc_lower;						//encoder lower byte
			tx_data[3]=set_point/0x100;
			tx_data[4]=set_point%0x100;
			
			
			/************************************************************************/
			/*				TWI communication and Motor control part  
					Can be included outside flag routine!!!
					
					Controller Description
					Mode: PI
					Maximum value: 255
					Direction Logic:
					
					Extreme Left=>  encoder=0
					Extreme Right=> encoder= 100 (can be converted from 255 or other value)
					
					If SET_POINT-ERROR > 0 => DIR= LEFT
					IF SET_POINT-ERROR < 0 => DIR= RIGHT 
					 
																					*/
			/************************************************************************/
			
					//Before compiling, declare all variable of this section and make function of encoder!!!
					
					

			/*// motor part
			data = (uint8_t)rx_data[3];
			if (data >123 && data <129){
				MOTOR_DISABLE;
			}else if(data > 129)
			{
				MOTOR_ENABLE;
				DIR_LEFT;
				data_to_send = data - 126;
				}else if(data <123){
				MOTOR_ENABLE;
				DIR_RIGHT;
				data_to_send = 126 - data;
			}*/
			sp_var_1=(uint8_t)rx_data[3];
			
			//sp_var_2=(60*sp_var_1);				//convert in percentage (0 to 100 is value)!!!
			
			set_point=60*sp_var_1;
			
			//set_point=(uint8_t)rx_data[3];				//SETPOINT is 4th byte of CAN frame (received), slider value!!!
		/*
			if (set_point >123 && set_point <129){
				MOTOR_DISABLE;
			}else if(set_point > 129)
			{
				MOTOR_ENABLE;
				DIR_LEFT;
			
			}else if(set_point <123){
				MOTOR_ENABLE;
				DIR_RIGHT;
	
			}
			error_p=set_point-encoder_value;
			
			error_int+=error_p;					//Integral effect
			
			
			
			control_output= (Kp*error_p) + (Ki*error_int);
			
			//data_to_send = (uint8_t)(96*(data_to_send/128.0));
			if(control_output > 200)
			control_output = 200;
			
			if(control_output < -200)
			control_output = -200;
			
			//
			*/
		
		error_p=set_point-encoder_value;
		
		error_current=error_p;
		
		error_int+=error_p;					//Integral effect
		
		error_d=error_current-error_last;
		
		error_last=error_current;
		#define RANGE_A 0.168
		#define RANGE_B 0.6
		//control_output= (Kp*error_p) + (Ki*error_int) + (Kd*error_d);
		if(abs(error_p) < 0.1*abs(encoder_range)){
			control_output= (Kp*error_p) + (Kd*error_d);
		}else if((abs(error_p) >= 0.1*abs(encoder_range))&&(abs(error_p) < 0.6*abs(encoder_range))){
			control_output= (Kp*error_p) ;
		}else{
			control_output= (Kp*error_p) + (Ki*error_int);
		}
		
		//control_output= (Kp*error_p) + (Ki*error_int)+(Kd*error_d);
		//control_output=set_point-120;				//at center, it should be zero and at extreme (left or right but when ADC =0 ) its should be maximum/high enough (120)
		if(control_output>0)
		{
			MOTOR_ENABLE;
			DIR_RIGHT;
			
		}
		else if (control_output<0)
		{
		MOTOR_ENABLE;
		DIR_LEFT;
		}
		else 
		{
		MOTOR_DISABLE;	
		}
		
		if(control_output >150 || control_output <-150)
		{
		control_output=150;	
		}
		
			data_to_send = (uint8_t)abs(control_output);
			
			atmel_led_drvr_writeregister(SLAVE_ADDRESS, DAC_CMD, data_to_send);
			 //
			 can_transmit(0,0x05,8,tx_data);			//BUFFER 0, ID 0x05, 8 data bytes and data=stored string
			can_rx_if_clear();
		}
		
		can_rx_if_clear();	
		
		
		
	PORTH &= ~(1<<(PH1));
	}
	
}


/************************************************************************/
/*		CAN RX FLAG INTERRUPT ROUTINE (ISR)                             
		
		EXTERNAL INTERRUPT 2 IS USED
																		*/
/************************************************************************/


ISR(INT2_vect)
{
	rx_flag=1;					//set receive flag for main function, flag will be cleared in main.
	EIFR &= ~(1<<INTF2);		//clear interrupt flag
}


/************************************************************************/
/*				ADC (NODE 1) POSITON CALCULATION FUNCTION              */
/************************************************************************/

void position(char *x_val,char *y_val)
{
	*x_val=(signed char) ((*x_val)*100/128);
	*y_val=(signed char) ((*y_val)*100/128);
}



/************************************************************************/
/*			NODE 2 ADC (IR) FILTER FUNCTION							    */
/************************************************************************/

uint8_t adc_filter_read(void)
{
	int i,mean = 0;
	
	for(i = 0; i < 10; i++)
	
	{
		mean += adc_read();
		//_delay_ms(5);
	}
	
	return (uint8_t)(mean/10);
}