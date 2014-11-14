/*****************************************************************
 * pin_layout.h
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef PIN_LAYOUT_H_
#define PIN_LAYOUT_H_
#include<avr/io.h>
#ifdef _LCM_12864
// lcm12864 pin layout
// output pins
#define RS_PORT PORTC
#define RS_BIT 0
#define RW_PORT PORTC
#define RW_BIT 1
#define E_PORT PORTC
#define E_BIT 2

#define lcddata PORTA
// input pins
#define busy_PORT PORTA
#define busy_BIT 7

// oled pin layout
#define OLED_DATA_PORT 	PORTB
#define OLED_DATA_DDR 	DDRB

#define OLED_WR_PORT	PORTA
#define OLED_WR_DDR		DDRA
#define OLED_WR_BIT		7

#define OLED_CS_PORT	PORTA
#define OLED_CS_DDR		DDRA
#define OLED_CS_BIT		6

#define OLED_DC_PORT	PORTA
#define OLED_DC_DDR		DDRA
#define OLED_DC_BIT		5
#endif

#endif /* PIN_LAYOUT_H_ */
