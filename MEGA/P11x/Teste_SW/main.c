/*
 * Teste_SW.c
 *
 * Created: 01/11/2018 22:38:04
 * Author : jleitao
 */ 

#include <avr/io.h>
#define MySW   DDF1
#define MyLED  DDB7

int main(void)
{
    
	/*configura os pinos do LED e do SW */
	
	/* inicializa as portas */
	DDRF &= ~(1UL << MySW);  //Pin F1 is INPUT
	PORTF |= (1UL << MySW);  // Pin F1 with pull-up resistor
	
	DDRB |=  (1UL << MyLED); // Pin B7 is OUTPUT
    while (1) 
    {
		while (!(PINF & (1UL << MySW))) { 
			PINB |= (1UL << MyLED);  //toggle the LED
			for ( uint32_t j=0; j< 200000; j++); //delay
			}
		 PORTB |= (1UL << MyLED); //apaga o LED
    }
}

