/*Etapa 02 fazer o led da placa piscar                                             */
/*                                                                                 */
/*Autor José P. Leitão                                                             */
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
		gpio_set_pin_high(LED0_GPIO);
		delay_ms(1000);
		gpio_set_pin_low(LED0_GPIO);
		delay_ms(1000);
	}
}
