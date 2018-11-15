/*
 * 2Led_Blinky.c
 *
 * Created: 08/08/2018 22:50:37
 * Author : jleitao
 * Etapa 03 : Fazer 2 Leds piscarem
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Ativa a Porta B, pino 0 e 7 como saída */
    DDRB |= (1<<DDB0) | (1<<DDB7);
	PORTB |= (1<<PB0) | (1<<PB7);  // Acende os Leds conectados em PB0 (53) e PB7 (13)
    while (1) 
    {
		PINB |= (1<<PB0); //Alterna o Led em PB0
		_delay_ms(250);
		PINB |= (1<<PB7);  //Alterna o Led em PB7
		_delay_ms(250);
    }
}

