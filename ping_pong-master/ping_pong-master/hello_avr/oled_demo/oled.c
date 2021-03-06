/*****************************************************************
 * oled.c
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/
#include "pin_layout.h"
#include "mem_map.h"
#include "helpFile.h"
#include "font_5x7.h"
#include "util/delay.h"
#include <stdio.h>
#include "oled.h"
/*					RD	WR	CS	DC
 * 	Write command 	H 	↑ 	L 	L
*	Write data 		H 	↑ 	L	H
*/
#ifdef _LCM_12864
#define WR_HIGH set_bit(OLED_WR_PORT, OLED_WR_BIT)
#define WR_LOW clr_bit(OLED_WR_PORT, OLED_WR_BIT)
#define CS_HIGH set_bit(OLED_CS_PORT, OLED_CS_BIT)
#define CS_LOW clr_bit(OLED_CS_PORT, OLED_CS_BIT)
#define DC_HIGH set_bit(OLED_DC_PORT, OLED_DC_BIT)
#define DC_LOW clr_bit(OLED_DC_PORT, OLED_DC_BIT)
#endif
// some Marcos
#define START_COL 0
#define END_COL 127
#define START_PAGE 0
#define END_PAGE 6
#define LINE_LENGTH (END_COL - START_COL + 1)
#define CHA_WIDTH 5
#define MAX_CHARS_A_LINE (LINE_LENGTH/CHA_WIDTH)
// global var, representing the position.
volatile static uint8_t current_col_address = 0;// from from 0 to END_COL - START_COL
volatile static uint8_t current_pag_address = 0;// from from 0 to END_PAG - START_PAGE
// buffer ram, possibly locate in ext sram
//uint8_t oled_disp_buffer[128];

//

void oled_wr_cmd(uint8_t cmd){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_CMD_START;
	*addr = cmd;*/
	volatile uint8_t *  addr = (uint8_t *)OLED_ADDR_CMD_START;
	*addr = cmd;
}



void oled_wr_d(uint8_t data){
	// write cmd to oled!! very important to add volatile
	/*volatile char * const addr = OLED_ADDR_DATA_START;
	*addr = data;*/
	volatile uint8_t * const addr = (uint8_t *)OLED_ADDR_DATA_START;
		*addr = data;
}

 
 void oled_init(void)
 {
 	MCUCR |= (1<<SRE);        //Enable External Memory interface
 	SFIOR |= (1<<XMM2);       //Mask out higher 4 bits of address
 	volatile uint8_t *addr= (uint8_t *) OLED_ADDR_CMD_START;		//selcet command mode
 	
 	//code copied from datasheet
 	*addr=0xae;				//turn display off
 	*addr=0xa1;			//column addr 127 is mapped to seg0
 	*addr=0xda;				//com pin configuration
 	*addr=0x12;				//com pin config=alternative
 	*addr=0xc8;				//o/p scan direction com63 to com0
 	*addr=0xa8;				//multiplex ratio config
 	*addr=0x3f;				//ratio=63
 	*addr=0xd5;				//dclck ratio
 	*addr=0x80;				//divide ratio and oscl. frequency
 	*addr=0x81;				//contrast control
 	*addr=0x50;				//value of contrast is 50h
 	*addr=0xd9;				//set pre-charge period
 	*addr=0x21;				//value =21
 	*addr=0x20;				//set memory addressing mode
 	*addr=0x00;				//horz addressing mode
 	*addr=0xdb;				//vcom de-select level mode
 	*addr=0x30;				//value=0.83vcc
 	*addr=0xad;				//master configuration
 	*addr=0x00;				//select extrenal iref
 	*addr=0xa4;				//out follows ram content
 	*addr=0xa6;				//set normal display
 	*addr=0xaf;				//display on
 }



void oled_goto_xy(uint8_t col,uint8_t row){
	// input: 0 to max - min.
	current_col_address = col;
	current_pag_address = row;
//	oled_wr_cmd(0x40);
	oled_wr_cmd(0x21);// set col address
	oled_wr_cmd(col + START_COL);
	oled_wr_cmd(END_COL);
	oled_wr_cmd(0x22);// set pag address
	oled_wr_cmd(row + START_PAGE);
	oled_wr_cmd(END_PAGE);
}

void oled_goto_nextln(void){
	// input: 0 to max-min
	current_pag_address++;
	if(current_pag_address > END_PAGE - START_PAGE)
		current_pag_address = 0;
	oled_goto_xy(0, current_pag_address);
}
void oled_putchar( const char c){
	if(c == '\n'){
		oled_goto_nextln();
		return;
	}
	int i;
	const char j = (c-' ');
	current_col_address+=CHA_WIDTH;

	if(current_col_address/CHA_WIDTH >= MAX_CHARS_A_LINE){
		//current_col_address = 0;
		oled_goto_nextln();
	}


	for(i = 0; i < 5; i++){
		// pay attention, progmem read
		 oled_wr_d(pgm_read_byte(&font[(int)j][i]));
	}
}

void oled_set_inverse(void){
	oled_wr_cmd(0xa7);
}

void oled_set_normal(void){
	oled_wr_cmd(0xa6);
}

void oled_putstr( const char * str){
	while(*str)
		oled_putchar(*str++);
}
void oled_putstr_inverse( const char * str){
	while(*str)
		oled_putchar_inverse(*str++);
}
void oled_putstr_P( const char * str){
		char read = pgm_read_byte(str);
		// Display buffer on LCD.
		while(read){
			oled_putchar(read);
			str++;
			read = pgm_read_byte(str);
		}
		
}
void oled_putstr_P_inverse( const char * str){
	char read = pgm_read_byte(str);
	// Display buffer on LCD.
	while(read){
		oled_putchar_inverse(read);
		str++;
		read = pgm_read_byte(str);
	}
	
}
void oled_clear(void){
	current_col_address = 0;
	current_pag_address = 0;
	oled_wr_cmd(0xae); // off
	//
	oled_wr_cmd(0x20);
	oled_wr_cmd(0);
	oled_wr_cmd(0x21);
	oled_wr_cmd(0);
	oled_wr_cmd(127);
	oled_wr_cmd(0x22);
	oled_wr_cmd(0);
	oled_wr_cmd(7);
	oled_wr_cmd(0x40);
	int temp;
	for(temp = 0; temp < 8*128; temp++){
		oled_wr_d(0x00);
	}
	oled_wr_cmd(0x21);
	oled_wr_cmd(START_COL);
	oled_wr_cmd(END_COL);
	oled_wr_cmd(0x22);
	oled_wr_cmd(START_PAGE);
	oled_wr_cmd(END_PAGE);
	oled_wr_cmd(0x7c);
	oled_wr_cmd(0xaf); // on
	oled_goto_xy(0,0);
}
void oled_clear_line(uint8_t ln){
	// ln: 1 - 7
	int i;
	oled_goto_xy(0,ln);
	for ( i = 0; i< 128; i++)
	{
		oled_wr_d(0);
	}
}

int oled_putchar_printf(char var, FILE *stream){
	if(var == '\r'||var =='\n'){
		oled_goto_nextln();
		return 0;
	}

	oled_putchar(var);
	return 0;
}

void oled_putchar_inverse(char c){
	if(c == '\n'){
		oled_goto_nextln();
		return;
	}
	int i;
	const char j = (c-' ');
	current_col_address+=CHA_WIDTH;

	if(current_col_address/CHA_WIDTH >= MAX_CHARS_A_LINE){
		//current_col_address = 0;
		oled_goto_nextln();
	}


	for(i = 0; i < 5; i++){
		// pay attention, progmem read
		oled_wr_d(~(pgm_read_byte(&font[(int)j][i])));
	}
}
static volatile uint8_t * const buffer_start = (uint8_t *)DISP_BUFFER_START;
static volatile uint16_t current_buffer_file_pointer = 0; 
void oled_draw_dot_buffer(uint8_t x, uint8_t y)
{
	// LOCATE THE BYTE
	uint16_t index_offset = x + (y/8)*128;
	buffer_start[index_offset] |= (1 << y % 8); 
}

void oled_buffer_update(void)
{
	// copy the whole buffer to the oled
	int i;
	oled_goto_xy(0,0);
	for (i = 0; i< 896; i++)
	{
		oled_wr_d(buffer_start[i]);
	}
	
}

void oled_putchar_buffer(char c)
{
	if(c == '\n'){
		current_buffer_file_pointer += 128;
		return;
	}
	int i;
	const char j = (c-' ');
	

	if(((current_buffer_file_pointer)%128)/CHA_WIDTH >= MAX_CHARS_A_LINE){
		//current_col_address = 0;
		current_buffer_file_pointer += 128;
		if (current_buffer_file_pointer >= 896)
		{
			current_buffer_file_pointer = 0;
		}
		
	}


	for(i = 0; i < 5; i++){
		// pay attention, progmem read
		buffer_start[current_buffer_file_pointer] = pgm_read_byte(&font[(int)j][i]);
		current_buffer_file_pointer++;
	}
}

void oled_putstr_buffer(const char *str)
{
	while(*str)
		oled_putchar_buffer(*str++);
		
	oled_buffer_update();	
}

void oled_goto_xy_buffer(uint8_t x,uint8_t y)
{
	uint16_t index = x + y*128;
	current_buffer_file_pointer = index;
}

extern void oled_clear_buffer(void)
{
	uint16_t i;
	for (i = 0; i< 1024; i++)
	{
		buffer_start[i] = 0x00;
	}
}

void oled_draw_circle(uint8_t x0, uint8_t y0, uint8_t r)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;
	oled_draw_dot_buffer(x0, y0+r);
	oled_draw_dot_buffer(x0, y0-r);
	oled_draw_dot_buffer(x0+r, y0);
	oled_draw_dot_buffer(x0-r, y0);
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		oled_draw_dot_buffer(x0 + x, y0 + y);
		oled_draw_dot_buffer(x0 - x, y0 + y);
		oled_draw_dot_buffer(x0 + x, y0 - y);
		oled_draw_dot_buffer(x0 - x, y0 - y);
		oled_draw_dot_buffer(x0 + y, y0 + x);
		oled_draw_dot_buffer(x0 - y, y0 + x);
		oled_draw_dot_buffer(x0 + y, y0 - x);
		oled_draw_dot_buffer(x0 - y, y0 - x);
	}
	oled_buffer_update();
}

void oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	int16_t i;
	for (i=x; i<x+w; i++) {
		oled_draw_dot_buffer(i, y);
		oled_draw_dot_buffer(i, y+h-1);
	}
	for (i=y; i<y+h; i++) {
		oled_draw_dot_buffer(x, i);
		oled_draw_dot_buffer(x+w-1, i);
	}
	oled_buffer_update();
}



/*
void oled_buffer_wr(
		uint8_t col,
		uint8_t row,
		uint8_t *data,
		uint8_t length){
	if(length<0 || length > (128 - row*16- col + 1))
		return;
	while(length--){
		oled_disp_buffer[row*16+col] = *data;
		data++;
	}
}*/
/*
void oled_buffer_update(void){
	uint8_t i;
	// maybe need some aligning modifications
	for (i = 0; i<128; i++){
		oled_wr_d(oled_disp_buffer[i]);
	}
}*/
