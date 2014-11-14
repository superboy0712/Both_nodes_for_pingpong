/*****************************************************************
 * mem_map.h
 * hello_avr
 *
 *  Created on		: Sep 16, 2014 
 *  Author			: yulongb
 *	Email			: yulongb@stud.ntnu.no
 *  Description		:
 *****************************************************************/

#ifndef MEM_MAP_H_
#define MEM_MAP_H_

#define OLED_ADDR_CMD_START		(0x100f)
#define OLED_ADDR_CMD_END		(0x11ff)
#define OLED_ADDR_DATA_START	(0x120f)
#define OLED_ADDR_DATA_END		0x13ff
#define ADC_ADDR_START			0x1400
#define ADC_ADDR_END			0x17ff
#define SRAM_ADDR_START			0x1800
#define SRAM_ADDR_END			0x1fff
#define DISP_BUFFER_START		0X1900
#define DISP_BUFFER_END			0X1CFF
#endif /* MEM_MAP_H_ */
