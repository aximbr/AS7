/**
 * \file
 *
 * \brief Etapa 4 - piscar 1 Led usando Systick e contador
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
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

int main (void)
{
	int i=0;
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
	Disable_global_interrupt();
	SysTick_Config(sysclk_get_cpu_hz() / 1000);  //configura o Systick para o tempo de 1ms
	SysTick->CTRL = 0x5;                         //habilita o systick sem interrupção
    Enable_global_interrupt();
	
    ioport_set_pin_level(LED0_GPIO,LOW);
    while(1){
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		i++;
		SysTick->VAL = 0; //reset o flag do Systick
		if(i==1000){
			ioport_toggle_pin_level(LED0_GPIO);
			i=0;
		}
	  }
	}
}	
    
