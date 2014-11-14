#define F_CPU 4915200UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "oled.h"
#include "MicroMenu.h"
#include "ADC.h"
#include "usart.h"
#include "CAN.h"
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "iA_PWM_NODE_1.h"
#define USART_BAUD 9600
#define MYUBRR F_CPU/16/USART_BAUD-1



enum ButtonValues
{
	BUTTON_NONE,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
};
static enum ButtonValues button_val = BUTTON_NONE;





static void genric_draw_fun(uint8_t x, uint8_t y, const char *str){
	oled_goto_xy(x,y);
	oled_putstr_P(str);
}

void generic_SelectCallback(uint8_t x, uint8_t y, const char* str){
	oled_goto_xy(x,y);
	oled_putstr_P_inverse(str);
}
/************************************************************************/
/* enable_debug_mode_display                                            */
/************************************************************************/
int flag_enable_debug_mode_display = 0;
void enable_debug_mode_display(void){
	oled_goto_xy(6,0);
	puts("Into Debug mode");
	flag_enable_debug_mode_display = 1;
}
/************************************************************************/
/*  enable online tunning                                               */
/************************************************************************/
int flag_enable_online_tuning = 0; 
void enable_online_tuning_display(void){
	oled_goto_xy(6,0);
	puts("Into Online tunning");
	flag_enable_online_tuning = 1;
}
/************************************************************************/
/*  Graphic Demo                                                        */
/************************************************************************/
int flag_enable_graphic_demo = 0;
void Graphic_demo_enable(void){
	flag_enable_graphic_demo = 1;
}
/************************************************************************/
/*  Music Demo                                                          */
/************************************************************************/
int flag_enable_music_demo = 0;
void Music_demo_enable(void){
	flag_enable_music_demo = 1;
}
/************************************************************************/
/*  display flags clr routine                                */
/************************************************************************/
inline static void Menu_Dsp_flag_clr(void){
	flag_enable_debug_mode_display = 0;
	flag_enable_online_tuning = 0;
	flag_enable_graphic_demo = 0;
	flag_enable_music_demo = 0;
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */

MENU_ITEM( Menu_1, (5), (2), Menu_2, NULL_MENU, NULL_MENU, Menu_1_1, generic_SelectCallback, NULL, "Let's Play!");
MENU_ITEM( Menu_2, (10), (3), Menu_3, Menu_1, NULL_MENU, Menu_2_1, generic_SelectCallback, NULL, "Online Tuning!");
MENU_ITEM( Menu_3, (15), (4), Menu_4, Menu_2, NULL_MENU, Menu_3_1, generic_SelectCallback, NULL, "On the Fly Debugging!");
MENU_ITEM( Menu_4, (20), (5), Menu_5, Menu_3, NULL_MENU, Menu_4_1, generic_SelectCallback, NULL, "Dual Buffer Demo!");
MENU_ITEM( Menu_5, (20), (6), NULL_MENU, Menu_4, NULL_MENU, Menu_5_1, generic_SelectCallback, NULL, "Music Demo!");
MENU_ITEM( Menu_1_1, 7, 1, Menu_1_2, NULL_MENU, Menu_1, NULL_MENU, generic_SelectCallback, NULL, "Play with Joystick");
MENU_ITEM( Menu_1_2, 7, 2, Menu_1_3, Menu_1_1, Menu_1, NULL_MENU, generic_SelectCallback, NULL, "Play with SmartPhone");
MENU_ITEM( Menu_1_3, 7, 3, NULL_MENU, Menu_1_2, Menu_1, NULL_MENU, generic_SelectCallback, NULL, "Auto Play");
MENU_ITEM( Menu_2_1, 7, 1, Menu_2_2, NULL_MENU, Menu_2, NULL_MENU, generic_SelectCallback, NULL, "Tuning Kp");
MENU_ITEM( Menu_2_2, 7, 2, Menu_2_3, Menu_2_1, Menu_2, NULL_MENU, generic_SelectCallback, NULL, "Tuning Ki");
MENU_ITEM( Menu_2_3, 7, 3, NULL_MENU, Menu_2_2, Menu_2, NULL_MENU, generic_SelectCallback, NULL, "Tuning Kd");
MENU_ITEM( Menu_3_1, 7, 1, NULL_MENU, NULL_MENU, Menu_3, NULL_MENU, generic_SelectCallback, enable_debug_mode_display, "press q for debug info");
MENU_ITEM( Menu_4_1, 7, 1, NULL_MENU, NULL_MENU, Menu_4, NULL_MENU, generic_SelectCallback, Graphic_demo_enable, "press q for Graphic DEMO");
MENU_ITEM( Menu_5_1, 7, 1, NULL_MENU, NULL_MENU, Menu_5, NULL_MENU, generic_SelectCallback, Music_demo_enable, "press q for Music DEMO");
static FILE oled_stdout =  FDEV_SETUP_STREAM(oled_putchar_printf, NULL, _FDEV_SETUP_WRITE);
static FILE usart_stdout =  FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

		//FUNCTIONS TO BE USED
		
void plunger(void);

        //MACROS for CAN

#define NORMAL 0x00
#define LOOP_BACK 0x02

        //FLAGS for ISR (Interrupts)

int can_rx_flag=0;
int flag_plunger=0;
int is_main_not_fetch_the_buffer=0;
char rx_buff[20];
int rx_count=0;

int main(void)
{	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
		stdout = &oled_stdout;
		usart_init(MYUBRR);

		fprintf(&usart_stdout,"Usart says hello!\n");
		oled_init();
		oled_clear();
		oled_putstr_inverse("hello world!!\n");
		adc_init();
	
	
		//INTERRUPT CONFIGURATION and Plunger Configuration (INT2 is for CAN and INT 0 is for solenoid)

		DDRB= (1<<PB0);							//Make PB0 as output for plunger
		PORTB = (1<<PB0);
		
		sei();									//Enable Global Interrupts
		GICR |= (1<<INT0) | (1<<INT2);			//Enable INT_2, falling edge (ISC2 is zero)
		MCUCR |= (1<<ISC01);					//Falling edge interrupt for Plunger
	
	
	
		//DATA VARIABLES

		char rx_data[8];
		char tx_data[8];
		uint8_t rx_id[2], *rx_length=0;



		//UART functions

	//	uart_config();
	//	fdevopen(&uart_transmit, (void *) 0);		//UART write function
	//
		//SPI and CAN functions

		spi_config();

		can_reset();

		can_config(NORMAL);		//configure in loop back mode
		//
	Menu_SetGenericWriteCallback(genric_draw_fun);
	Menu_SetGenericClear(oled_clear);
	oled_clear();
	Menu_Navigate(&Menu_1);
	Menu_DrawBase();
	Menu_Navigate(&Menu_1);
	
	
	// default pid 
	tx_data[3] = 8;//KP
	tx_data[4] = 5;//KI
	tx_data[5] = 5;//KD
	/************************************************************************/
	/*      music  init                                                     */
	/************************************************************************/
	InitMusic();

	
	while (1)
    {
		
			switch (button_val)
			{
				case BUTTON_UP:
					Menu_Dsp_flag_clr();
					Menu_Navigate(MENU_PREVIOUS);
					break;
				case BUTTON_DOWN:
					Menu_Dsp_flag_clr();
					Menu_Navigate(MENU_NEXT);
					break;
				case BUTTON_LEFT:
					Menu_Dsp_flag_clr();
					Menu_Navigate(MENU_PARENT);
					break;
				case BUTTON_RIGHT:
					Menu_Dsp_flag_clr();
					Menu_Navigate(MENU_CHILD);
					break;
				case BUTTON_ENTER:
					Menu_Dsp_flag_clr();
					Menu_EnterCurrentItem();
					break;
				default:
					break;
			}
			button_val = BUTTON_NONE;
		
			//while (1)
			{
			adc_pos_t pos = adc_get_position();
			_delay_ms(125);
			/************************************************************************ /
			/ *       arrange  the frame to send to can                              * /
			/ ************************************************************************/
			tx_data[0] = pos.x*0xff00;
			tx_data[1] = pos.x;//(int8_t)((pos.x & 0x8000)? (((pos.x)|0x7f)+ 0x80):(pos.x));
			tx_data[2] = 255-pos.y;
			/*tx_data[3] = 8;//KP
			tx_data[4] = 5;//KI
			tx_data[5] = 5;//KD
			tx_data[6] = 0;
			tx_data[7] = 0;*/
			
			//int x = (0x80 & pos.x )? (pos.x + 0xff00):pos.x;
			//int y = (0x80 & pos.y )? (pos.y + 0xff00):pos.y;
			can_transmit(0,0x05,8,tx_data);			//BUFFER 0, ID 0x05, 8 data bytes and data=stored string
			
			//CAN RECEIVE FLAG PART
			if(can_rx_flag)
			{
				
				/************************************************************************ /
				/ *		Display pattern on OLED
					Inverted pattern, always remains constant
					SCORE (changed from DIODE VALUE val)
					X_value of ADC
					Y_value of ADC
					servo value from node 2
					encoder upper byte from node 2
					encoder lower byte from node 2
																						* /
				/ ************************************************************************/

				can_rx_flag=0;
				can_receive(rx_id,rx_length,rx_data);

			}
			
			// enable debug mode displaying
				if (flag_enable_debug_mode_display)
				{
					int print_temp[7];
					oled_goto_xy(5,1);
					print_temp[0] = (0x80 & tx_data[1] )? (tx_data[1] + 0xff00):tx_data[1];
					printf("data Tran: %d, x:%3d, y:%3d, tranx: %d, %d, %d, %d, %d"\
					, tx_data[0],pos.x,pos.y,tx_data[3],tx_data[4],tx_data[5],tx_data[6],tx_data[7]);		//0 is pattern, 1 is x and 2 is y
					oled_goto_xy(5,4);
					
					int i;
					for (i = 0; i< 7; i++)
					{
						print_temp[i] = (0x80 & rx_data[i] )? (rx_data[i] + 0xff00):rx_data[i];
					}
					int16_t encoder_val = (uint8_t)rx_data[5]*0x100 + (uint8_t)rx_data[6];
					int16_t set_val =  (uint8_t)rx_data[3]*0x100 + (uint8_t)rx_data[4];
					//int a = (0x80 & rx_data[2] )? (rx_data[2] + 0xff00):rx_data[2];
					printf("Receive Data: %3i, %3i, SP: %5d, ENC: %5d\n",print_temp[0],print_temp[1],set_val,encoder_val);
				}



				//PLUNGER FLAG PART

				if(flag_plunger)
				{
					flag_plunger=0;     //clear flag
					plunger();             //call plunger functions
				}
			
				// Terminal command parsing
				if(is_main_not_fetch_the_buffer){
					
					oled_goto_xy(0,0);
					printf("%s",rx_buff);
					is_main_not_fetch_the_buffer = 0;
					// menu navigation cmd
					if (!strcmp("e\n",rx_buff))
					{
						button_val = BUTTON_UP;
					} else if (!strcmp("d\n",rx_buff))
					{
						button_val = BUTTON_DOWN;
					} else if (!strcmp("s\n",rx_buff))
					{
						button_val = BUTTON_LEFT;
					} else if (!strcmp("f\n",rx_buff))
					{
						button_val = BUTTON_RIGHT;
					}  else if (!strcmp("q\n",rx_buff))
					{
						button_val = BUTTON_ENTER;
					}	else 
					// Online Tuning Mode
					if (!strcmp("config\n",rx_buff))
					{	
						Menu_Dsp_flag_clr();
						Menu_Navigate(&Menu_2_1);
						oled_goto_xy(6,0);
						puts("Into PID config mode");
						oled_goto_xy(2,5);
						printf("Current Kp%2d, Ki%2d, Kd%2d",tx_data[3],tx_data[4],tx_data[5]);
						flag_enable_online_tuning = 1;
					}else 
					// Play Mode
					if (!strcmp("play\n",rx_buff))
					{	
						Menu_Dsp_flag_clr();
						Menu_Navigate(&Menu_1_1);
						oled_goto_xy(6,0);
						puts("Into Play mode");
					}
					if (!strcmp("debug\n",rx_buff))
					{
						Menu_Dsp_flag_clr();
						Menu_Navigate(&Menu_3_1);
						/*oled_goto_xy(6,0);
						puts("Into Debug mode");*/
						Menu_EnterCurrentItem();
					}
					
					/************************************************************************/
					/*    online changing pid gains enabled                                 */
					/************************************************************************/
					if (flag_enable_online_tuning)
					{	
						int getval;
						if (!strncmp("kp ",rx_buff,3)){
							sscanf(rx_buff,"kp %d\n", &getval);
							tx_data[3] = (char) getval; 
							printf("new kp: %2d\n",tx_data[3]);
						} else if (!strncmp("ki ",rx_buff,3))
						{
							sscanf(rx_buff,"ki %d\n", &getval);
							tx_data[4] = (char) getval;
							printf("new ki: %2d\n",tx_data[3]);
						} else if (!strncmp("kd ",rx_buff,3))
						{
							sscanf(rx_buff,"kd %d\n", &getval);
							tx_data[5] = (char) getval;
							printf("new kd: %2d\n",tx_data[3]);
						}
						oled_goto_xy(2,5);
						printf("Current Kp%2d, Ki%2d, Kd%2d",tx_data[3],tx_data[4],tx_data[5]);
						
					}
					// music
					if (flag_enable_music_demo)
					{
						flag_enable_music_demo = 0;
						cli();
						MusicDemo();
						sei();
					}
					// graphic
					if (flag_enable_graphic_demo)
					{
						flag_enable_graphic_demo = 0;
						cli();
						oled_clear_buffer();
						oled_goto_xy_buffer(23,4);
						oled_putstr_buffer("hello world!!\n");
						uint16_t x = 0;
						
						do{
							x++;
							oled_draw_circle(64,32,x);
							oled_draw_rect(12,13,2*x,x);
							_delay_ms(100);
						} while(x<=32);
						sei();
					}
					
				}
			}
	}
}


/************************************************************************/
/*			PLUNGER FUCNTION TO ACTIVATE SOLENOID                      */
/************************************************************************/
void plunger(void)
{
	cli();
	PORTB &= ~(1<<PB0);			// clear PORTB_0 pin 1 when switch is pressed, will be used for plunger
						
	_delay_ms(50);             //can be changed!!!
	PORTB |= (1<<PB0);			// Set PORTB_0
	sei();
}

      /************************************************************************/
      /*
                    ISR 0, ISR2 and ISR UART_RX (channel 1)
					
                => ISR 2 is for CAN (RECEIVE)
                
				=> ISR 0 is for PLUNGER (SOLENOID), JOYSTICK SWICTH IS SOURCE
				
				=> ISR UARTRX is for completion of UART Data Receive, used for interacting
							with keyboard and changing modes of game
							(Initialize, Play and Config) 
                                                                               */
      /************************************************************************/


ISR(INT2_vect)
{
	can_rx_flag=1;					//set receive (CAN receive NOT UART!!!) flag for main function, flag will be cleared in main.
	GIFR &= ~(1<<INTF2);		//clear interrupt flag
}


ISR(INT0_vect)
{
	flag_plunger=1;		//set flag
	GIFR &= ~(1<<INTF0);	//clear flag of interrupt
}


ISR ( USART1_RXC_vect)
{
	
		if(!is_main_not_fetch_the_buffer){
			
			rx_buff[rx_count]=UDR1;
			
			
			//UCSR1A &= ~(1<<RXC1);		//Clear Flag
			//UCSR1A |= (1<<UDRE1);		//Flush the buffer and enable it for next transmission
			UCSR1A &= ~(1<<RXC1);		//Clear Flag
			
			if(rx_buff[rx_count]=='\r'||rx_buff[rx_count]=='\n'||rx_count>18)
			{
				rx_buff[rx_count+1] = '\0';
				rx_count=0;
				is_main_not_fetch_the_buffer=1;
			
			}else{
				rx_count++;
				
				
			}	
		}
		
}
