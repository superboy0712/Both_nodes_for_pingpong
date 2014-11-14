#ifndef TWI_NODE_2_h
#define TWI_NODE_2_h

void TWI_funct(uint8_t addr,uint8_t data);

void TWI_init(void);

void TWI_start(void);

void TWI_stop(void);

void TWI_write(uint8_t data);

#endif