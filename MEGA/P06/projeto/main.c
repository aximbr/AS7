/*
 * Projeto 6 - acender um LED quando pressionado um switch
 *
 * Created: 24/11/2018
 * Author : jleitao
 */ 
//#define F_CPU 16000000

#include <avr/io.h>
#ifndef _BV
#define _BV(n) (1 << n)
#endif

#define MyLED            (DDB6)   //B6 como output
#define MySW             (DDF1)   //F1 como input
#define DEBOUNCE_TIME    10       //Debounce for switch

int main(void)
{
    uint8_t SW_Read, SW_Read_0, SW_Read_1, SW_State;
    SW_Read_0 = 0;
    SW_Read_1 = 0;
	SW_State = 0;
	  
	/*configura os pinos do LED e do SW */
	
	/* inicializa o LED */
	DDRB |= _BV(MyLED); // Pin B6 is OUTPUT

	/* inicializa as portas */
	DDRF &= ~_BV(MySW);  //Pin F1 is INPUT
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
		
		if (SW_State) DDRB = _BV(MyLED); //turn-on LED 
		else DDRB &= ~_BV(MyLED);        //turn-off LED
		
    }
}

