#ifndef _DMA_H
#define _DMA_H
#include "stm32f10x.h"
void MyDMA_Init(uint32_t addrA,uint32_t addrB,uint16_t size);
void MyDMA_Transfer(void);
#endif
