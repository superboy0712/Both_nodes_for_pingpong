#ifndef CAN_H
#define CAN_H


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
