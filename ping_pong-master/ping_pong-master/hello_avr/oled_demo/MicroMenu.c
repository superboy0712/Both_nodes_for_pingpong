#include "MicroMenu.h"

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t PROGMEM NULL_MENU = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
static void (*MenuWriteFunc)(uint8_t x, uint8_t y, const char* Text) = NULL;
static void (*MenuClrFunc)(void) = NULL;
/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;


Menu_Item_t* Menu_GetCurrentMenu(void)
{
	return CurrentMenuItem;
}

void Menu_Navigate(Menu_Item_t* const NewMenu)
{
	if ((NewMenu == &NULL_MENU) || (NewMenu == NULL))
		return;
	static Menu_Item_t* LastMenuItem;
	LastMenuItem = CurrentMenuItem;
	CurrentMenuItem = NewMenu;
	if (MENU_ITEM_READ_POINTER(&LastMenuItem->Parent) != MENU_ITEM_READ_POINTER(&NewMenu->Parent))
	{
		// not the same level navigation, need to be refreshed
		MenuClrFunc();
	}
	

/*
	if (MenuWriteFunc)
		MenuWriteFunc(pgm_read_byte(&(CurrentMenuItem->pos_x)),pgm_read_byte(&(CurrentMenuItem->pos_y)),CurrentMenuItem->Text);
*/

	void (*SelectCallback)(uint8_t x, uint8_t y, const char* Text) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->SelectCallback);
	
	Menu_DrawBase();
	if (SelectCallback)
		SelectCallback(pgm_read_byte(&(CurrentMenuItem->pos_x)),pgm_read_byte(&(CurrentMenuItem->pos_y)),CurrentMenuItem->Text);
}

void Menu_SetGenericWriteCallback(void (*WriteFunc)(uint8_t x, uint8_t y, const char* Text))
{
	MenuWriteFunc = WriteFunc;
	Menu_Navigate(CurrentMenuItem);
}
void Menu_SetGenericClear(void (*ClrFunc)(void)){
	MenuClrFunc = ClrFunc;
}
void Menu_EnterCurrentItem(void)
{
	if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
		return;

	void (*EnterCallback)(void) = MENU_ITEM_READ_POINTER(&CurrentMenuItem->EnterCallback);

	if (EnterCallback)
		EnterCallback();
}

/************************************************************************/
/* draw the base of the whole menu, all same level items of new         */
/************************************************************************/
void Menu_DrawBase(void){
	Menu_Item_t * ptr = Menu_GetCurrentMenu();// payattention here you need some thing
	if ((ptr == &NULL_MENU) || (ptr == NULL))
	return;
	// get the first item of the same level
	while (
		(MENU_ITEM_READ_POINTER(&ptr->Previous) != &NULL_MENU)
		 && (MENU_ITEM_READ_POINTER(&ptr->Previous) != NULL)
		 )
		ptr = MENU_ITEM_READ_POINTER(&ptr->Previous);
		
	// get the first item of the same level
	if (MenuWriteFunc){
		MenuWriteFunc(pgm_read_byte(&(ptr->pos_x)),pgm_read_byte(&(ptr->pos_y)),ptr->Text);
		while (( MENU_ITEM_READ_POINTER(&ptr->Next) != &NULL_MENU) && ( MENU_ITEM_READ_POINTER(&ptr->Next) != NULL)) {
			ptr = MENU_ITEM_READ_POINTER(&ptr->Next);
			MenuWriteFunc(pgm_read_byte(&(ptr->pos_x)),pgm_read_byte(&(ptr->pos_y)),ptr->Text);
		}
	}
}