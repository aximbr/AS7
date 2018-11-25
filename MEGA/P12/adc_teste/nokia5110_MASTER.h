/*
 * nokia5110_MASTER.h
 *
 * Created: 30/10/2018 22:44:08
 *  Author: jleitao
 */ 


#ifndef NOKIA5110_MASTER_H_
#define NOKIA5110_MASTER_H_

#include <stdint.h>

void N5110_Cmnd(char DATA);

void N5110_char(char DATA);

void N5110_Data(char *DATA);

void N5110_Reset(void);					

void N5110_init(void);

void lcd_setXY(char x, char y);		

void N5110_clear(void);					

void N5110_image(const unsigned char *image_data);		

void N5110_OutUDec(uint16_t n);


#endif /* NOKIA5110_MASTER_H_ */