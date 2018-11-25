/*
 * adc_MEGA.c
 *
 * Created: 25/11/2018 10:22:03
 *  Author: jleitao
 */
#include <stdint.h> 
#include "adc_MEGA.h"
#include <avr/io.h>

void adc_mega_init(void){
	/* Set ADC single convertion, ADC_CLK = Main_Clk/2, Result is right aligned
	* input is ADC0 (F0)
	*
	*/
	//16MHz/128 = 125kHz the ADC reference clock and enable ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //pre-scaler 128
	ADMUX |= (1 << REFS0); //AVCC (5V) as reference
	//ADCSRA |= (1 << ADATE); //Free running mode
	ADCSRA |= (1 << ADEN); //Enable ADC
	
}

uint16_t adc_mega_read(void){
	uint16_t result;
	ADCSRA |= (1 <<ADSC);
	while(!(ADCSRA & (1<<ADIF))); // wait for conversion
	result = ADCW;
	return result;
}