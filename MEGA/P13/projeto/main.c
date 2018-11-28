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

#define REFRESH 200  //Taxa de refresh e também base do contador

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};




void setup(void)
{
	/* Set DIO pins to outputs */
	DDRH |= _BV(CLK_PIN) | _BV(DATA_PIN);
	DDRG |= _BV(LATCH_PIN);
	
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
	int t= REFRESH; //refresh do display
	/* Replace with your application code */
	setup();
	
	
	while (1)
	{
		
		
		for (int m=0; m<10; m++) {
			for (int c=0; c<10; c++) { 
		        for (int d=0;d<10;d++) { 
		            for (int u=0;u<10;u++) {
					    while(t){ 
					        WriteNumberToSegment(0 , m);
					        WriteNumberToSegment(1 , c);
						    WriteNumberToSegment(2 , d);
							WriteNumberToSegment(3 , u);
							t--;}
					     t = REFRESH;
					}
				}
			}
		}
		   
	}
}

