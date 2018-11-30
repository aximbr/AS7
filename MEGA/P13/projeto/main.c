/*
 * Projeto 13 - simples biblioteca para comunicar com display de 7 segmentos (4 dígitos) 
 * utilizando o Multi function shield - baseado no CHIP 74HC595
 * Created: 27/11/2018 
 * Author : jleitao
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef _BV
#define _BV(n) (1 << n)
#endif

#define bitSet(_port,_pin)    ((_port) |= _BV(_pin))
#define bitClear(_port,_pin)  ((_port) &= ~_BV(_pin))

/* Define shift register pins used for seven segment display */
#define LATCH_PORT  PORTG
#define CLK_PORT    PORTH
#define DATA_PORT   PORTH

#define LATCH_PIN 5  //D4 <-> PG5
#define CLK_PIN   4  //D7 <-> PH4
#define DATA_PIN  5  //D8 <-> PH5

#define REFRESH 1000  //Taxa de refresh ou base de tempo do contador (ms)

/* Global variables */

int volatile tick_count=0;  //contador de eventos

ISR(TIMER1_COMPA_vect)
{
/*cada vez que o contador atinge o valor do comparador */
tick_count++;
}



/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};




void setup(void)
{
	/* Set DIO pins to outputs */
	DDRH |= _BV(CLK_PIN) | _BV(DATA_PIN);
	DDRG |= _BV(LATCH_PIN);
	
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
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value)
{
	uint8_t x;
	bitClear(LATCH_PORT, LATCH_PIN);

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_MAP[Value] & (1 << (7 - i)));
		if (x) bitSet(DATA_PORT,DATA_PIN);
		else bitClear(DATA_PORT,DATA_PIN);
		bitSet(CLK_PORT, CLK_PIN);
		bitClear(CLK_PORT, CLK_PIN);
	}

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_SELECT[Segment] & (1 << (7 - i)));
		if (x) bitSet(DATA_PORT,DATA_PIN);
		else bitClear(DATA_PORT,DATA_PIN);
		bitSet(CLK_PORT, CLK_PIN);
		bitClear(CLK_PORT, CLK_PIN);
	}

	bitSet(LATCH_PORT, LATCH_PIN);
}

int main(void)
{
	/* Replace with your application code */
	setup();
	
	tick_count = 0;
	
	while (1)
	{
		
		for (int m=0; m<6; m++) {
			for (int c=0; c<10; c++) { 
		        for (int d=0;d<6;d++) { 
		            for (int u=0;u<10;u++) {
					    while(tick_count < REFRESH){ 
					        WriteNumberToSegment(0 , m);
					        WriteNumberToSegment(1 , c);
						    WriteNumberToSegment(2 , d);
							WriteNumberToSegment(3 , u);
							}
							tick_count = 0;
					     }
				      }
			     }
		     }
		   
	   }
}

