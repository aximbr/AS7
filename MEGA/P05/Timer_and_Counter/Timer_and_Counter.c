/*
 * Timer_and_Counter.c
 *
 * Created: 12/08/2018 10:00:24
 * Author : jleitao
 * Etapa 4: Piscar o Led usando o Timer e um contador 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//ISR(TIMER1_COMPA_vect)
//{
	/*cada vez que o contador atinge o valor do comparador */
//	PINB |=(1<<PB7);
//}

int volatile i;  //contador de eventos

int main(void)
{
    /*inicializa o timer1 (16 bits) */
	cli();  // desabilita interrupções
	TCNT0 = 0;                   //Limpa o contador
	//Limpa a programação do TCCR1
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	
	/*clock= 16000000 Hz
	* prescaler = 1024
	* timer resolution = 1/(clock/prescaler) = 64 us = resolution
	* tempo desejado (t) = 1 ms
	* quantidade de ticks necessários para chegar ao tempo (t) desejado = count
	* count = (t/resolution) - 1
	* count = (1 ms/64 us) -1 = 15   */
	
	// set compare match register to desired timer count:
	OCR1A = 15;
	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler:
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	// enable timer compare interrupt:
	//TIMSK1 |= (1 << OCIE1A);
	
	// enable global interrupts:
	sei();
	
	/* Define a porta onde está o LED (pino 13) como saida */
	DDRB = (1<<DDB7);
	
	/* um tick dura 1 ms. Conta 250 ms e pisca o LED */
    while (1) 
    {
		if(TIFR1 & (1<<OCF1A)) {
			i++;  //conta mais um tick
			TIFR1 |= (1<<OCF1A);  //reset o comparador
			if(i==250) { //250 x 1 ms
				PINB |=(1<<PB7);  //pisca o LED
				i = 0;     //reinicia o contador
			}
		}
			
    }
}

