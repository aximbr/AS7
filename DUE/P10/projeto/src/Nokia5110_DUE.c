/*
 * Nokia5110_DUE.c
 *
 * Created: 21/11/2018 21:34:44
 *  Author: jleitao
 */ 
#include "Nokia5110_DUE.h"
#include "spi.h"
#include "gpio.h"
#include "sysclk.h"

/* Define os pinos RST (D.7) e D/C (D.8)  */
#define RST_PIN            (PIO_PD7_IDX)
#define RST_PIN_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define DC_PIN             (PIO_PD8_IDX)
#define DC_PIN_FLAGS       (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define BAUD_RATE          1000000


void nokia5110_write_byte(unsigned char data, unsigned char dc) {
	
	spi_enable(SPI0);            /*enable interface SPI */
	for(int i=0; i<300; i++);            // wait for clock stabilize
	if(dc) gpio_set_pin_high(DC_PIN);    // DC = 1 (DATA)
	else gpio_set_pin_low(DC_PIN);       // DC = 0 (COMMAND)

	
	spi_write(SPI0, data, 0, 0); // wait for TX buffer is empty and then Transmits the byte
	
	spi_disable(SPI0);          /* Disable the SPI (clock e CS)  */

}

void nokia5110_clear(void) {
	unsigned int i;
	nokia5110_write_byte(0x0c, 0);
	nokia5110_write_byte(0x80, 0);
	for (i=0; i<504; i++)
	nokia5110_write_byte(0, 1);
}

void nokia5110_init(void){
	uint16_t baud_rate_div;
	/*inicializa o módulo SPI0 com as seguintes configurações: 
	MASTER
	clock = 1 MHZ
	Modo = 1 (idle clock, phase xxx)
	tamanho da palavra = 8 bits
	*/
	spi_enable_clock(SPI0);
	/* SPI MODE | CPOL  | NCPHA
	      0     |   0   |   1
		  1     |   0   |   0
		  2     |   1   |   1
		  3     |   1   |   0
    Nokia5110 works on mode 0, 1 or 3
	*/
	//spi_set_clock_polarity(SPI0, 0, SPI_CSR_CPOL );
	//spi_set_clock_phase(SPI0, 0, SPI_CSR_NCPHA);
	spi_configure_cs_behavior(SPI0, 0, SPI_CS_RISE_NO_TX);
	spi_set_bits_per_transfer(SPI0, 0, SPI_CSR_BITS_8_BIT);
	baud_rate_div = spi_calc_baudrate_div( BAUD_RATE, sysclk_get_cpu_hz());
	spi_set_baudrate_div(SPI0, 0, baud_rate_div);
	spi_set_master_mode(SPI0);
	//spi_enable(SPI0);
	gpio_configure_pin(RST_PIN, RST_PIN_FLAGS);
	gpio_configure_pin(DC_PIN, DC_PIN_FLAGS);
	
	/* Reset Nokia module */
	gpio_set_pin_low(RST_PIN);
	for(int i=0; i< 100000; i++) {}   //3.57 ms
	gpio_set_pin_high(RST_PIN);	
	
	/*send configuration to Nokia */
	nokia5110_write_byte(0x21, 0);    // use the extended command set the nokia5110 mode
	nokia5110_write_byte(0xB8, 0);    // set the contrast
	nokia5110_write_byte(0x04, 0);    // temperature correction
	nokia5110_write_byte(0x13, 0);    // 1:48
	nokia5110_write_byte(0x20, 0);    // use basic commands
	nokia5110_clear();                // clear the screen
	nokia5110_write_byte (0x0c, 0);   // set display mode, the normal display
	
}

//********nokia5110_set_XY*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// Inputs: newX  new X-position of the cursor (0<=newX<=11)
//         newY  new Y-position of the cursor (0<=newY<=5)
// Outputs: none
void nokia5110_set_XY(uint8_t newX, uint8_t newY){
   if((newX > 11) || (newY > 5)){        // bad input
    return;                             // do nothing
   }
  // multiply newX by 7 because each character is 7 columns wide
   nokia5110_write_byte(0x80|newX*7,0);     // setting bit 7 updates X-position
   nokia5110_write_byte(0x40|newY, 0);      // setting bit 6 updates Y-position
}


void nokia5110_write_char(unsigned char c) {
unsigned char line;
   //nokia5110_write_byte(0x00, 1);
   for (line=0; line<5; line++)
   nokia5110_write_byte(ASCII[c-32][line], 1);
   nokia5110_write_byte(0x00, 1);
}

void nokia5110_write_string(const char *s) {
   while (*s) nokia5110_write_char(*s++);
}

//********nokia5110_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// assumes: nokia5110 is in default horizontal addressing mode (V = 0)
void nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    nokia5110_write_string("     ");
    nokia5110_write_char(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    nokia5110_write_string("     ");
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  } else if(n<1000){
    nokia5110_write_string("   ");
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    nokia5110_write_char(' ');
    nokia5110_write_char(n/1000+'0'); /* thousands digit */
    nokia5110_write_char(',');
		n = n%1000;
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
  else {
    nokia5110_write_char(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    nokia5110_write_char(n/1000+'0'); /* thousands digit */
		nokia5110_write_char(',');
    n = n%1000;
    nokia5110_write_char(n/100+'0'); /* hundreds digit */
    n = n%100;
    nokia5110_write_char(n/10+'0'); /* tens digit */
    nokia5110_write_char(n%10+'0'); /* ones digit */
  }
}


