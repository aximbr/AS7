/*
 * Etapa 11 controlar um Led piscando e exibir o status da chave no display Nokia utilizando tasks
 * no FreeRTOS
 * Created: 30/10/2018
 * Author : jleitao
 * observei uma estranho comportamento na porta PB7 quando ativado SPI, por isso optei usar outra porta para o led
 */ 

#define _BV(n) (1 << n)

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "SPI_Master_H_file.h"
#include "nokia5110_MASTER.h"

/*utilizar o LED interno em PB27 e SW1 em PF1 */

#define MyLED            (DDB6)   //B6 como output
#define MySW             (DDF1)   //F1 como input
#define DEBOUNCE_TIME    10       //Debounce for switch


QueueHandle_t MyQ;  //fila para transferir dados de uma task para outra

/*Prototipo de funções locais */
void vReadSwitch(void *pvParameters);
void vBlinkRedLed(void *pvParameters);
void vDisplay_String_Nokia(void *pvParameters);



//Task para fazer ler o switch

void vReadSwitch(void *pvParameters) {
	uint8_t SW_Read, SW_Read_0, SW_Read_1, SW_State;
	SW_Read_0 = 0;
	SW_Read_1 = 0;
	// Estabelece a duração da task
	//const TickType_t xDelay = pdMS_TO_TICKS(5);
	//TickType_t xLastWakeTime = xTaskGetTickCount();
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

//Task para fazer o Led Vermelho Piscar

void vBlinkRedLed(void *pvParameters) // LED posição PB.7
{
	uint8_t sw_status=0;
	
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(300);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		xQueuePeek(MyQ, &sw_status, portMAX_DELAY);
		// Block until the next release time.
		if (sw_status) PINB |= _BV(MyLED);   //troca
		else PORTB &= ~_BV(MyLED);  //apaga
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}


//Task para exibir uma mensagem no display Nokia
void vDisplay_String_Nokia(void *pvParameters){
	uint8_t sw_status=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
	
	
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		// Block until the next release time.
		xQueuePeek(MyQ, &sw_status, portMAX_DELAY);
		lcd_setXY(0,0);
		N5110_Data("          ");
		if (sw_status) {
			lcd_setXY(0,0);
			N5110_Data("SW pressed ");
			}
		else {
			lcd_setXY(0,0);
			N5110_Data("SW free ");}
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
	}
}

int main(void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	
	
	
	/*configura os pinos do LED e do SW */
	
	/* inicializa o LED */
DDRB |= _BV(MyLED); // Pin B7 is OUTPUT

/* inicializa as portas */
DDRF &= ~_BV(MySW);  //Pin F1 is INPUT
PORTF |= _BV(MySW);  // Pin F1 with pull-up resistor
	
		
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	SPI_Init();
	N5110_init();
		
	/* cria a fila para receber o dado do botão */
	MyQ = xQueueCreate(1,sizeof(uint8_t));	 //cria uma fila para um dado do tipo uint32_t
	
	if (MyQ == NULL)
	{
		//não consegui criar a fila }
	}
	
	/* cria as tasks do FreeRTOS */
	xTaskCreate(vReadSwitch, "SW status", 100, NULL, 1, NULL);
	xTaskCreate(vBlinkRedLed, "LED", 100, NULL, 1, NULL);
	xTaskCreate(vDisplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler. The program should block here.
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;);
	
}

