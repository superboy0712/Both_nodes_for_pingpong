/*****************************************************************
 * helpFile.h
 * hello_avr
 *
 *  Created on		: Sep 9, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef HELPFILE_H_
#define HELPFILE_H_

#define set_bit(reg,bit) (reg |=(1<<bit))
#define clr_bit(reg,bit) (reg &=~(1<<bit))
#define test_bit(reg, bit) (reg & (1<<bit))


#endif /* HELPFILE_H_ */
