/*
 * adc_DUE.c
 *
 * Created: 18/11/2018 11:18:40
 *  Author: jleitao
 */ 
#include "adc_DUE.h"
#include "adc.h"

void adc_due_init(void){
	const enum adc_channel_num_t channel_list[]={ADC_CHANNEL_0,};
	//uint32_t main_clock = sysclk_get_cpu_hz();
		
	adc_reset(ADC);	
	adc_init(ADC, 48000000, ADC_FREQ_MAX, ADC_STARTUP_TIME_1);
	adc_set_resolution(ADC, ADC_12_BITS);
	adc_configure_trigger(ADC, ADC_TRIG_SW, ADC_MR_FREERUN_OFF);
	adc_configure_sequence(ADC, channel_list, 1);
	adc_enable_channel(ADC, ADC_CHANNEL_0);
	
}

uint16_t adc_due_read(void){
	uint16_t result;
	adc_start(ADC);
	result =  (uint16_t)(adc_get_channel_value(ADC, ADC_CHANNEL_0) & 0x0FFF);
	return result;
}

