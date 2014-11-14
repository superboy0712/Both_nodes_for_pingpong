#ifndef CAN_H
#define CAN_H

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

void spi_config(void);						//function to configure spi communication
unsigned char spi_tx_rx(unsigned char data);					//function to receive and transmit data
void spi_chipselect(uint8_t signal);			//to enable/disable chip select signal


		//CAN FUNCTIONS


		//char type is used for data and unit8_t is  used for address and id (as standard)


void can_config(int mode);					//mode is to be given as argument
void can_set_timing(void);
void can_transmit(uint8_t buff_num,uint8_t msg_id,uint8_t length, char *data);		//buff_id= buffer number, msg_id=address of node, length=no. of data bytes,standard frames are used!!!!
void can_receive(uint8_t *id, uint8_t *length, char *data);		//received data and id is returned using pointers, standard frames are used with 8 data bytes!!!
char can_read_status(void);							//get mode of can controller
char can_read_reg(uint8_t addr);			//read contents of can controller register
void can_write_reg(uint8_t addr, char data); //write value to a can controller register
void can_reset(void);						//reset can controller
void can_rx_if_clear(void);			//bit modify for RX interrupts! N.B. changed to void!!!
#endif
