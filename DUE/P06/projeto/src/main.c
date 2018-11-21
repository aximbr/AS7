/*Etapa 06 fazer um LED acender de acordo com comando de uma chave                 */
/*                                                                                 */
/*Autor José P. Leitão                                                             */
/* 2018-11-20                                                                      */


#include <asf.h>

/*LED 1 da placa Multifunção  conectada a D.8 do Arduino DUE */
#define LED_MF_1            (PIO_PD8_IDX)
#define LED_MF_1_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

/*SW1 da placa multifunção conecta a A.24 do Arduino DUE */
#define SW_MF_1            (PIO_PA24_IDX)
#define SW_MF_1_FLAGS      (PIO_TYPE_PIO_INPUT| PIO_DEFAULT)

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	
	/* initiaate board components and MultiFunction components */
	board_init();
	gpio_configure_pin(LED_MF_1, LED_MF_1_FLAGS);
	gpio_configure_pin(SW_MF_1, SW_MF_1_FLAGS);
	

	/* Insert application code here, after the board has been initialized. */
	while(1){
		if (gpio_pin_is_low(SW_MF_1)) {
			  gpio_set_pin_low(LED_MF_1);      //LED turn off when high
			 
		   } else gpio_set_pin_high(LED_MF_1); //LED turn off when high
	}
}

