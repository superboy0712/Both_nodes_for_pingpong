#ifndef PWM_H
#define PWM_H

//MACROS for check

#define PWM_MAX_DUTY 16
#define PWM_MIN_DUTY 8

void pwm_config();				//PWM Configuration function
	
void pwm_funct(uint8_t duty);	//PWM duty cycle changing function

#endif