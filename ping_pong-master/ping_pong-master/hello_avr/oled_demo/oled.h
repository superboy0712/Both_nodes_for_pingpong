/*****************************************************************
 * oled.h
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef OLED_H_
#define OLED_H_
#include <inttypes.h>
#include <stdio.h>
#include <avr/pgmspace.h>
extern void oled_init(void);
extern void oled_home(void);
extern void oled_goto_xy(uint8_t x,uint8_t y);
extern void oled_goto_nextln(void);
extern void oled_clear(void);
extern void oled_putchar( const char c);
extern void oled_putchar_inverse(char c);
//
extern void oled_draw_dot_buffer( uint8_t col, uint8_t row);
extern void oled_putchar_buffer(char c);
extern void oled_putstr_buffer(const char * str);
extern void oled_clear_buffer(void);
extern void oled_goto_xy_buffer(uint8_t x,uint8_t y);
extern void oled_buffer_update(void);
extern void oled_putstr( const char * str);
extern void oled_putstr_inverse( const char * str);
extern void oled_putstr_P( const char * str);
extern void oled_putstr_P_inverse( const char * str);
extern void oled_wr_d(uint8_t data);
extern void oled_wr_cmd(uint8_t cmd);
extern void oled_set_normal(void);
extern void oled_set_inverse(void);// whole
extern int oled_putchar_printf(char var, FILE *stream);
//
extern void oled_draw_circle(uint8_t x0, uint8_t y0, uint8_t r);
extern void oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
#endif /* OLED_H_ */
