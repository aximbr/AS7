/*
 * Etapa 12 exibir o valor de saída de um divisor de tensão implementado com um potenciômetro deslizante
 * no display Nokia
 * Created: 25/11/2018
 * Author : jleitao
 * 
 */ 

#define _BV(n) (1 << n)

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "SPI_Master_H_file.h"
#include "nokia5110_MASTER.h"
#include "adc_MEGA.h"

#define ADC_RESOLUTION  10

//protótipo de funções locais
uint16_t convert_adc(uint16_t x);
void vReadADC(void *pvParameters);
void vDiplay_String_Nokia(void *pvParameters);


QueueHandle_t MyQ;  //fila para transferir dados de uma task para outra

/*função que converte o valor lido do ADC */
uint16_t convert_adc(uint16_t x){
	uint16_t valor;
	//MEGA utilizes 5V as Vref, use 5000UL pois é uma MCU de 8 bits
	valor = ((x * 5000UL) >> ADC_RESOLUTION); //resultado em milhares de milivolts
	return valor;
	
}


/*implementação das funções locais */

//Task para fazer ler o adc Fifo
void vReadADC(void *pvParameters){
	uint16_t adc_read;
	uint16_t my_valor;
	// Estabelece a duração da task
	const TickType_t xDelay = pdMS_TO_TICKS(50);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		adc_read = adc_mega_read();
		//my_valor = adc_mega_read();  //just to read the ADC valor
		my_valor = convert_adc( adc_read );
		
		// Block until the next release time.
		
		xQueueOverwrite( MyQ, &my_valor);
		
		vTaskDelayUntil(&xLastWakeTime, xDelay);
	}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint16_t valor=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
	
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	lcd_setXY(0,0);
	N5110_Data("Projeto 12");
	
	for (;;) {
		// Block until the next release time.
		xQueuePeek(MyQ, &valor, xDelay);
		lcd_setXY(0,1);
		N5110_OutUDec( valor );
		lcd_setXY(7,1);
		N5110_Data("mV");
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		
	}
}

int main(void)
{
	
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	SPI_Init();
	N5110_init();
	
	/*Inicializa o módulo ADC */
	adc_mega_init();
		
	/* cria a fila para receber o dado do botão */
	MyQ = xQueueCreate(1,sizeof(uint16_t));	 //cria uma fila para um dado do tipo uint16_t
	
	/* cria as tasks do FreeRTOS */
	xTaskCreate(vReadADC, "Ler ADC", 100, NULL, 1, NULL);
	xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL);
	
	// Startup of the FreeRTOS scheduler. The program should block here.
	vTaskStartScheduler();
	// The following line should never be reached. Failure to allocate enough
	// memory from the heap would be one reason.
	for (;;);
	
}

