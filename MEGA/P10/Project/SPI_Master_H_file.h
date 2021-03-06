#ifndef SPI_MASTER_H_FILE_H_
#define SPI_MASTER_H_FILE_H_

#include <avr/io.h>

#define MOSI 2
#define MISO 3
#define SCK 1
#define SS 0
#define DC 4
#define RST 5

void SPI_SS_Enable();
void SPI_SS_Disable();
void SPI_Init();
void SPI_Write(char write_data);
char SPI_Read();


#endif /* SPI_MASTER_H_FILE_H_ */