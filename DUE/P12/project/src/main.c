/**
 * \file
 *
 * \Projeto 12
 *
 */




#include <asf.h>
#include "adc_DUE.h"

int main (void)
{
	volatile uint16_t x;
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
	adc_due_init();

	/* Insert application code here, after the board has been initialized. */
	
	while(1){
		x = adc_due_read();
		
		__NOP;
		
	}

}
