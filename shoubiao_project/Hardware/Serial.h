#ifndef _SERIAL_H
#define _SERIAL_H
#include "stm32f10x.h"
uint8_t Usart_GetData(void);

uint8_t Usart_GetFlag(void);
void send_byte(uint8_t data);
void Serial_Init(void);
#endif
