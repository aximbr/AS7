/*
 * Projeto 13 - simples biblioteca para comunicar com display de 7 segmentos (4 dígitos) 
 * utilizando o Multi function shield - baseado no CHIP 74HC595
 * Created: 28/11/2018 
 * Author : jleitao
 */ 



#include <asf.h>

#ifndef _BV
#define _BV(n) (1 << n)
#endif

#define bitSet(_pin)    gpio_set_pin_high(_pin)
#define bitClear(_pin)  gpio_set_pin_low(_pin)

/* Define shift register pins used for seven segment display */
//#define LATCH_PORT  PORTC
//#define CLK_PORT    PORTC
//#define DATA_PORT   PORTC

#define LATCH_PIN    (PIO_PC26_IDX)  //D4 <-> C.26
#define CLK_PIN      (PIO_PC23_IDX)  //D7 <-> C.23
#define DATA_PIN     (PIO_PC22_IDX)  //D8 <-> C.22


#define LATCH_PIN_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define CLK_PIN_FLAGS        (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define DATA_PIN_FLAGS       (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

#define REFRESH 1000  //Taxa de refresh ou base de tempo do contador (ms)

/* Global variables */

int volatile tick_count=0;  //contador de eventos

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

/*prototypes for local functions */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value);

/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void)
{
	tick_count++;
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value)
{
	uint8_t x;
	bitClear(LATCH_PIN);

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_MAP[Value] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_SELECT[Segment] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	bitSet(LATCH_PIN);
}


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
	
	/* initialise  Systick  for 1 ms @ 16MHz clock */
	
	SysTick->CTRL  = 0x00;                                      /* disable systick during setup */
	SysTick->LOAD  = (80000UL) - 1;	                            /* set reload register 1 ms when clock is 16MHz */
	NVIC_SetPriority (SysTick_IRQn, 15);			            /* set Priority for Systick Interrupt */
	SysTick->VAL   = 0;											/* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |               /* Clock source is MCK */
	                 SysTick_CTRL_TICKINT_Msk   |               /* Assert SysTick Exception when counter reaches zero */
	                 SysTick_CTRL_ENABLE_Msk;					/* Enable SysTick IRQ and SysTick Timer */
	 
    /* configure the pins for Latch, Clock and Data */
	gpio_configure_pin(LATCH_PIN, LATCH_PIN_FLAGS);
	gpio_configure_pin(CLK_PIN, CLK_PIN_FLAGS);
	gpio_configure_pin(DATA_PIN, DATA_PIN_FLAGS);

	tick_count = 0;    //reset tick count
		
		while (1)
		{
			
			for (int m=0; m<6; m++) {
				for (int c=0; c<10; c++) {
					for (int d=0;d<6;d++) {
						for (int u=0;u<10;u++) {
							while(tick_count < REFRESH){   //keeping display the numbers before increment a unit 
								WriteNumberToSegment(0 , m);
								WriteNumberToSegment(1 , c);
								WriteNumberToSegment(2 , d);
								WriteNumberToSegment(3 , u);
								
							}
							tick_count = 0;   //reset tick count
						}
					}
				}
			}
			
		}
	}
