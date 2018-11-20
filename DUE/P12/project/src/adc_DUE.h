/*
 * adc_DUE.h
 *
 * Created: 18/11/2018 11:19:11
 *  Author: jleitao
 */ 
#include <stdint.h>

#ifndef ADC_DUE_H_
#define ADC_DUE_H_

//Prototype for my functions
void adc_due_init(void);

uint16_t adc_due_read(void);


#endif /* ADC_DUE_H_ */