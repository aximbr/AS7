/*Etapa 03 fazer dois leds da placa piscarem                                       */
/*                                                                                 */
/*Autor José P. Leitão                                                             */
/* 2018-11-20                                                                      */


#include <asf.h>

/*LED 1 da placa Multifunção  conectada a D.8 do Arduino DUE */
#define LED_MF_1            (PIO_PD8_IDX)
#define LED_MF_1_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	/* initiaate board components, include LED 0 */
	board_init();
	gpio_configure_pin(LED_MF_1, LED_MF_1_FLAGS);

	/* Insert application code here, after the board has been initialized. */
	while(1){
		gpio_set_pin_high(LED0_GPIO);
		delay_ms(500);
		gpio_set_pin_low(LED0_GPIO);
		//delay_ms(500);
		gpio_set_pin_high(LED_MF_1);
		delay_ms(500);
		gpio_set_pin_low(LED_MF_1);
		//delay_ms(500);
	}
}
