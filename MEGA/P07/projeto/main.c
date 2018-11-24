/*
 * Projeto 7 - piscar um LED quando pressionado um switch
 *
 * Created: 24/11/2018
 * Author : jleitao
 */ 
//#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef _BV
#define _BV(n) (1 << n)
#endif

#define MyLED            (DDB6)   //B6 como output
#define MySW             (DDF1)   //F1 como input
#define DEBOUNCE_TIME    3       //Debounce for switch

/* Global variables */

int volatile tick_count;  //contador de eventos

/* Interrrupt Handle for TIMER1 */
ISR(TIMER1_COMPA_vect)
{
	/*cada vez que o contador atinge o valor do comparador */
	tick_count++;
}


void MyDelay(unsigned int delay_ms){
	tick_count = 0;
	while(tick_count < delay_ms);
}


int main(void)
{
    uint8_t SW_Read, SW_Read_0, SW_Read_1, SW_State;
    SW_Read_0 = 0;
    SW_Read_1 = 0;
	SW_State = 0;
	
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
	TIMSK1 |= (1 << OCIE1A);
	
	// enable global interrupts:
	sei();
  
	/*configura os pinos do LED e do SW */
	
	/* inicializa o LED */
	DDRB |= _BV(MyLED); // Pin B6 is OUTPUT

	/* inicializa as portas */
	DDRF &= ~_BV(MySW);  // Pin F1 is INPUT
	PORTF |= _BV(MySW);  // Pin F1 with pull-up resistor
	
    while (1) 
    {
		SW_Read = !(PINF & _BV(MySW)); //lê status da chave
		
		if (SW_Read == 0) {
			SW_Read_0++;
			SW_Read_1 = 0;
			if (SW_Read_0 >= DEBOUNCE_TIME){
				SW_Read_0 = DEBOUNCE_TIME +1;
			    SW_State = 0;}
		}
		else {
			SW_Read_1++;
		    SW_Read_0 = 0;}
		
		if (SW_Read_1 >= DEBOUNCE_TIME){
			SW_Read_1 = DEBOUNCE_TIME +1;
		    SW_State = 1;}
		
		if (SW_State) { 
			DDRB = _BV(MyLED); //blink LED
			MyDelay(250);
			DDRB &= ~_BV(MyLED);
			MyDelay(250);
		    }
		else DDRB &= ~_BV(MyLED);        //turn-off LED
		
    }
}

