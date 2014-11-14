#ifndef ADC_H
#define ADC_H

void adc_init(void);
//void adc_write(int channel);
uint8_t adc_read(int channel);
uint8_t adc_calibrate(int channel);
//void position(signed int *x_val,signed int *y_val,uint8_t x_mean,uint8_t y_mean);
struct adc_pos {uint8_t x; int16_t y;};
typedef struct adc_pos adc_pos_t;
adc_pos_t adc_get_position(void);
#endif