/**
 * \file
 *
 * \brief Etapa 3 - piscar 2 Leds
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
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
    ioport_init();
	/* Insert application code here, after the board has been initialized. */
	ioport_set_pin_dir(PIO_PB27_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(PIO_PD8_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PB27_IDX, HIGH);
	ioport_set_pin_level(PIO_PD8_IDX, HIGH);
	while(1){
		ioport_toggle_pin_level(PIO_PB27_IDX);   //Led1 da placa ou Led1 da placa de expansão
		delay_ms(1000);
		ioport_toggle_pin_level(PIO_PD8_IDX); //Led 2 da placa de expansão
		delay_ms(1000);
	}
}
