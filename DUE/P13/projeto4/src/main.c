/* Se for trocar o Timer Counter (TC) e o Channel, não esqueça de trocar o nome da Interrupt Handler de acordo
 * TC0, Channel 0 usam TC0_IRQn e nome do Handle TC0_Handler
 * TC0, Channel 1 usam TC1_IRQn e nome do Handle TC1_Handler
 * TC1, Channel 0 usam TC3_IRQn e nome do Handle TC3_Handler
 *
 */

#include <asf.h>

#define TC_WAVEFORM_TIMER_SELECTION TC_CMR_TCCLKS_TIMER_CLOCK4
#define TC_WAVEFORM_DIVISOR     128
#define TC_WAVEFORM_FREQUENCY   178
#define TC_WAVEFORM_DUTY_CYCLE  60

void TimerStart(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t freq);

//Defina o nome do interrupt handle de acordo com o Timer Counter utilizado
void TC0_Handler(void)
{
	//caso for usar interrupção
	tc_get_status(TC0, 0);
	gpio_toggle_pin(LED0_GPIO);
		
}

void TimerStart(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t freq)
{
	uint32_t ra, rc;
	
	pmc_set_writeprotect(false);
	pmc_enable_periph_clk(irq);
	tc_init(tc, channel,
	TC_WAVEFORM_TIMER_SELECTION // Waveform Clock Selection
	| TC_CMR_WAVE       // Waveform mode is enabled
	| TC_CMR_ACPA_SET   // RA Compare Effect: set
	| TC_CMR_ACPC_CLEAR // RC Compare Effect: clear
	| TC_CMR_WAVSEL_UP_RC     // UP mode with automatic trigger on RC Compare
	);
	
	rc = (sysclk_get_peripheral_bus_hz(tc) /
	      TC_WAVEFORM_DIVISOR /
	      TC_WAVEFORM_FREQUENCY);
	ra = ((100 - TC_WAVEFORM_DUTY_CYCLE) * rc) / 100;
	
	tc_write_ra(tc, channel, ra);
	tc_write_rc(tc, channel, rc);
	
	tc_start(tc, channel);
	//Em caso de usar interrupção para adicionar outra tarefa
	//tc->TC_CHANNEL[channel].TC_IER=  TC_IER_CPCS | TC_IER_CPAS;
	//NVIC_EnableIRQ(irq);
}



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

    //configura o pino TIOA0 (PB25 no DUE)
	sysclk_enable_peripheral_clock(ID_PIOB);
	PIOB->PIO_PDR |= PIO_PB25;
	PIOB->PIO_ABSR |= PIO_PB25;
	
	//configura o TC0 Channel 0 para um tempo de 200 Hz
	TimerStart(TC0, 0, TC0_IRQn, 200);
	
	while(1){
	}
}
