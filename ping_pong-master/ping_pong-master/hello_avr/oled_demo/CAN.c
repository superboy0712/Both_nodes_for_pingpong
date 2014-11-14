#define F_CPU 4195200UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "CAN.h"





		//PIN DECLARATIONS FOR SPI
#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define ENABLE 1
#define DISABLE 0



		//CAN Declarations

#define WRITE_CMD 0b00000010
#define READ_CMD 0b00000011
#define RESET 0b11000000
#define RX_STATUS_CMD 0b10110000
#define BIT_MODIFY_CMD 0x05
#define RX_MASK 0xff					// SERIOUS !!!!!!!
#define INT_FLAG_ADDR 0x2C
#define RX_INT_MASK 0x0
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28
#define BRP_value 0x03			//BRP=3
#define PH1_PRP_value 0xD1		//Propagation=2. PhSEG1=3,  beware of MSB!!!!
#define PH2_value 0x41			//PhSEG2=2,    beware of upper two bits (MSB)!!!!








		//SPI FUNCTIONS

void spi_config(void)
{
	DDRB |= (1<<SS) | (1<<MOSI) | (1<<SCK);		//set MOSI, SS and SCK as output
	PORTB|=(1<<SS);								//set SS high to disable slave
	SPCR |= (1<<SPE) | (1<<MSTR);				//enable spi in master mode,non-interrupt,MSB first,Leading Edge=Rising,sample at leading edge,fosc/4 is SCK

}


void spi_chipselect(uint8_t signal)
{
	if(signal==1)
	{
		PORTB &= ~(1<<SS);					//set SS=0 and enable slave, define enable 1 in main prog!!!
	}
	else
	{
		PORTB |=(1<<SS);					//set SS=1 and disable slave, define disable 0 in main prog!!!
	}
}

unsigned char spi_tx_rx(unsigned char data)
{
	char rx_data;

	SPDR=data;							//sent data on SPI

	while(!(SPSR & (1<<SPIF)));			//wait until flag is not equal to 1

	rx_data=SPDR;						//receive data from spi
	return rx_data;
}


			//CAN FUNCTIONS
			
		

void can_reset(void)
{
	//ENABLE SLAVE AND SEND TRANSMIT CODE
	spi_chipselect(ENABLE);
	spi_tx_rx(RESET);
	spi_chipselect(DISABLE);
}


void can_rx_if_clear(void)
{
	
	spi_chipselect(ENABLE);
	
	spi_tx_rx(BIT_MODIFY_CMD);			
	spi_tx_rx(INT_FLAG_ADDR);
	spi_tx_rx(RX_MASK);			
	spi_tx_rx(0x00);				//send 0x00 to clear flags
	
	spi_chipselect(DISABLE);
}


void can_transmit(uint8_t buff_num,uint8_t msg_id,uint8_t length, char *data)		//ask that can we for address use 8_t!!!!!
{

	unsigned char temp=0;
	//always maximum priority is used, standard frames no remote request

	spi_chipselect(ENABLE);		//enable slave (mcp)


			//check for pending transmissions

	
	spi_tx_rx((unsigned char)READ_CMD);

	spi_tx_rx((unsigned char)0x30+(buff_num*0x10));		//send address of Transmit Control Register (add 10 to base address to point to other buffers)

	temp=spi_tx_rx((unsigned char)0);			//send zero and get status of buffer (Transmit Control Register) in temp
	
	/*while(temp & 0x08)				//0x08 because TXREQ is position 3 in Transmit control buffer!!!!
	{
		spi_chipselect(ENABLE);
		// temp=spi_tx_rx((unsigned char)0);		//keep receiving status of control Transmit Control Register until TXREQ is 0 !!!!
		spi_tx_rx((unsigned char)READ_CMD);

		spi_tx_rx((unsigned char)0x30+(buff_num*0x10));		//send address of Transmit Control Register (add 10 to base address to point to other buffers)

		temp=spi_tx_rx((unsigned char)0);			//send zero and get status of buffer (Transmit Control Register) in temp
		
		spi_chipselect(DISABLE);
	}
*/
	spi_chipselect(DISABLE);
	


			//preparing TX buffer to start transmission
	
		//priority
		
	spi_chipselect(ENABLE);

	spi_tx_rx((unsigned char)WRITE_CMD);		//send write command when flag is cleared

	spi_tx_rx((unsigned char)0x30+(buff_num*0x10));		//send address of Transmit Control Register (add 10 to base address to point to other registers)

	spi_tx_rx((unsigned char)0x03);			//send message priority as highest (first 2 bits in control register)!!!!

	spi_chipselect(DISABLE);
	
	//higher byte of ID
	
	spi_chipselect(ENABLE);

	spi_tx_rx(WRITE_CMD);
	
	spi_tx_rx(0x31+(buff_num*0x10));		//send address of Transmit Buffer Std. Identifier HIGH Register (add 10 to base address to point to other registers)

	spi_tx_rx((unsigned char)msg_id>>3);			//shift out lower 3 bits!!!!

	spi_chipselect(DISABLE);
	
		//lower byte of ID
	
	spi_chipselect(ENABLE);
	
	spi_tx_rx(WRITE_CMD);
	
	spi_tx_rx((unsigned char)0x32+(buff_num*0x10));		//send address of Transmit Buffer Std. Identifier LOW Register (add 10 to base address to point to other registers)

	spi_tx_rx((unsigned char)msg_id<<5);			//shift lower 5 bits to higher position!!!!
	
	spi_chipselect(DISABLE);
	
		//length of data
	
	spi_chipselect(ENABLE);
	
	spi_tx_rx(WRITE_CMD);

	spi_tx_rx((unsigned char)0x35+(buff_num*0x10));		//send address of Transmit Buffer Data Length Register, RTR is always 0!!!

	spi_tx_rx((unsigned char)(length & 0x0F));			//only lower 4 bits of length are used, RTR=0
	
	spi_chipselect(DISABLE);


			//NOW DATA IS SENT
	spi_chipselect(ENABLE);
	spi_tx_rx(WRITE_CMD);
	spi_tx_rx((unsigned char)((0x36+(buff_num*0x10))));

	unsigned char loop_var;
	
	 for(loop_var=0;loop_var<length;loop_var++)
	 {
		 //spi_tx_rx((unsigned char)((0x36+(buff_num*0x10))+loop_var));		//transmit buffer data registers addresses

		 spi_tx_rx(data[loop_var]);				//data to be sent!!!!

	 }

	spi_chipselect(DISABLE);
	
		//set TXREQ to initiate transmission
	
	spi_chipselect(ENABLE);
	spi_tx_rx(WRITE_CMD);
	

			//TX initiation part


	spi_tx_rx((unsigned char)0x30+(buff_num*0x10));		//send address of Transmit Control Register (add 10 to base address to point to other registers)

	spi_tx_rx((unsigned char)0x08);			//set TXREQ to start transmission, it is 4th bit!!!!

	spi_chipselect(DISABLE);				//disable slave
	
	//clear flag!!! YULONG THE GREAT
	
	can_rx_if_clear();

}

void can_config(int mode)
{
	can_reset();		//reset mcp

	spi_chipselect(ENABLE);		//enable slave

	spi_tx_rx(WRITE_CMD);

		//Enter config mode


            //Send address of CAN CONTROL registerxFF because 0xXFH is mentioned
			//0b10000000 to enter config mode
			//be aware of potential delay while setting config_mode
			
    can_write_reg(0xFF,0x80);			//check config mode is achieved or not
	
	spi_chipselect(DISABLE);
	
	if(can_read_status()>>5 !=0x04)
	{
		printf("\n\rERROR UNABLE TO ENTER CONFIG MODE\n\r");
	}	
	
	else 
	{
		printf("\n\r CONFIG MODE ACHIEVED\n\r");
	}


	//set filter and mask registers we are using 0x00 for mask so that all messages are accepted!!!

        //send mask address

        //send 0x00 to enable all messages
		_delay_us(1000); // TODO remove this test
		

	spi_chipselect(ENABLE);		//enable slave
	spi_tx_rx(WRITE_CMD);
    can_write_reg(0x20,0x00);
	spi_chipselect(DISABLE);

	_delay_us(1000); // TODO remove this test

	spi_chipselect(ENABLE);		//enable slave
	spi_tx_rx(WRITE_CMD);
	can_write_reg(0x24,0x00);
	spi_chipselect(DISABLE);
	
	_delay_us(1000); // TODO remove this test
	
	spi_chipselect(ENABLE);		//enable slave
	spi_tx_rx(WRITE_CMD);
	can_write_reg(0x21,0x00);
	spi_chipselect(DISABLE);
	
	_delay_us(1000); // TODO remove this test
	
	spi_chipselect(ENABLE);		//enable slave
	spi_tx_rx(WRITE_CMD);
	can_write_reg(0x25,0x00);
	spi_chipselect(DISABLE);

	_delay_us(1000); // TODO remove this test
	//Enable interrupts for reception

	//address of CAN Interrupt register

	//enable RX0 and RX1 interrupt only!!!!
    spi_chipselect(ENABLE);		//enable slave
    spi_tx_rx(WRITE_CMD);
	can_write_reg(0x2B,0x03);
	spi_chipselect(DISABLE);
	
	_delay_us(1000); // TODO remove this test
	
	
	//bit timing part
/*unsigned char setBitTiming(unsigned char rCNF1, unsigned char rCNF2, unsigned char rCNF3){

if(getMode == CONFIGURATION_MODE) {
mcp2515_write_byte(CNF1, rCNF1);
mcp2515_write_byte(CNF2, rCNF2);
mcp2515_bit_modify(CNF3, 0x07, rCNF3);
return 1;
}
return 0;
}
*/

 spi_chipselect(ENABLE);		//enable slave
 spi_tx_rx(WRITE_CMD);
 can_write_reg(CNF1,BRP_value);
 spi_chipselect(DISABLE);
 
 	_delay_us(1000); // TODO remove this test
 
 spi_chipselect(ENABLE);		//enable slave
 spi_tx_rx(WRITE_CMD);
 can_write_reg(CNF2,PH1_PRP_value);
 spi_chipselect(DISABLE);
 
 	_delay_us(1000); // TODO remove this test
  
 spi_chipselect(ENABLE);		//enable slave
 spi_tx_rx(WRITE_CMD);
 can_write_reg(CNF3,PH2_value);
 spi_chipselect(DISABLE);
 
 	_delay_us(1000); // TODO remove this test





	//goto normal mode
			//Send address of CAN CONTROL registerxFF because 0xXFH is mentioned

			//mode, to shift bytes  to higher nibble enter normal mode
	spi_chipselect(ENABLE);		//enable slave
	spi_tx_rx(WRITE_CMD);			
    can_write_reg(0xFF,(mode<<5));			//mode should be sent as 0,1 and 2 from main since it is left shifted here by 5
	spi_chipselect(DISABLE);

}

void can_write_reg(uint8_t addr, char data)
{
    spi_tx_rx((unsigned char) addr);
    spi_tx_rx((unsigned char)data);
}

void can_receive(uint8_t *id,uint8_t *length, char *data)
{
    unsigned char temp_data[14]={0};
    uint8_t buff_id=0,loop_var=0;
  

            //ID of RX buffer which received message

	spi_chipselect(ENABLE);
	
	spi_tx_rx((unsigned char)RX_STATUS_CMD);      //send RX Status Command

    buff_id=spi_tx_rx(0);			//to exclude first byte because of suspected error 
	buff_id=spi_tx_rx(0);			//to ensure we get correct byte because data is repeating and previous can be corrupted
	
	spi_chipselect(DISABLE);
	
    buff_id=(buff_id>>6);               //get value of buffer id into higher two bytes
	
	
    //message reception
	
	spi_chipselect(ENABLE);
    spi_tx_rx(READ_CMD);

    spi_tx_rx((unsigned char) 0x60 + (buff_id-1));      //id can have value of  or 2, so subtract 1 in order to make it 0 or 1


     for(loop_var=0;loop_var<14;loop_var++)
     {
         temp_data[loop_var]=spi_tx_rx(0);          //receive whole data
     }
	 
     id[0]=temp_data[1]<<3;
     id[1]=temp_data[2]>>5;

     length[0]=temp_data[5];        //5th byte is length

     for(loop_var=6;loop_var<14;loop_var++)
     {
         data[loop_var-6]=temp_data[loop_var];              //data starts from  6th bye!!!
     }
	 
	 
		//Clear interrupts
		
	/*	
	 spi_chipselect(ENABLE);
	 
	 spi_tx_rx(WRITE_CMD);
	 
	 spi_tx_rx(INT_FLAG_ADDR);
	 
	 spi_tx_rx(0x00);
	 
	 spi_chipselect(DISABLE);*/
	can_rx_if_clear();
	//can_reset();
}


char can_read_reg(uint8_t addr)
{
	char data=0;
	spi_chipselect(ENABLE);
	spi_tx_rx(READ_CMD);
	spi_tx_rx(addr);
	data=spi_tx_rx(0);
	return data;
}


char can_read_status(void)
{
	char data=0;
	spi_chipselect(ENABLE);
	spi_tx_rx(READ_CMD);
	spi_tx_rx(0xFE);		//address of data register
	data=spi_tx_rx(0);		//upper 3 bits are mode of operation, bit 1 till 3 are for interrupt 
	return data;			//for mode, shift by 5, for interrupt, and with 0b00001110 and shift by 1
}