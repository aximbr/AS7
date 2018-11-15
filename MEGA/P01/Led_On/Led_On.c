/*
 * Led_On.c
 *
 * Created: 06/08/2018 23:33:17
 * Author : jleitao
 */ 
#define F_CPU 16000000
#include <avr/io.h>


int main(void)
{
    /* Habilita PB7 como saida */
	DDRB = (1<<DDB7);
	
	/* Liga o PB7 onde se encontra o LED */
	PORTB |= (1<<PB7);
	
    while (1) 
    {
    }
}


