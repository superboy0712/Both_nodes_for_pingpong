#define F_CPU 4915200UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "oled.h"
#include "MicroMenu.h"
#include "ADC.h"

/*** DUMY CODE ***/
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
enum ButtonValues GetButtonPress(void)
{
	return BUTTON_NONE;
};
/*** END DUMY CODE ***/



static void genric_draw_fun(uint8_t x, uint8_t y, const char *str){
	oled_goto_xy(x,y);
	oled_putstr_P(str);
}

void generic_SelectCallback(uint8_t x, uint8_t y, const char* str){
	oled_goto_xy(x,y);
	oled_putstr_P_inverse(str);
}

/** Generic function to write the text of a menu.
 *
 *  \param[in] Text   Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
 */

MENU_ITEM( Menu_1, (5), (2), Menu_2, NULL_MENU, NULL_MENU, Menu_1_1, generic_SelectCallback, NULL, "I love Zhang Yun!\n");
MENU_ITEM( Menu_2, (10), (3), Menu_3, Menu_1, NULL_MENU, NULL_MENU, generic_SelectCallback, NULL, "I am Second\n");
MENU_ITEM( Menu_3, (15), (4), Menu_4, Menu_2, NULL_MENU, NULL_MENU, generic_SelectCallback, NULL, "I am Third\n");
MENU_ITEM( Menu_4, (20), (5), NULL_MENU, Menu_3, NULL_MENU, NULL_MENU, generic_SelectCallback, NULL, "I am Fourth\n");
MENU_ITEM( Menu_1_1, 0, 0, Menu_1_2, NULL_MENU, Menu_1, NULL_MENU, generic_SelectCallback, NULL, "1.1");
MENU_ITEM( Menu_1_2, 0, 1, NULL_MENU, Menu_1_1, Menu_1, NULL_MENU, generic_SelectCallback, NULL, "1.2");
static FILE oled_stdout =  FDEV_SETUP_STREAM(oled_putchar_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{	/* Set up the default menu text write callback, and navigate to an absolute menu item entry. */
		stdout = &oled_stdout;
		oled_init();
		oled_clear();
		oled_putstr_inverse("hello world!!\n");
		adc_init();
	Menu_SetGenericWriteCallback(genric_draw_fun);
	Menu_SetGenericClear(oled_clear);
	
	Menu_Navigate(&Menu_1);
	Menu_DrawBase();
	Menu_Navigate(&Menu_1);
	
	while (1)
    { 
		adc_pos_t pos = adc_get_position();
		//printf(" x: %d, y:%d \n", pos.x, pos.y);
		if( -20 < pos.x && pos.x < 20){
			if (pos.y > 70)
			{
				button_val = BUTTON_UP;
			} else if (pos.y < -70)
			{
				button_val = BUTTON_DOWN;
			}	
		}
		
		if( -20 < pos.y && pos.y < 20){
			if (pos.x > 70)
			{
				button_val = BUTTON_RIGHT;
			} else if (pos.x < -70)
			{
				button_val = BUTTON_LEFT;
			}
		}

		/* Example usage of Micromenu - here you can create your custom menu navigation system; you may wish to perform
		 * other tasks while detecting key presses, enter sleep mode while waiting for user input, etc.
		 */
		switch (button_val)
		{
			case BUTTON_UP:
				Menu_Navigate(MENU_PREVIOUS);
				break;
			case BUTTON_DOWN:
				Menu_Navigate(MENU_NEXT);
				break;
			case BUTTON_LEFT:
				Menu_Navigate(MENU_PARENT);
				break;
			case BUTTON_RIGHT:
				Menu_Navigate(MENU_CHILD);
				break;
			case BUTTON_ENTER:
				Menu_EnterCurrentItem();
				break;
			default:
				break;
		}
		button_val = BUTTON_NONE;
		_delay_ms(200);
    }
}
