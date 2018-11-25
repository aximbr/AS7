/*
 * projeto 8 - Simples aplicação com o FreeRTOS
 *
 * Created: 25/11/2018
 * Author : jleitao
 */ 

#define _BV(n) (1 << n)

// F_CPU defined on projet properties (definitions chapter)

#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>

#define LED1            (DDB7)   //B7 como output (LED1 da placa Multifunção)
#define LED2            (DDB6)   //B6 como output (LED2 da placa Multifunção)

void vPeriodicTask(void *pvParameters)
{
	// Establish the task's period.
	const TickType_t xDelay = pdMS_TO_TICKS(1000);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		// Block until the next release time.
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

static void TaskBlinkLED1(void *pvParameters) // Led1
{
    (void) pvParameters;

    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	DDRB |= _BV(LED1);

    while(1)
    {
    	PORTB |=  _BV(LED1);       // LED off. 
		vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_PERIOD_MS ) );

		PORTB &= ~_BV(LED1);       // LED on. 
		vTaskDelayUntil( &xLastWakeTime, ( 400 / portTICK_PERIOD_MS ) );
    }
}

static void TaskBlinkLED2(void *pvParameters) // Main Red LED Flash
{
    (void) pvParameters;

    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	DDRB |= _BV(LED2);

    while(1)
    {
    	PORTB |=  _BV(LED2);       // LED off. 
		vTaskDelayUntil( &xLastWakeTime, ( 200 / portTICK_PERIOD_MS ) );

		PORTB &= ~_BV(LED2);       // LED on. 
		vTaskDelayUntil( &xLastWakeTime, ( 400 / portTICK_PERIOD_MS ) );
    }
}

int main()
{
	//xTaskCreate(vPeriodicTask, "My Task", 256, NULL, 1, NULL);
	// Startup of the FreeRTOS scheduler. The program should block here.
	
	xTaskCreate(
	TaskBlinkLED1
	,  (const portCHAR *)"RedLED1" // LED1 da placa Multifunção
	,  256				// Tested 9 free @ 208
	,  NULL
	,  3
	,  NULL ); // */
	
	xTaskCreate(
	TaskBlinkLED2
	,  (const portCHAR *)"RedLED2" // LED2 da placa Multifunção
	,  256				// Tested 9 free @ 208
	,  NULL
	,  3
	,  NULL ); // */
	
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;);
}

