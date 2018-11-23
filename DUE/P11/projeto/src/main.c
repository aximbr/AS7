/*Etapa 11 exibir uma mensagem no display Nokia 5110 piscar um led, quando uma     */
/* chave for pressionada. Utilizar o FreeRTOS para gerenciar as tarefas            */
/*Autor José P. Leitão                                                             */
/* 2018-11-22                                                                      */


#include <asf.h>
#include "Nokia5110_DUE.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "trcUser.h"


/*SW1 da placa multifunção conecta a A.24 do Arduino DUE */
#define SW_MF_1            (PIO_PA24_IDX)
#define SW_MF_1_FLAGS      (PIO_TYPE_PIO_INPUT| PIO_DEFAULT)

/* Fila para transferir dados de uma task para outra */
xQueueHandle MyQ;

/* The task functions prototype*/
void vReadSwitch(void *pvParameters);
void vBlinkOrangeLed(void *pvParameters);
void vDiplay_String_Nokia(void *pvParameters);

/* Hook Functions prototype */
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );


/*-----------------------------------------------------------*/
void vReadSwitch(void *pvParameters){
	uint32_t sw_status=0;   //status inicial é released
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(50);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		// Block until the next release time.
		sw_status = gpio_pin_is_low(SW_MF_1);  //lê status da chave
		xQueueSendToBack( MyQ, &sw_status, xDelay);
		
		//vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

//Task para fazer o Led Laranja Piscar

void vBlinkOrangeLed(void *pvParameters){
	uint32_t sw_status=0;
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(250);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		xQueueReceive(MyQ, &sw_status, xDelay);
		// Block until the next release time.
		if (sw_status) gpio_toggle_pin(LED0_GPIO); //toggle LED
		else gpio_set_pin_high(LED0_GPIO);  //turn off led on B.27
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint32_t sw_status=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
	
	
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		// Block until the next release time.
		xQueueReceive(MyQ, &sw_status, xDelay);
		nokia5110_set_XY(0,0);
		nokia5110_write_string("          ");
		if (sw_status) {
			nokia5110_set_XY(0,0);
			nokia5110_write_string("SW pressed ");
			}
		else {nokia5110_set_XY(0,0);
			nokia5110_write_string("SW free    ");
			}
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
	}
}


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
    sysclk_init();
	
	/* initiate board components */
	board_init();  //configura o LED da placa do DUE (B.27) LED0_GPIO
	nokia5110_init();
	nokia5110_clear();
	
	/*configura um SW em A.24 */
	gpio_configure_pin(SW_MF_1, SW_MF_1_FLAGS);

	/* cria a fila para receber o dado do botão */
	MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	
	if (MyQ == NULL)
	{
		//não consegui criar a fila }
	}
	
	/* cria as tasks do FreeRTOS */
	xTaskCreate(vReadSwitch, "SW status", 100, NULL, 1, NULL);
	xTaskCreate(vBlinkOrangeLed, "LED", 100, NULL, 1, NULL);
	xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler. The program should block here.
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;);
		
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
