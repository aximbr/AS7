/**
 * \file
 *
 * \brief Etapa 5 - Usar Systick com interrupção para piscar o Led
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>

/*global variables */
uint32_t volatile tick_count;

/*prototipo de funções locais */
void my_delay(uint32_t tempo);

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
    Disable_global_interrupt();
	/*Systick configuration */
	SysTick_Config(sysclk_get_cpu_hz()/1000);   //configura 1 tick = 1ms
	Enable_global_interrupt();
	
	/* Insert application code here, after the board has been initialized. */
	
	ioport_set_pin_level(LED0_GPIO,LOW);
	
	while(1){
		ioport_toggle_pin_level(LED0_GPIO);
		my_delay(1000);
	}
}

/*implementação de funções locais */
void my_delay(uint32_t tempo){
	tick_count = 0;
	while(tick_count < tempo);
}

/*implementação de ISR */
void SysTick_Handler(void){
	tick_count++;
}