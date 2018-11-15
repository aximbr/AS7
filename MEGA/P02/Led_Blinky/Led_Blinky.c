/*
 * Led_Blinky.c
 *
 * Created: 06/08/2018 23:55:13
 * Author : jleitao
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Ativa a Porta B, pino 7 como saída */
	DDRB = (1<<DDB7);
	
	/* Loop onde o Led pisca a cada 250 ms */
    while (1) 
    {
		PINB |=(1<<PB7);
		_delay_ms(250);
		
    }
}

