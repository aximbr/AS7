/*Etapa 11 exibir uma mensagem no display Nokia 5110 piscar um led, quando uma     */
/* chave for pressionada. Utilizar o FreeRTOS para gerenciar as tarefas            */
/*Autor José P. Leitão                                                             */
/* 2018-11-22                                                                      */


#include <asf.h>
#include "Nokia5110_DUE.h"
#include "adc_DUE.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "trcUser.h"

#define ADC_RESOLUTION  12

//protótipo de funções locais
uint32_t convert_adc(unsigned long x);
void vReadADC(void *pvParameters);
void vDiplay_String_Nokia(void *pvParameters);

/* Hook Functions prototype */
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

// variáveis globais
xQueueHandle MyQ;  //fila para transferir dados de uma task para outra

// programa principal
	
int main (void)
{

	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	board_init();
	/* inicializa o módulo ADC */
	adc_due_init();
	
		
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	nokia5110_init();
	nokia5110_clear();
	
	
	/* cria a fila para receber o dado do botão */
	MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	/* cria as tasks do FreeRTOS */
	xTaskCreate(vReadADC, "Ler ADC", 100, NULL, 1, NULL);
	xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler. The program should block here.
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;);
	
	
}

/*função que converte o valor lido do ADC */
uint32_t convert_adc(unsigned long x){
	uint32_t valor;
	
	valor = ((x * 3300) >> ADC_RESOLUTION); //resultado em milhares de milivolts
	return valor;
	
}

/*implementação das funções locais */

//Task para fazer ler o adc Fifo
void vReadADC(void *pvParameters){
	unsigned long adc_read;
	uint32_t my_valor;
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(50);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		adc_read = adc_due_read();
		my_valor = convert_adc( adc_read );
		
		// Block until the next release time.
		
		xQueueOverwrite( MyQ, &my_valor);
		
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint32_t valor=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
	
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	nokia5110_set_XY(0,0);
	nokia5110_write_string("Projeto 12");
	
	for (;;) {
		// Block until the next release time.
		xQueuePeek(MyQ, &valor, xDelay);
		nokia5110_set_XY(0,1);
		nokia5110_OutUDec( (unsigned short) valor );
		nokia5110_set_XY(7,1);
		nokia5110_write_string("mV");
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
	}
}


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}

