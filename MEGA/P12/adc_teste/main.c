/*
 * adc_teste.c
 *
 * Created: 25/11/2018 14:37:47
 * Author : jleitao
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include "adc_MEGA.h"
#include "SPI_Master_H_file.h"
#include "nokia5110_MASTER.h"

volatile uint16_t x;

int main(void)
{
    /* Replace with your application code */
	adc_mega_init();
	SPI_Init();
	N5110_init();
	N5110_clear();
	
	
    while (1) 
    {
		x = adc_mega_read();
		
		lcd_setXY(0,2);
		N5110_OutUDec( x );
		for(int i=0; i<5000;i++);
		
    }
}

