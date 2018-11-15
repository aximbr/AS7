/*
 * Etapa 9 - piscar 2 Leds usando duas tasks do FreeRTOS
 *
 * Created: 07/09/2018 15:45:56
 * Author : jleitao
 */ 
#define _BV(n) (1 << n)

#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>


/*utilizar o LED interno em PB27 e SW1 em PF1 */

#define MyLED            (DDB7)   //B7 como output
#define MySW             (DDF1)   //F1 como input
#define DEBOUNCE_TIME    10       //Debounce for switch

portBASE_TYPE volatile resultado;
QueueHandle_t MyQ;  //fila para transferir dados de uma task para outra

/*Prototipo de funções locais */
void vReadSwitch(void *pvParameters);
static void vBlinkRedLed(void *pvParameters);
static void TaskBlinkLED2(void *pvParameters);

/*Global variable */


//Task para fazer ler o switch

void vReadSwitch(void *pvParameters) {
	uint8_t SW_Read, SW_Read_0, SW_Read_1, SW_State;
	SW_Read_0 = 0;
	SW_Read_1 = 0;
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(5);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		SW_Read = !(PINF & _BV(MySW)); //lê status da chave
		
		if (SW_Read == 0) {
			SW_Read_0++;
			SW_Read_1 = 0;
			if (SW_Read_0 >= DEBOUNCE_TIME){
				SW_Read_0 = DEBOUNCE_TIME +1;
			    SW_State = 0;}
		}
		else {
			SW_Read_1++;
		    SW_Read_0 = 0;}
			
		if (SW_Read_1 >= DEBOUNCE_TIME){
			SW_Read_1 = DEBOUNCE_TIME +1;
		    SW_State = 1;}
		xQueueOverwrite(MyQ, &SW_State);
		
		//vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

static void vBlinkRedLed(void *pvParameters) // LED posição D13
{
    uint8_t sw_status=0;
	
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(300);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		xQueuePeek(MyQ, &sw_status, portMAX_DELAY);
		// Block until the next release time.
		if (sw_status) PINB |= _BV(MyLED);   //troca
		else PORTB |= _BV(MyLED);  //apaga
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

static void TaskBlinkLED2(void *pvParameters) // Led posição D12
{
    (void) pvParameters;

    TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
    	PINB |=  _BV(PORTB6);       // alterna a porta B06 (Digital 12)
		vTaskDelayUntil( &xLastWakeTime, ( 400 / portTICK_PERIOD_MS ) );
    }
}


int main(void)
{
//Inicializa os LED´s (D13 e D12)
//DDRB |= _BV(DDB7);  //Coloca a porta B07 (Digital 13) como saída
DDRB |= _BV(DDB6);  //Coloca a porta B06 (Digital 12) como saída
DDRB |= _BV(MyLED); // Pin B7 is OUTPUT

/* inicializa as portas */
DDRF &= ~_BV(MySW);  //Pin F1 is INPUT
PORTF |= _BV(MySW);  // Pin F1 with pull-up resistor



/* cria a fila para receber o dado do botão */
MyQ = xQueueCreate(1,sizeof(uint8_t));	 //cria uma fila para um dado do tipo uint32_t

//if (MyQ == NULL)
//{
	//não consegui criar a fila }
//}
	
	xTaskCreate(vBlinkRedLed,  (const portCHAR *)"LED_1",  100,  NULL,  1,  NULL ); 
	
	xTaskCreate(TaskBlinkLED2,  (const portCHAR *)"LED_2",  100,  NULL,  1,  NULL ); 
	
	xTaskCreate(vReadSwitch, (const portCHAR *)"SW status", 100, NULL, 1, NULL );
	
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for(;;);
}
