#ifndef _HARD_SPI_H
#define _HARD_SPI_H
#include "stm32f10x.h"
void Hard_SPI_Init(void);
void soft_spi_start(void);
void soft_spi_stop(void);
uint8_t Myspi_Swap_Byte(uint8_t ByteSend);
#endif