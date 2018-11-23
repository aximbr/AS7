/*Etapa 10 exibir uma mensagem no display Nokia 5110                               */
/*                                                                                 */
/*Autor José P. Leitão                                                             */
/* 2018-11-22                                                                      */


#include <asf.h>
#include "Nokia5110_DUE.h"


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	
	/* initiaate board components and MultiFunction components */
	board_init();
	nokia5110_init();
	

	/* Insert application code here, after the board has been initialized. */
	while (1){
			nokia5110_clear();
			for (int j=0; j<5; j++){
				nokia5110_set_XY(0, j);
				nokia5110_write_string("Projeto 10");
				delay_ms(1000);
				//gpio_toggle_pin(LED0_GPIO);
			    }
		     }
}
