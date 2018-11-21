/*Etapa 01 fazer o led da placa acender                                            */
/*                                                                                 */
/*Autor Jos� P. Leit�o                                                             */
/* 2018-11-20                                                                      */


#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	/* initiaate board components, include LED 0 */
	board_init();

	/* Insert application code here, after the board has been initialized. */
	while(1){
		gpio_set_pin_high(LED0_GPIO);  //keep LED ON
	}
}
