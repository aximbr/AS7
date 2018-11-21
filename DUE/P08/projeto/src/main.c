/*Etapa 08 rodar uma tarefa simples com FreeRTOS                                   */
/*                                                                                 */
/*Autor José P. Leitão                                                             */
/* 2018-11-20                                                                      */


#include <asf.h>

#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
//#include "trcUser.h"


/* The task functions prototype*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Hook Functions prototype */
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );


/*-----------------------------------------------------------*/


/**
 *  Configure UART console.
 */
// [main_console_configure]
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}


int main( void )
{
   /* This function initializes the MCU clock, PLL will be used to generate Main MCU clock */
	sysclk_init();
	
	/* essential Board initializations */
	board_init();
	
	/* Initialize the serial I/O(console ), This function configures Due's CONSOLE_UART */
	configure_console();

	/*Allocates, if necessary, and initializes the recorder data structure */
	//vTraceInitTraceData();

	/*Starts the recorder*/
	//uiTraceStart();
	
	/* Create one of the two tasks. */
	xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
					"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
					100,		/* Stack depth in words. */
					NULL,		/* We are not using the task parameter. */
					1,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	xTaskCreate( vTask2, "Task 2", 100, NULL, 1, NULL );

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		taskENTER_CRITICAL();
		printf( "Task 1 is running\n" );
		taskEXIT_CRITICAL();

		/* Delay for a period.  This time we use a call to vTaskDelay() which
		puts the task into the Blocked state until the delay period has expired.
		The delay period is specified in 'ticks'. */
		vTaskDelay( 250 / portTICK_RATE_MS );
		
		
	}

	/* we have not used vTaskDelete() function here */

}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		taskENTER_CRITICAL();
		printf( "Task 2 is running\n" );
		taskEXIT_CRITICAL();

		/* Delay for a period.  This time we use a call to vTaskDelay() which
		puts the task into the Blocked state until the delay period has expired.
		The delay period is specified in 'ticks'. */
		vTaskDelay( 250 / portTICK_RATE_MS );
		
		
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
